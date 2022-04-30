#pragma once

namespace Diamond {
	void glErrorCallback(int error, const char* description);
	void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	bool glLogCall(const char* function, const char* file, int line);

	//if(!GetGLFiberManager()->IsSameThread(this_thread::get_id())) LOG_ERROR("AAAAAA");

#ifdef D_DEBUG
#define GL(x) glGetError();\
		x; \
		if (!glLogCall(#x, __FILE__, __LINE__)) __debugbreak();
#else
#define GL(x) x
#endif
}