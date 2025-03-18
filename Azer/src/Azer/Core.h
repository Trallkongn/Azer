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

#define BIT(x) (1 << x)