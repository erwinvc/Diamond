#include "stdafx.h"
#include "util/utils.h"
#include <locale>

#include "stb_image.h"
#include "graphics/textureParameters.h"
#include "util/path.h"

namespace Diamond {
	const char* formatChar(const char* fmt, ...) {
		static char vaBuffer[0x200];
		va_list ap;
		va_start(ap, fmt);
		vsprintf_s(vaBuffer, fmt, ap);
		va_end(ap);
		return vaBuffer;
	}

	const char* formatBuffer(char* buffer, uint32_t size, const char* fmt, ...) {
		va_list ap;
		va_start(ap, fmt);
		vsprintf_s(buffer, size, fmt, ap);
		va_end(ap);
		return buffer;
	}


	std::string formatString(const char* fmt, ...) {
		char vaBuffer[0x200];
		va_list ap;
		va_start(ap, fmt);
		vsprintf_s(vaBuffer, fmt, ap);
		va_end(ap);
		return std::string(vaBuffer);
	}

	namespace StringUtils {
		bool endsWith(const std::string& str, const char* suffix, unsigned suffixLen) {
			return str.size() >= suffixLen && 0 == str.compare(str.size() - suffixLen, suffixLen, suffix, suffixLen);
		}

		bool endsWith(const std::string& str, const char* suffix) {
			return endsWith(str, suffix, (uint32_t)std::string::traits_type::length(suffix));
		}

		bool startsWith(const std::string& str, const char* prefix, unsigned prefixLen) {
			return str.size() >= prefixLen && 0 == str.compare(0, prefixLen, prefix, prefixLen);
		}

		bool startsWith(const std::string& str, const char* prefix) {
			return startsWith(str, prefix, (uint32_t)std::string::traits_type::length(prefix));
		}

		void lTrim(std::string& s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) {
				return !std::isspace(ch, std::locale::classic());
				}));
		}

		void rTrim(std::string& s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
				return !std::isspace(ch, std::locale::classic());
				}).base(), s.end());
		}

		void trim(std::string& s) {
			lTrim(s);
			rTrim(s);
		}

		std::vector<std::string> split(const std::string& s, const std::string& splitter) {
			size_t pos;
			std::vector<std::string> out;
			std::string ss = s;
			while ((pos = ss.find(splitter)) != std::string::npos) {
				std::string token = ss.substr(0, pos);
				out.push_back(token);
				ss.erase(0, pos + splitter.length());
			}
			out.push_back(ss);
			return out;
		}

		std::string replace(std::string subject, const std::string& search, const std::string& replace) {
			size_t pos = 0;
			while ((pos = subject.find(search, pos)) != std::string::npos) {
				subject.replace(pos, search.length(), replace);
				pos += replace.length();
			}
			return subject;
		}
	}
	namespace Utils {
		void doTimedFunction(int* timer, int timeMS, std::function<void()> function) {
			if (*timer < (int)GetTickCount()) {
				*timer = (int)GetTickCount() + timeMS;
				function();
			}
		}

		//void SetPositionInFrontOfCam(glm::vec3& dest, const Camera* cam, float distance) {
		//	const float x = cam->transform.position.x;
		//	const float y = cam->transform.position.y;
		//	const float z = cam->transform.position.z;
		//
		//	dest.x = x + Math::Sin(cam->transform.rotation.y) * distance * Math::Abs(Math::Cos(cam->transform.rotation.x));
		//	dest.y = y - Math::Sin(cam->transform.rotation.x) * distance;
		//	dest.z = z - Math::Cos(cam->transform.rotation.y) * distance * Math::Abs(Math::Cos(cam->transform.rotation.x));
		//}

		glm::vec3 rotationToDirection(glm::vec3 rotation) {
			glm::vec3 mirrorRot = rotation;
			mirrorRot.y = -mirrorRot.y;
			mirrorRot.x = -mirrorRot.x;
			return glm::quat(mirrorRot) * glm::vec3(0, 0, -1);
		}

		uint32_t getImageMemorySize(TextureParameters m_params, uint32_t width, uint32_t height) {
			return m_params.getChannelCount() * width * height;
		}

	}

	namespace GLUtils {
		const char* shaderTypeToString(int type, bool upperCase) {
			switch (type) {
				case GL_VERTEX_SHADER: return upperCase ? "Vertex" : "vertex";
				case GL_GEOMETRY_SHADER: return upperCase ? "Geometry" : "geometry";
				case GL_FRAGMENT_SHADER: return upperCase ? "Fragment" : "fragment";
				case GL_TESS_EVALUATION_SHADER: return upperCase ? "Tessellation evaluation" : "tessellation evaluation";
				case GL_TESS_CONTROL_SHADER: return upperCase ? "Tessellation control" : "tessellation control";
			}
			return "NULL";
		}

		const char* getFBOStatus(GLenum status) {
			switch (status) {
				case GL_FRAMEBUFFER_COMPLETE:						return "no error";
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			return "incomplete attachment";
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	return "missing Attachment";
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			return "draw Buffer";
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			return "read Buffer";
				case GL_FRAMEBUFFER_UNSUPPORTED:					return "snsupported Framebuffer configuration";
				//case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:			return "dimensions do not match";
				//case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:				return "formats error";
				default:												return "unkown Framebuffer Object error";
			}
		}

		const char* glErrorToString(int error) {
			switch (error) {
				case 0x500: return "GL_INVALID_ENUM";
				case 0x501: return "GL_INVALID_VALUE";
				case 0x502: return "GL_INVALID_OPERATION";
				case 0x503: return "GL_STACK_OVERFLOW";
				case 0x504: return "GL_STACK_UNDERFLOW";
				case 0x505: return "GL_OUT_OF_MEMORY";
				case 0x506: return "GL_INVALID_FRAMEBUFFER_OPERATION";
				case 0x507: return "GL_CONTEXT_LOST";
				case 0x5031: return "GL_TABLE_TOO_LARGE1";
			}
			return "";
		}
	}

	namespace TextureUtils {
		bool loadTexture(const Path& filePath, bool flip, std::function<void(const LoadedTexture& data)> callback, int goalChannels) {
			//if (!FileSystem::DoesFileExist(filePath.GetFullPath())) {
			//	LOG_WARN("[~gTexture~x] ~1%s.png~x at ~1%s~x does not exist!", filePath.GetFileName().c_str(), filePath.GetDirectory().c_str());
			//	return false;
			//}
			//
			//int channelCount;
			//int width, height;
			//
			//stbi_set_flip_vertically_on_load(flip);
			//byte* data = stbi_load(filePath.GetFullPath().c_str(), &width, &height, &channelCount, goalChannels);
			//
			////if (bpc != 3 && bpc != 4) {
			////	LOG_ERROR("[~gTexture~x] Unsupported image bit-depth (%d) ~1%s", bpc, path.c_str());
			////	stbi_image_free(data);
			////	return false;
			////}
			//
			////if (channelCount < 1 || channelCount > 4) LOG_ERROR("[~gTexture~x] Unsupported image bit-depth (%d) ~1%s", channelCount, filePath.GetFileName().c_str());
			//
			//int size = channelCount * width * height;
			//
			//if (data) {
			//	//LOG("[~gTexture~x] Loaded ~1%s", path.c_str());
			//	LoadedTexture textureData = { data, (uint)width, (uint)height, channelCount, size };
			//	callback(textureData);
			//	stbi_image_free(data);
			//	return true;
			//} //else LOG_ERROR("[~gTexture~x] Failed to load ~1%s", path.c_str());
			return false;
		}
	}
}