//
// Created by michal-swiatek on 10.06.2022.
// Github: https://github.com/michal-swiatek/
//

#ifndef NEBULA_NEBULAAPI_H
#define NEBULA_NEBULAAPI_H

#if defined(_WIN32) || defined(WIN32)
    #ifdef BUILD_NEBULA_DLL
        #define NEBULA_API __declspec(dllexport)
    #else
        #define NEBULA_API __declspec(dllimport)
    #endif
#else
    #error "Only Windows platform is supported so far"
#endif

#endif //NEBULA_NEBULAAPI_H
