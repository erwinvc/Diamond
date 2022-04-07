#pragma once
#include <ctime>
#include <random>

class RandomInstance {
private:
	std::mt19937 m_random;
	std::uniform_int_distribution<std::mt19937::result_type> m_distribution;

public:
	RandomInstance() { m_random.seed(std::random_device()()); }
	RandomInstance(unsigned int seed) { m_random.seed(seed); }

	inline float nextFloat(float min, float max) {
		float r = (float)m_distribution(m_random) / (float)std::numeric_limits<uint32_t>::max();
		return r * (max - min) + min;
	}

	inline float nextFloat(float max) {
		float r = (float)m_distribution(m_random) / (float)std::numeric_limits<uint32_t>::max();
		return r * max;
	}

	/// <summary> Generates a random integer </summary>
	/// <param name="min"> Inclusive minimum </param>
	/// <param name="max"> Inclusive maxmimum </param>
	inline int nextInt(int min, int max) {
		return min + rand() % ((max + 1) - min);
	}

	inline int nextInt(int max) {
		return rand() % (max + 1);
	}
};

/// Globally available RandomInstance with wrapper functions
class Random {
private:
	static RandomInstance s_instance;

public:
	static void initialize() {
		srand((uint32_t)time(NULL));
		s_instance = RandomInstance((uint32_t)time(nullptr));
	}
	static float nextFloat(float min, float max) {
		return s_instance.nextFloat(min, max);
	}

	static float nextFloat(float max) {
		return s_instance.nextFloat(max);
	}

	/// <summary> Generates a random integer </summary>
	/// <param name="min"> Inclusive minimum </param>
	/// <param name="max"> Inclusive maxmimum </param>
	static int nextInt(int min, int max) {
		return s_instance.nextInt(min, max);
	}

	static int nextInt(int max) {
		return s_instance.nextInt(max);
	}
};