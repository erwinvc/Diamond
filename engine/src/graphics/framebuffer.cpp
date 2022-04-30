#include "stdafx.h"
#include "framebuffer.h"
#include "util/color.h"
#include "application.h"
#include "ref.h"
#include "glError.h"
#include "texture.h"
#include "util/utils.h"

namespace Diamond {
	FrameBuffer::FrameBuffer(std::string name, FBOScale scale, const Color& clearColor)
		: m_name(name), m_scale(scale), m_width(0), m_height(0), m_clearColor(clearColor), m_hasDepth(false), m_hasStencil(false) {
		m_realWidth = getApp()->getWidth();
		m_realHeight = getApp()->getHeight();
		m_width = (uint32_t)(FBOScaleToFloat(m_scale) * m_realWidth);
		m_height = (uint32_t)(FBOScaleToFloat(m_scale) * m_realHeight);

		GL(glGenFramebuffers(1, &m_fbo));
		GL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
		GL(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
	}

	FrameBuffer::FrameBuffer(std::string name, uint32_t width, uint32_t height, const Color& clearColor)
		: m_name(name), m_scale(FBOScale::STATIC), m_width(0), m_height(0), m_clearColor(clearColor), m_hasDepth(false), m_hasStencil(false) {
		m_realWidth = width;
		m_realHeight = height;
		m_width = (uint32_t)(FBOScaleToFloat(m_scale) * m_realWidth);
		m_height = (uint32_t)(FBOScaleToFloat(m_scale) * m_realHeight);

		GL(glGenFramebuffers(1, &m_fbo));
		GL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
		GL(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
	}

	FrameBuffer::~FrameBuffer() {
		GL(glDeleteFramebuffers(1, &m_fbo));
		for (AssetRef<Texture> texture : m_textures) {
			delete texture.Get();
		}
	}

	void FrameBuffer::Resize(uint32_t width, uint32_t height) {
		if (m_scale == FBOScale::STATIC) return;
		m_realWidth = width;
		m_realHeight = height;
		m_width = (uint32_t)(FBOScaleToFloat(m_scale) * m_realWidth);
		m_height = (uint32_t)(FBOScaleToFloat(m_scale) * m_realHeight);

		for (Texture* texture : m_textures) {
			texture->resize(m_width, m_height);
		}

		CheckStatus();
	}


	void FrameBuffer::SetScale(FBOScale scale) {
		if (m_scale == scale) return;
		m_scale = scale;
		Resize(m_realWidth, m_realHeight);
	}

	float FrameBuffer::FBOScaleToFloat(FBOScale scale) {
		switch (scale) {
			case FBOScale::FULL: return 1.0f;
			case FBOScale::HALF: return 0.5f;
			case FBOScale::QUARTER: return 0.25f;
			case FBOScale::ONEEIGHTH: return 0.125f;
		}
		return 1.0f;
	}

	bool FrameBuffer::CheckStatus() {
		GL(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			LOG_ERROR("[~cFBO~x] ~rFramebuffer %s failed: %s", m_name.c_str(), GLUtils::getFBOStatus(status));
			return false;
		} else return true;
	}

	AssetRef<Texture> FrameBuffer::AddBuffer(const std::string& name, const TextureParameters& params, FBOAttachment type) {
		Texture* texture = new Texture(params, m_width, m_height, false);
		texture->invalidate();
		m_textures.push_back(texture);
		m_textureNames.push_back(name);

		Bind();

		if (type == FBOAttachment::COLOR) {
			GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_colorAttachments, GL_TEXTURE_2D, texture->getHandle(), 0));
			GL(glDrawBuffers(++m_colorAttachments, drawBuffers));
		} else if (type == FBOAttachment::DEPTH) {
			GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getHandle(), 0));
			m_hasDepth = true;
		} else if (type == FBOAttachment::STENCIL) {
			GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture->getHandle(), 0));
			m_hasStencil = true;
		} else if (type == FBOAttachment::DEPTHSTENCIL) {
			GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture->getHandle(), 0));
			m_hasDepth = true;
			m_hasStencil = true;
		}

		GL(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
		if (status != GL_FRAMEBUFFER_COMPLETE) LOG_ERROR("[~cFBO~x] ~radding attachment to %s failed: %s", m_name.c_str(), GLUtils::getFBOStatus(status));
		Unbind();

		return texture;
	}

	void FrameBuffer::Blit(FrameBuffer* targetFBO) {
		GL(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo));
		GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFBO ? targetFBO->GetHandle() : 0));
		uint32_t w = targetFBO ? targetFBO->GetWidth() : getApp()->getWidth();
		uint32_t h = targetFBO ? targetFBO->GetHeight() : getApp()->getHeight();
		GL(glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, w, h, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST));
	}

	void FrameBuffer::BlitColorOnly(FrameBuffer* targetFBO) {
		GL(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo));
		GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFBO ? targetFBO->GetHandle() : 0));
		uint32_t w = targetFBO ? targetFBO->GetWidth() : getApp()->getWidth();
		uint32_t h = targetFBO ? targetFBO->GetHeight() : getApp()->getHeight();
		GL(glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	}

	void FrameBuffer::BlitDepthOnly(FrameBuffer* targetFBO) {
		GL(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo));
		GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFBO ? targetFBO->GetHandle() : 0));
		uint32_t w = targetFBO ? targetFBO->GetWidth() : getApp()->getWidth();
		uint32_t h = targetFBO ? targetFBO->GetHeight() : getApp()->getHeight();
		GL(glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, w, h, GL_DEPTH_BUFFER_BIT, GL_NEAREST));
	}

	void FrameBuffer::Bind() const {
		GL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
		GL(glViewport(0, 0, m_width, m_height));
	}
	void FrameBuffer::Unbind() const {
		GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	void FrameBuffer::Clear() const {
		GL(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
		GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	}

	void FrameBuffer::ClearDepthOnly() const {
		GL(glClear(GL_DEPTH_BUFFER_BIT));
	}

	void FrameBuffer::ClearColorOnly() const {
		GL(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
		GL(glClear(GL_COLOR_BUFFER_BIT));
	}

	void FrameBuffer::ClearStencilOnly() const {
		GL(glClear(GL_STENCIL_BUFFER_BIT));
	}

	void FrameBuffer::SetDrawAndReadBuffersToNone() {
		Bind();
		GL(glDrawBuffer(GL_NONE));
		GL(glReadBuffer(GL_NONE));
		Unbind();
	}

	AssetRef<FrameBuffer> FrameBufferManager::Create(const std::string& name, uint32_t width, uint32_t height, const Color& clearColor) {
		for (FrameBuffer* fbo : m_frameBuffers) {
			if (fbo->GetName().compare(name) == 0) {
				LOG_ERROR("[~cFBO~x] Framebuffer ~1%s~x already exists", fbo->GetName().c_str());
				return AssetRef<FrameBuffer>(fbo);
			}
		}
		AssetRef<FrameBuffer> fbo = new FrameBuffer(name, width, height, clearColor);
		m_frameBuffers.push_back(fbo);
		return AssetRef<FrameBuffer>(fbo);
	}

	AssetRef<FrameBuffer> FrameBufferManager::Create(const std::string& name, FBOScale scale, const Color& clearColor) {
		for (FrameBuffer* fbo : m_frameBuffers) {
			if (fbo->GetName().compare(name) == 0) {
				LOG_ERROR("[~cFBO~x] Framebuffer ~1%s~x already exists", fbo->GetName().c_str());
				return AssetRef<FrameBuffer>(fbo);
			}
		}
		AssetRef<FrameBuffer> fbo = new FrameBuffer(name, scale, clearColor);
		m_frameBuffers.push_back(fbo);
		return AssetRef<FrameBuffer>(fbo);
	}

	void FrameBufferManager::BindDefaultFBO() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GL(glViewport(0, 0, getApp()->getWidth(), getApp()->getHeight()));
	}

	void FrameBufferManager::OnResize(uint32_t width, uint32_t height) {
		if (m_width == width && m_height == height)return;
		m_width = width;
		m_height = height;
		for (FrameBuffer* fbo : m_frameBuffers) fbo->Resize(m_width, m_height);
	}
}