#pragma once
#include "util/singleton.h"
#include "util/asyncQueue.h"

namespace Diamond {
	class ThreadPool : public Singleton<ThreadPool> {
	private:
		bool m_initialized = false;
		AsyncQueue<std::function<void(void)>> m_queue;

		void threadEntry();

	public:
		void initialize(int threads);
		void doJob(std::function <void(void)> func);
		bool isInitialized() { return m_initialized; }
		void shutdown() { m_queue.releaseWaitingThreads(); }
	};

	static ThreadPool* GetThreadPool() { return ThreadPool::getInstance(); }
}