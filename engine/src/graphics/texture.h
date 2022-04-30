#pragma once
#include "stdincl.h"
#include "buffer.h"
#include "graphics\textureParameters.h"

namespace Diamond {
	class Texture {
	private:
		TextureParameters m_params;
		Buffer m_data;
		uint32_t m_handle = 0;
		uint32_t m_width, m_height;
		bool m_hasMipmaps;
		uint8_t m_mipmapCount = 0;
		void cleanup();

	public:

		Texture(TextureParameters params, uint32_t width, uint32_t height, const void* data, bool hasMipmaps, bool keepData = false);
		Texture(TextureParameters params, uint32_t width, uint32_t height, bool keepData = false);
		~Texture();

		void invalidate();

		uint32_t getHandle() { return m_handle; }

		void bind(uint32_t slot = 0);
		void unbind(uint32_t slot = 0);
		void resize(uint32_t width, uint32_t height);

		inline const uint32_t getWidth() const { return m_width; }
		inline const uint32_t getHeight() const { return m_height; }
		void saveAsPNG(const std::string& file);
	};
}
