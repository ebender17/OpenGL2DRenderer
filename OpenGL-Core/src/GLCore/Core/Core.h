#pragma once

#include <memory>

#ifdef GLCORE_DEBUG
    #if defined(GLCORE_PLATFORM_WINDOWS)
        #define GLCORE_DEBUGBREAK() __debugbreak()
    #elif defined(GLCORE_PLATFORM_LINUX)
        #include <signal.h>
        #define GLCORE_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define GLCORE_ENABLE_ASSERTS
#else
    #define GL_DEBUGBREAK()
#endif

#ifdef GLCORE_ENABLE_ASSERTS
	#define GLCORE_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); GLCORE_DEBUGBREAK(); } }
#else
	#define GLCORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define GLCORE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace GLCore {

    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}