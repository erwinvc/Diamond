#include "stdafx.h"
#include "application.h"
#include "graphics/window.h"
#include "math/random.h"

namespace Diamond {
	void Application::initialize() {
		Random::initialize();
		Logger::Initialize("Diamond");

		if (!glfwInit()) {
			LOG_ERROR("[GLFW] GLFW failed to initialize");
			return;
		}

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		//glfwWindowHint(GLFW_SAMPLES, 8);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);

#ifdef D_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif // D_DEBUG


		m_mainWindow = new Window("Diamond", 1920, 1080);
		m_mainWindow->makeContextCurrent();
		m_mainWindow->clearColor(Color(0.5f, 0.7f, 1.0f, 1.0f));
		m_mainWindow->setVSync(true);

		if (glewInit() != GLEW_OK) {
			LOG_ERROR("[GLEW] failed to initialize");
			return;
		}

		LOG("[~cGPU~x] %-26s %s", "GPU~1", glGetString(GL_RENDERER));
		LOG("[~cGPU~x] %-26s %s", "OpenGL version~1", glGetString(GL_VERSION));

		m_mainWindow->show();
		while (!m_mainWindow->shouldClose()) {
			m_mainWindow->clear();
			m_mainWindow->swapBuffers();
			m_mainWindow->pollEvents();
		}

		glfwTerminate();
	}
}