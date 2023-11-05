//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_CORE_H
#define NEBULAENGINE_CORE_H

#include "platform/DetectPlatform.h"

#ifdef NB_PLATFORM_WINDOWS
    #ifdef NB_BUILD_DLL
        #define NEBULA_API __declspec(dllexport)
    #else
        #define NEBULA_API __declspec(dllimport)
    #endif
#else
    #error "Unsupported platform!"
#endif

#ifdef NB_DEBUG_BUILD
    #if defined(NB_PLATFORM_WINDOWS)
        #define NB_DEBUGBREAK() __debugbreak()
    #elif defined(NB_PLATFORM_LINUX)
        #include <signal.h>
        #define NB_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define NB_ENABLE_ASSERTS
#else
    #define NB_DEBUGBREAK()
#endif

#define BIT(x) (1 << x)

#define NB_EXPAND_MACRO(x) x
#define NB_STRINGIFY_MACRO(x) #x

#define NB_BIND_EVENT_FUNCTION(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#endif //NEBULAENGINE_CORE_H
