#include "stdafx.h"
#include "glFiberManager.h"

namespace Diamond {
	void GLFiberManager::initialize() {
		if (m_initialized) return;
		m_glThreadID = std::this_thread::get_id();
		ConvertThreadToFiber(nullptr);
		m_mainFiber = GetCurrentFiber();
		m_initialized = true;
		LOG("[~rThreads~x] GLFiber created");
	}

	void GLFiberManager::addFiber(std::string name, void(*function)()) {
		UINT64* args = new UINT64[2]{ (UINT64)function, (UINT64)m_mainFiber };
		LPVOID fiber = CreateFiber(NULL, [](LPVOID lpFiberParameter) {
			UINT64* arguments = (UINT64*)lpFiberParameter;
			//try {
			while (true) {
				((void(*)(void)) arguments[0])();
				SwitchToFiber((LPVOID)arguments[1]);
			}
			//} catch (...) {
			//	LOG_ERROR("[~rThreads~x] caught exception in fiber");
			delete[] arguments;
			//}
		}, args);

		Fiber newFiber = { fiber, nullptr, (float)glfwGetTime(), name };
		m_fibers.push_back(newFiber);

		for (auto& fiber = m_fibers.begin(); fiber != m_fibers.end(); ++fiber) {
			auto& nextFiber = fiber + 1;
			if (nextFiber != m_fibers.end()) fiber->m_nextFiber = &*nextFiber;
			else fiber->m_nextFiber = &m_fibers.front();
		}
		m_currentFiber = &m_fibers.front();
		LOG("[~rThreads~x] Created ~1%s~x fiber", newFiber.m_name.c_str());
	}

	void GLFiberManager::deleteFiber(const std::string& name) {
		for (Fiber& fiber : m_fibers) {
			if (fiber.m_name.compare(name) == 0) {
				DeleteFiber(fiber.m_fiber);
				LOG("[~rThreads~x] Deleted ~1%s~x fiber", fiber.m_name.c_str());
				break;
			}
		}
	}

	void GLFiberManager::tick() {
		if (!m_currentFiber || !m_initialized) return;
		for (Fiber& f : m_fibers) {
			if (glfwGetTime() >= f.m_wakeTime) {
				SwitchToFiber(f.m_fiber);
			}
		}
		m_currentFiber = m_currentFiber->m_nextFiber;
	}

	void GLFiberManager::sleep(float waitTime) {
		auto& fiber = find_if(m_fibers.begin(), m_fibers.end(), [](const Fiber& fiber) -> bool { return fiber.m_fiber == GetCurrentFiber(); });
		if (fiber != m_fibers.end()) {
			fiber->m_wakeTime = (float)glfwGetTime() + waitTime;
			SwitchToFiber(m_mainFiber);
		}
	}

	void GLFiberManager::cleanup() {
		if (!m_cleaned) {
			for (Fiber& fiber : m_fibers) {
				DeleteFiber(fiber.m_fiber);
				LOG("[~rThreads~x] Deleted ~1%s~x fiber", fiber.m_name.c_str());
			}
			//LOG_DEBUG("[Threads] %s deleted", m_name);
			m_cleaned = true;
		}
	}
}