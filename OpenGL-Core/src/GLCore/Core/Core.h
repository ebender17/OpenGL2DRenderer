#pragma once

#ifdef GLCORE_PLATFORM_WINDOWS
	#ifdef GLCORE_BUILD_DLL
		#define GLCORE_API __declspec(dllexport)
	#else
		#define GLCORE_API __declspec(dllimport)
	#endif
#else
	#error GLCORE only supports Windows!
#endif

#ifdef GLCORE_DEBUG
	#define GLCORE_ENABLE_ASSERTS
#endif

#ifdef GLCORE_ENABLE_ASSERTS
	#define GLCORE_ASSERT(x, ...) { if(!(x)) { GLCORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define GLCORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)