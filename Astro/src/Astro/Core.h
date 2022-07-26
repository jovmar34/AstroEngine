#pragma once

#include <memory>

#ifdef AS_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	#ifdef AS_BUILD_DLL
		#define ASTRO_API __declspec(dllexport)
	#else
		#define ASTRO_API __declspec(dllimport)
	#endif
#else
	#define ASTRO_API 
#endif
#else 
	#error Only Windows so far
#endif // !AS_PLATFORM_WINDOWS

#ifdef AS_ENABLE_ASSERTS
	#define AS_ASSERT(x, ...) {if (!(x)) { HZ_ERROR("Assertion failed: {0]", __VA_ARGS__); __debugbreak(); } }
	#define AS_CORE_ASSERT(x, ...) {if (!(x)) { HZ_CORE_ERROR("Assertion failed: {0]", __VA_ARGS__); __debugbreak(); } }
#else
	#define AS_ASSERT(x, ...) 
	#define AS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define AS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Astro {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}