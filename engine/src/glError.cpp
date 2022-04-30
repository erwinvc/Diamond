#include "stdafx.h"
#include "GLError.h"
#include "util/utils.h"

namespace Diamond {
	void glErrorCallback(int error, const char* description) {
		LOG_ERROR("[GLFW] %s", description);
	}

	void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		LOG("%d %d %s", type, id, message);
	}

	bool glLogCall(const char* function, const char* file, int line) {
		while (GLenum error = glGetError()) {
			std::string fileName = file;
			size_t position = fileName.find_last_of('\\');
			fileName = fileName.substr(position);
			LOG_ERROR("[GLError] %s caused error %s in %s at line %d", function, GLUtils::glErrorToString(error), fileName.c_str(), line);
			return false;
		}
		return true;
	}
}