#pragma once
#include "glWindowCallbacks.h"
#include "util/color.h"

namespace Diamond {
	class Window {
	private:
		GLFWwindow* m_window;
		std::string m_title;
		uint32_t m_width, m_height;
		glm::ivec2 m_resizeBuffer = glm::ivec2(-1, -1);
		GLWindowCallbacks callbacks;

		bool m_vSync;
		bool m_isFocussed;
		bool m_isMinimized = false;

		inline void onFocusEvent(int focus) { m_isFocussed = focus; }

	public:
		Window(std::string title, int width, int height);
		~Window() { glfwDestroyWindow(m_window); }

		GLWindowCallbacks& getCallbacks() { return callbacks; }
		void setWidth(uint32_t width) { m_width = width; }
		void setHeight(uint32_t height) { m_height = height; }
		uint32_t getWidth() const { return m_width; }
		uint32_t getHeight() const { return m_height; }
		float getAspect() const { return (float)(m_width) / m_height; }
		bool getFocussed() { return m_isFocussed; }

		bool checkResized();
		void setVSync(bool toggle);
		void setTitle(const std::string& title) { glfwSetWindowTitle(m_window, title.c_str()); }

		inline void swapBuffers() { glfwSwapBuffers(m_window); }
		inline void pollEvents() { glfwPollEvents(); }
		inline bool shouldClose() { return glfwWindowShouldClose(m_window); }
		inline void makeContextCurrent() { glfwMakeContextCurrent(m_window); }
		inline void show() { glfwShowWindow(m_window); }

		inline void clearColor(Color& col) { glClearColor(col.r, col.g, col.b, col.a); }
		inline void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

		inline bool getVSync() { return m_vSync; }
		inline bool isMinimized() { return m_isMinimized; }
		//inline void setIcon(const Icon& icon) { GL(glfwSetWindowIcon(GetHandle(), 1, icon.GetImage())); }
		inline void onResize(int width, int height) { m_resizeBuffer = glm::ivec2(width, height); }

		inline GLFWwindow* getHandle() { return m_window; }
	};
}