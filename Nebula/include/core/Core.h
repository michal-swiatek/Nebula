//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_CORE_H
#define NEBULAENGINE_CORE_H

#if defined(_WIN32) || defined(WIN32)
    #ifdef NB_BUILD_DLL
        #define NEBULA_API __declspec(dllexport)
    #else
        #define NEBULA_API __declspec(dllimport)
    #endif
#else
    #error "Unsupported platform"
#endif

#endif //NEBULAENGINE_CORE_H
