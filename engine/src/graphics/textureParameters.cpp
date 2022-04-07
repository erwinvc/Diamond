#include "stdafx.h"
#include "textureParameters.h"

std::string TextureParameters::WrapToString(TextureWrap wrap) const {
	switch (wrap) {
		case CLAMP: return "CLAMP";
		case REPEAT: return "REPEAT";
		case MIRRORED_REPEAT: return "MIRRORED_REPEAT";
		case CLAMP_TO_EDGE: return "CLAMP_TO_EDGE";
		case CLAMP_TO_BORDER: return "CLAMP_TO_BORDER";
	}
	return "NULL";
}

std::string TextureParameters::FilterToString(TextureFilter filter) const {
	switch (filter) {
		case LINEAR: return "LINEAR";
		case NEAREST: return "NEAREST";
	}
	return "NULL";
}

GLenum TextureParameters::getFilter(int type, bool mipmap) const {
	switch (type) {
		case GL_TEXTURE_MIN_FILTER: {
			switch (m_filter) {
				case LINEAR: return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
				case NEAREST: return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST;
			}
		}break;
		case GL_TEXTURE_MAG_FILTER: return m_filter == LINEAR ? GL_LINEAR : GL_NEAREST; break;
	}
	return GL_LINEAR;
}