#pragma once
#include "util/singleton.h"

namespace Diamond {
	struct Fiber {
		void* m_fiber;
		Fiber* m_nextFiber;
		float m_wakeTime;
		std::string m_name;
		std::thread::id m_glThreadID;
	};

	class GLFiberManager : public Singleton<GLFiberManager> {
	private:
		GLFiberManager() {}
		~GLFiberManager() {}
		friend Singleton;

		std::vector<Fiber> m_fibers;

		LPVOID m_mainFiber;
		Fiber* m_currentFiber;

		bool m_initialized = false;
		bool m_cleaned;
		std::thread::id m_glThreadID;

	public:
		void initialize();
		void tick();
		void addFiber(std::string name, void(*function)());
		void deleteFiber(const std::string& name);
		void sleep(float waitTime);
		void cleanup();

		const std::vector<Fiber>& getFibers() { return m_fibers; }
		std::string getCurrentFiberName() { return m_currentFiber->m_name; }
		void goToMainFiber() { SwitchToFiber(m_mainFiber); }

		bool isSameThread(std::thread::id threadId) {
			if (!m_initialized) return true;
			return(threadId == m_glThreadID);
		}
	};

	static GLFiberManager* GetGLFiberManager() { return GLFiberManager::getInstance(); }
}