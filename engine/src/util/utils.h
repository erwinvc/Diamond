#pragma once

#include "graphics\texture.h"

namespace Diamond {
	class Path;

	const char* formatChar(const char* fmt, ...);
	const char* formatBuffer(char* buffer, uint32_t size, const char* fmt, ...);
	std::string formatString(const char* fmt, ...);

	namespace StringUtils {
		//String
		bool endsWith(const std::string& str, const char* suffix, unsigned suffixLen);
		bool endsWith(const std::string& str, const char* suffix);
		bool startsWith(const std::string& str, const char* prefix, unsigned prefixLen);
		bool startsWith(const std::string& str, const char* prefix);

		void lTrim(std::string& s);
		void rTrim(std::string& s);
		void trim(std::string& s);

		std::vector<std::string> split(const std::string& s, const std::string& splitter);

		std::string replace(std::string subject, const std::string& search, const std::string& replace);
	}

	namespace Utils {
		static void nullfunc() {}

		void doTimedFunction(int* timer, int timeMS, std::function<void()> function);

		template <typename T>
		void removeFromVector(std::vector<T>& vec, const T& obj) {
			vec.erase(remove(vec.begin(), vec.end(), obj), vec.end());
		}

		template <typename T>
		bool vectorContains(std::vector<T>& vec, const T& obj) {
			return find(vec.begin(), vec.end(), obj) != vec.end();
		}

		//Math?
		//void SetPositionInFrontOfCam(glm::vec3& dest, const Camera* cam, float distance);
		glm::vec3 rotationToDirection(glm::vec3 rotation);
		
		uint32_t getImageMemorySize(TextureParameters m_params, uint32_t width, uint32_t height);
	}

	namespace GLUtils {
		const char* shaderTypeToString(int type, bool upperCase = false);
		const char* getFBOStatus(GLenum status);
		const char* glErrorToString(int error);
	}

	struct LoadedTexture {
		byte* m_data;
		uint32_t m_width;
		uint32_t m_height;
		int m_channelCount;
		int m_size;
	};

	namespace TextureUtils {
		bool loadTexture(const Path& filePath, bool flip, std::function<void(const LoadedTexture& data)> callback,
			int goalChannels = 0);
	}
}