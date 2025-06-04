#pragma once

#ifdef AZ_PLATFORM_WINDOWS
#ifdef AZ_BUILD_DLL
#define AZER_API __declspec(dllexport)
#else
#define AZER_API __declspec(dllimport)
#endif // AZ_BUILD_DLL
#else
#error Azer only support Windows!
#endif

#ifdef AZ_ENABLE_ASSERTS
	#define AZ_ASSERT(X, ...) { if(!(x){ AZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define AZ_CORE_ASSERT(x, ...) { if(!(x)){ AZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define AZ_ASSERT(x, ...)
	#define AZ_CORE_ASSERT(x, ...)
#endif // AZ_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)