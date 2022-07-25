#pragma once

#ifdef AS_PLATFORM_WINDOWS
	#ifdef AS_BUILD_DLL
		#define ASTRO_API __declspec(dllexport)
	#else
		#define ASTRO_API __declspec(dllimport)
	#endif
#else 
	#error Only Windows so far
#endif // !AS_PLATFORM_WINDOWS
