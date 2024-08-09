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

#define BIT(x) (1 << x)