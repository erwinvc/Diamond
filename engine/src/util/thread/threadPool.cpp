#include "stdafx.h"
#include "threadPool.h"
#include "threadManager.h"
#include "util/utils.h"

namespace Diamond {
	void ThreadPool::initialize(int threads) {
		if (m_initialized) return;
		m_initialized = true;

		for (int i = 0; i < threads; ++i) {
			GetThreadManager()->registerThread(formatString("Job thread %d", i), [] {GetThreadPool()->threadEntry(); });
		}
		LOG("[~rThreads~x] Thread pool created");
	}

	void ThreadPool::doJob(std::function <void(void)> func) {
		if (!m_initialized) {
			LOG_ERROR("[~rThreads~x] Thread pool has not yet been initialized");
			return;
		}
		m_queue.add(func);
	}

	void ThreadPool::threadEntry() {
		static std::function <void(void)> job;

		if (m_queue.waitForGet(job))	job();
	}
}