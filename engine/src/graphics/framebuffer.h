#pragma once
#include "stdincl.h"
#include "ref.h"
#include "util/color.h"
#include "textureParameters.h"
#include "util/singleton.h"

namespace Diamond {
	class Texture;
	class FrameBufferManager;

	enum class FBOAttachment {
		COLOR,
		DEPTH,
		STENCIL,
		DEPTHSTENCIL
	};

	enum FBOScale {
		STATIC,
		FULL,
		HALF,
		QUARTER,
		ONEEIGHTH
	};

	class FrameBuffer {
	private:
		std::vector<AssetRef<Texture>> m_textures;
		std::vector<std::string> m_textureNames;
		FBOScale m_scale;
		std::string m_name;
		uint32_t m_colorAttachments = 0;
		uint32_t m_fbo = 0;
		uint32_t m_width, m_height;
		uint32_t m_realWidth, m_realHeight;
		Color m_clearColor;
		bool m_hasDepth;
		bool m_hasStencil;

		const uint32_t drawBuffers[16] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4,
			GL_COLOR_ATTACHMENT5,
			GL_COLOR_ATTACHMENT6,
			GL_COLOR_ATTACHMENT7,
			GL_COLOR_ATTACHMENT8,
			GL_COLOR_ATTACHMENT9,
			GL_COLOR_ATTACHMENT10,
			GL_COLOR_ATTACHMENT11,
			GL_COLOR_ATTACHMENT12,
			GL_COLOR_ATTACHMENT13,
			GL_COLOR_ATTACHMENT14,
			GL_COLOR_ATTACHMENT15
		};

		FrameBuffer(std::string name, FBOScale scale, const Color& clearColor = Color::black());
		FrameBuffer(std::string name, uint32_t width, uint32_t height, const Color& clearColor = Color::black());
		~FrameBuffer();

		bool CheckStatus();

		friend FrameBufferManager;

		void SetScale(FBOScale scale);
		float FBOScaleToFloat(FBOScale scale);

	public:
		AssetRef<Texture> AddBuffer(const std::string& name, const TextureParameters& params, FBOAttachment type = FBOAttachment::COLOR);

		const std::string& GetName() { return m_name; }

		void Bind() const;
		void Unbind() const;
		void Clear() const;
		void ClearDepthOnly() const;
		void ClearColorOnly() const;
		void ClearStencilOnly() const;
		void SetDrawAndReadBuffersToNone();

		void Blit(FrameBuffer* targetFBO);
		void BlitColorOnly(FrameBuffer* targetFBO);
		void BlitDepthOnly(FrameBuffer* targetFBO);

		void Resize(uint32_t width, uint32_t height);

		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }
		FBOScale GetScale() const { return m_scale; }
		uint32_t GetHandle() const { return m_fbo; }
		bool HasDepth() const { return m_hasDepth; }
		void SetClearColor(Color& color) { m_clearColor = color; }
		std::vector<AssetRef<Texture>>& GetTextures() { return m_textures; }
		std::vector<std::string>& GetTextureNames() { return m_textureNames; }
		AssetRef<Texture> GetDepthTexture() { return m_textures[0]; }
	};

	class FrameBufferManager : public Singleton<FrameBufferManager> {
	private:
		std::vector<FrameBuffer*> m_frameBuffers;
		FrameBufferManager() {}
		~FrameBufferManager() {}
		friend Singleton;

		uint32_t m_width = 0;
		uint32_t m_height = 0;

	public:
		AssetRef<FrameBuffer> Create(const std::string& name, uint32_t width, uint32_t height, const Color& clearColor = Color::black());
		AssetRef<FrameBuffer> Create(const std::string& name, FBOScale scale, const Color& clearColor = Color::black());

		void BindDefaultFBO();

		void OnResize(uint32_t width, uint32_t height);
	};

	inline FrameBufferManager* GetFrameBufferManager() { return FrameBufferManager::getInstance(); }
}