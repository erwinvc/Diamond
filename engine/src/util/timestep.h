#pragma once

namespace Diamond {
	class TimeStep {
	private:
		float m_time;
		float m_deltaTime;
		float m_totalTime;
		uint64_t m_frameCount;
	public:

		//https://gyazo.com/3beb3d4d8b88517ebd327028747be10c
		TimeStep(float initial = 0.0, float totalTime = 0.0f, uint64_t frameCount = 0)
			: m_time(initial), m_deltaTime(initial / 1000.0f), m_totalTime(totalTime), m_frameCount(frameCount) {
		}

		//operator float() const { return m_deltaTime; }

		//inline float DeltaTime() const { return m_deltaTime; }
		//inline float DeltaTimeMS() const { return m_time; }

		inline float totalTime() const { return m_totalTime; }
		inline uint64_t frameCount() const { return m_frameCount; }
	};
}