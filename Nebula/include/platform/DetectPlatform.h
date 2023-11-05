//
// Created by michal-swiatek on 05.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_DETECTPLATFORM_H
#define NEBULAENGINE_DETECTPLATFORM_H

#if defined(_WIN32) || defined(WIN32)
    #ifdef _WIN64
        #define NB_PLATFORM_WINDOWS
    #else
        #error "x86 Windows builds are not supported!"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #define NB_PLATFORM_MACOS
    #error "MacOS is not supported!"
#elif defined(__ANDROID__)
    #define NB_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
    #define NB_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
    #error "Unknown platform!"
#endif

#ifndef NDEBUG
    #define NB_DEBUG_BUILD
#endif

#endif //NEBULAENGINE_DETECTPLATFORM_H
