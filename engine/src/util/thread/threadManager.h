#pragma once

namespace Diamond {
	class Thread {
	private:
		std::string m_name;
		HANDLE m_handle;
		std::function<void()> m_function;
		std::atomic<bool> m_shutDown = false;
		std::atomic<bool> m_finished = false;
		std::atomic<bool> m_background = false;

		friend class ThreadManager;

	public:
		Thread() : m_name(""), m_handle(INVALID_HANDLE_VALUE), m_background(false) {}
		Thread(const std::string& name, bool background) : m_name(name), m_handle(INVALID_HANDLE_VALUE), m_background(background) {}

		HANDLE getHandle() { return m_handle; };
		void shutdown() { m_shutDown = true; }
		bool isShuttingDown() { return m_shutDown; }
		bool isFinished() { return m_finished; }

		const std::string& getName() { return m_name; }
		void setName(std::string& name) { m_name = name; }
	};

	class ThreadManager {
	private:
		std::mutex m_lock;
		std::vector<Thread*> m_threads;

	public:
		Thread* registerThread2(std::string name, std::function<void()> func, bool forceKill = false);
		Thread* registerThread(std::string name, void(*func)(), bool forceKill = false);
		void cleanup();
	};

	ThreadManager* GetThreadManager();
}