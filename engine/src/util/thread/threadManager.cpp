#include "stdafx.h"
#include "threadManager.h"
#include "threadPool.h"

namespace Diamond {
	ThreadManager g_threadManager;

	Thread* ThreadManager::registerThread2(std::string name, std::function<void()> func, bool background) {
		Thread* c_thread = new Thread(name, background);
		c_thread->m_function = func;
		uint64_t* args = new uint64_t[2]{ 0, (uint64_t)c_thread };
		c_thread->m_handle = CreateThread(0, 0, [](void* lpFiberParameter) -> DWORD {
			uint64_t* arguments = (uint64_t*)lpFiberParameter;
			Thread* thread = (Thread*)arguments[1];
			//try {
			while (!thread->m_shutDown) {
				thread->m_function;
			}
			LOG("[~rThreads~x] shut down ~1%s~x thread", thread->getName().c_str());
			thread->m_finished = true;

			//} catch (...) { LOG_ERROR("[~rThreads~x] caught exception in ~1%s ~rthread", thread->m_name.c_str()); }
			delete thread;
			delete[] arguments;
			return 1;
		}, args, 0, 0);

		std::unique_lock<std::mutex> lock(m_lock);
		m_threads.push_back(c_thread);

		LOG("[~rThreads~x] registered ~1%s ~xthread ", name.c_str());
		return c_thread;
	}

	Thread* ThreadManager::registerThread(std::string name, void(*func)(), bool background) {
		Thread* c_thread = new Thread(name, background);
		uint64_t* args = new uint64_t[3]{ 0, (uint64_t)c_thread, (uint64_t)func };
		c_thread->m_handle = CreateThread(0, 0, [](void* lpFiberParameter) -> DWORD {
			uint64_t* arguments = (uint64_t*)lpFiberParameter;
			Thread* thread = (Thread*)arguments[1];
			//try {
			while (!thread->m_shutDown) {
				((void(*)(void)) arguments[2])();
			}
			LOG("[~rThreads~x] shut down ~1%s~x thread", thread->getName().c_str());
			thread->m_finished = true;

			//} catch (...) { LOG_ERROR("[~rThreads~x] caught exception in ~1%s ~rthread", thread->m_name.c_str()); }
			delete thread;
			delete[] arguments;
			return 1;
		}, args, 0, 0);

		std::unique_lock<std::mutex> lock(m_lock);
		m_threads.push_back(c_thread);

		LOG("[~rThreads~x] registered ~1%s ~xthread ", name.c_str());
		return c_thread;
	}

	void ThreadManager::cleanup() {
		GetThreadPool()->shutdown();
		for (Thread* thread : m_threads) {
			thread->shutdown();
			if (thread->m_background) {
				TerminateThread(thread->m_handle, 0);
				thread->m_finished = true;
			}
		}
		bool finished = false;
		do {
			finished = true;
			for (auto& thread : m_threads) {
				if (!thread->isFinished()) finished = false;
			}
			Sleep(0);
		} while (!finished);
		LOG("[~rThreads~x] Thread pool terminated");
	}

	ThreadManager* GetThreadManager() { return &g_threadManager; }
}