#pragma once
#include "random.h"

namespace Diamond {
	namespace Math {
		const float PI = 3.1415927f;
		const float TWO_PI = 6.2831854f;
		const float HALF_PI = 1.5707964f;
		const float THIRD_PI = 1.0471976f;
		const float QUARTER_PI = 0.7853982f;
		
		const float MIN_FLOAT = FLT_MIN;
		const float MAX_FLOAT = FLT_MAX;
		const int MIN_INT = INT_MIN;
		const int MAX_INT = INT_MAX;

		inline float toRadians(float degrees) {
			return (float)(degrees * (3.14 / 180.0f));
		}

		inline float toDegrees(float radians) {
			return (float)(radians * (180.0f / 3.14f));
		}

		inline float sign(float value, float amount = 1.0f) {
			if (value == 0) return 0;
			return value < 0 ? -amount : amount;
		}

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
			float theta = u * 2.0f * PI;
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
}