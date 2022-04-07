#pragma once
#include <math.h>
#include "random.h"

namespace Math {
	static const float PI = 3.1415927f;
	static const float TWO_PI = 6.2831854f;
	static const float HALF_PI = 1.5707964f;
	static const float THIRD_PI = 1.0471976f;
	static const float QUARTER_PI = 0.7853982f;

	static const float MIN_FLOAT = FLT_MIN;
	static const float MAX_FLOAT = FLT_MAX;
	static const int MIN_INT = INT_MIN;
	static const int MAX_INT = INT_MAX;
	inline float toRadians(float degrees) {
		return (float)(degrees * (3.14 / 180.0f));
	}

	inline float toDegrees(float radians) {
		return (float)(radians * (180.0f / 3.14f));
	}

	inline int abs(int val) {
		return val <= 0 ? 1 - val : val;
	}

	inline float abs(float val) {
		return val <= 0.0f ? 0.0f - val : val;
	}

	inline float sin(float angle) {
		return (float)::sin(angle);
	}

	inline float cos(float angle) {
		return (float)::cos(angle);
	}

	inline float tan(float angle) {
		return (float)::tan(angle);
	}

	inline float sqrt(float value) {
		return (float)::sqrt(value);
	}

	inline float rsqrt(float value) {
		return 1.0f / (float)::sqrt(value);
	}

	inline float pow(float value, float pow) {
		return (float)::pow(value, pow);
	}

	inline float log(float value) {
		return (float)::log(value);
	}

	inline float log2(float value) {
		return (float)::log(value);
	}

	inline float asin(float value) {
		return (float)::asin(value);
	}

	inline float acos(float value) {
		return (float)::acos(value);
	}

	inline float atan(float value) {
		return (float)::atan(value);
	}

	inline float atan2(float y, float x) {
		return (float)::atan2(y, x);
	}

	inline uint32_t min(uint32_t value, uint32_t value2) {
		return (value < value2) ? value : value2;
	}

	inline float min(float value, float value2) {
		return (value < value2) ? value : value2;
	}

	inline uint32_t max(uint32_t value, uint32_t value2) {
		return (value > value2) ? value : value2;
	}

	inline float max(float value, float value2) {
		return (value > value2) ? value : value2;
	}

	inline uint32_t floor(float val) {
		return (uint32_t)::floor(val);
	}

	inline float sign(float value, float amount = 1.0f) {
		if (value == 0) return 0;
		return value < 0 ? -amount : amount;
	}

	inline float mod(float value1, float value2) {
		return std::fmod(value1, value2);
	}
	//template <typename T>
	//int sign(const T& a) {
	//	return (T{ 0 } < a) - (a < T{ 0 });
	//}

	template<typename T>
	inline T clamp(T val, T min, T max) {
		return val < min ? min : val > max ? max : val;
	}

	template<typename T>
	inline void clamp(T* val, T min, T max) {
		*val = *val < min ? min : *val > max ? max : *val;
	}

	template<typename T>
	inline bool within(T val, T min, T max) {
		return val <= max && val >= min;
	}

	template<typename T>
	inline bool within(T valMin, T valMax, T min, T max) {
		return valMin < max&& valMax > min;
	}

	inline glm::vec3 pointOnUnitSphere(float u, float v) {
		float theta = u * 2.0f * Math::PI;
		float phi = acos(2.0f * v - 1.0f);
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);
		float sinPhi = sin(phi);
		float cosPhi = cos(phi);
		float x = sinPhi * cosTheta;
		float y = sinPhi * sinTheta;
		float z = cosPhi;
		return { x, y, z };
	}

	inline glm::vec3 randomOnUnitSphere() {
		float u = Random::nextFloat(0.0f, 1.0f);
		float v = Random::nextFloat(0.0f, 1.0f);
		return pointOnUnitSphere(u, v);
	}


	inline float lerp(float toEase, float easeFrom, float percent) {
		return (toEase + percent * (easeFrom - toEase));
	}

	inline float ease(float value, float easeTo, float multiplier) {
		return value + (value < easeTo ? abs(value - easeTo) / multiplier : -abs(value - easeTo) / multiplier);
	}

	inline uint32_t round(float val) {
		return (val >= 0) ? (uint32_t)(val + 0.5f) : (uint32_t)(val - 0.5f);
	}

	inline float map(float value, float minFrom, float maxFrom, float minTo, float maxTo) {
		return minTo + (maxTo - minTo) * ((value - minFrom) / (maxFrom - minFrom));
	}
}
