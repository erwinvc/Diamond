#pragma once
#include "util/singleton.h"
#include "src/assets/assetManager.h"
#include "ref.h"
#include "graphics/window.h"

namespace Diamond {


	class Application : public Singleton<Application> {
	private:
		friend Singleton;

		Window* m_mainWindow;

	public:
		void initialize();
		void cleanup() {

		}

		void onResize(uint32_t width, uint32_t height);

		uint32_t getWidth() const { return m_mainWindow->getWidth(); }
		uint32_t getHeight()const { return m_mainWindow->getHeight(); }
	};

	inline Application* getApp() { return Application::getInstance(); }
}