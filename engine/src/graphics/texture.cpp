#include "stdafx.h"
#include "texture.h"
#include "stb_image_write.h"
#include "graphics/textureParameters.h"
#include "math/math.h"
#include "util/utils.h"

namespace Diamond {
	Texture::Texture(TextureParameters params, uint32_t width, uint32_t height, const void* data, bool hasMipmaps, bool keepData)
		: m_params(params), m_width(width), m_height(height) {
		if (data) m_data = Buffer::copy(data, Utils::getImageMemorySize(m_params, m_width, m_height));
	}

	Texture::Texture(TextureParameters params, uint32_t width, uint32_t height, bool keepData)
		: m_params(params), m_width(width), m_height(height) {
	}

	Texture::~Texture() {
		cleanup();
	}

	void Texture::cleanup() {
		if (m_handle) {
			glDeleteTextures(1, &m_handle);
		}
		m_data.cleanup();
	}

	void Texture::invalidate() {
		if (m_handle) cleanup();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);

		if (m_hasMipmaps) {
			m_mipmapCount = 1 + Math::floor(Math::log2(Math::min(m_width, m_height)));
		} else m_mipmapCount = 0;

		glTextureStorage2D(m_handle, m_mipmapCount, m_params.getInternalFormat(), m_width, m_height);

		if (m_data) {
			glTextureSubImage2D(m_handle, 0, 0, 0, m_width, m_height, m_params.getImageFormat(), m_params.getDataType(), m_data.getData());
			if (m_hasMipmaps) glGenerateMipmap(GL_TEXTURE_2D);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, m_params.getWrap());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_params.getWrap());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_params.getWrap());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_params.getFilter(GL_TEXTURE_MIN_FILTER, m_hasMipmaps));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_params.getFilter(GL_TEXTURE_MAG_FILTER, m_hasMipmaps));


		if (m_hasMipmaps) {
			if (GLEW_EXT_texture_filter_anisotropic) {
				float value = 0;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
				float amount = Math::min(4.0f, value);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
			} else LOG_WARN("[~gTexture~x] GL_EXT_texture_filter_anisotropic not supported");
		}
	}

	void Texture::bind(uint32_t slot /*= 0*/) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_handle);
	}

	void Texture::unbind(uint32_t slot /*= 0*/) {
		glBindTexture(GL_TEXTURE_2D, slot);
	}

	void Texture::resize(uint32_t width, uint32_t height) {
		if (m_width == width && m_height == height) return;
		m_width = width;
		m_height = height;

		invalidate();
	}

	void Texture::saveAsPNG(const std::string& file) {
		if (m_data) {
			uint32_t channels = m_params.getChannelCount();
			stbi_write_png(file.c_str(), getWidth(), getHeight(), channels, m_data.getData(), channels * getWidth());
			LOG("[~gTexture~x] saved texture to ~1%s~x", file.c_str());
		} else LOG("[~gTexture~x] failed to save texture because texture has no data");
	}
}