#pragma once

namespace Diamond {
	template<typename T>
	class AsyncQueue {
	private:
		std::mutex m_lock;
		std::condition_variable m_conditionVariable;
		std::queue <T> m_queue;
		bool m_releaseThreads = false;

	public:
		void add(T obj) {
			std::unique_lock<std::mutex> l(m_lock);
			m_queue.emplace(std::move(obj));
			m_conditionVariable.notify_one();
		}

		bool tryToGet(T& obj) {
			std::unique_lock<std::mutex> lock(m_lock);
			if (m_queue.empty()) return false;
			obj = move(m_queue.front());
			m_queue.pop();
			return true;
		}

		bool peek(T& obj) {
			std::unique_lock<std::mutex> lock(m_lock);
			if (m_queue.empty()) return false;
			obj = move(m_queue.front());
			return true;
		}

		bool waitForGet(T& obj) {
			std::unique_lock<std::mutex> lock(m_lock);
			while (!m_releaseThreads && m_queue.empty()) m_conditionVariable.wait(lock);
			if (m_queue.empty()) return false;
			obj = move(m_queue.front());
			m_queue.pop();
			return true;
		}

		void releaseWaitingThreads() {
			m_releaseThreads = true;
			m_conditionVariable.notify_all();
		}

		int size() {
			std::unique_lock<std::mutex> lock(m_lock);
			return m_queue.size();
		}
	};
}