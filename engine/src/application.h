#pragma once
#include "util/singleton.h"

namespace Diamond {
	class Window;

	class Application : public Singleton<Application> {
	private:
		friend Singleton;

		Window* m_mainWindow;
	public:
		void initialize();
		void cleanup() {

		}
	};

	inline Application* GetApp() { return Application::getInstance(); }
}