#pragma once

#ifdef D_DEBUG
#define ASSERT(x, ...) \
		if (!(x)) {\
			LOG("~r*************************"); \
			LOG("~r    ASSERTION FAILED!    "); \
			LOG("~r*************************"); \
			LOG("~r%s:%d", __FILE__, __LINE__); \
			LOG("~rCondition: %s", #x); \
			LOG_ERROR(__VA_ARGS__); \
			__debugbreak(); \
}
#else
#define ASSERT(x, ...) 
#endif