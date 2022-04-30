#pragma once

enum TextureWrap {
	CLAMP = 0x2900,
	REPEAT = 0x2901,
	MIRRORED_REPEAT = 0x8370,
	CLAMP_TO_EDGE = 0x812F,
	CLAMP_TO_BORDER = 0x812D
};

enum TextureFilter {
	LINEAR,
	NEAREST
};

enum TextureFormat {
	RGB,
	RGBA,
	RGBA16F,
	RGBA32F,
	SRGB,
	SRGBA,
	DEPTH32F,
	DEPTH24STENCIL8
};

class TextureParameters final {
private:
	TextureFormat m_format;
	TextureFilter m_filter;
	TextureWrap m_wrap;
	bool m_flipY;

	std::string WrapToString(TextureWrap wrap) const;
	std::string FilterToString(TextureFilter filter) const;
public:

	TextureParameters(TextureFormat format, TextureFilter filter = LINEAR, TextureWrap wrap = REPEAT, bool flipY = true)
		: m_format(format), m_filter(filter), m_wrap(wrap), m_flipY(flipY) {
	}

	inline bool getFlipY() const { return m_flipY; }
	inline GLenum getWrap() const { return m_wrap; }
	GLenum getFilter(int type, bool mipmap) const;

	inline uint32_t getChannelCount() {
		switch (m_format)
		{
			case TextureFormat::RGB:
			case TextureFormat::SRGB:    return 3;
			case TextureFormat::SRGBA:    
			case TextureFormat::RGBA:    return 4;
			case TextureFormat::RGBA16F: return 2 * 4;
			case TextureFormat::RGBA32F: return 4 * 4;
		}
		return 0;
	}

	inline GLenum getImageFormat() const {
		switch (m_format) {
			case TextureFormat::RGB:     return GL_RGB;
			case TextureFormat::SRGB:    return GL_RGB;
			case TextureFormat::SRGBA:    return GL_RGBA;
			case TextureFormat::RGBA:
			case TextureFormat::RGBA16F:
			case TextureFormat::RGBA32F: return GL_RGBA;
		}
		return 0;
	}

	inline GLenum getInternalFormat() const {
		switch (m_format) {
			case TextureFormat::RGB:             return GL_RGB8;
			case TextureFormat::SRGB:            return GL_SRGB8;
			case TextureFormat::SRGBA:           return GL_SRGB8_ALPHA8;
			case TextureFormat::RGBA:            return GL_RGBA8;
			case TextureFormat::RGBA16F:         return GL_RGBA16F;
			case TextureFormat::RGBA32F:         return GL_RGBA32F;
			case TextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
			case TextureFormat::DEPTH32F:        return GL_DEPTH_COMPONENT32F;
		}
		return 0;
	}

	inline GLenum getDataType() const {
		switch (m_format) {
			case TextureFormat::RGB:
			case TextureFormat::SRGB:
			case TextureFormat::RGBA:    return GL_UNSIGNED_BYTE;
			case TextureFormat::RGBA16F:
			case TextureFormat::RGBA32F: return GL_FLOAT;
		}
		return 0;
	}
};