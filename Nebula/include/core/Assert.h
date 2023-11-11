//
// Created by michal-swiatek on 05.11.2023.
// Github: https://github.com/michal-swiatek
//
// Taken from Cherno YouTube series
//

#ifndef NEBULAENGINE_ASSERT_H
#define NEBULAENGINE_ASSERT_H

#include "Core.h"
#include "Logging.h"

#ifdef NB_ENABLE_ASSERTS
    #define NB_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { NB##type##ERROR(msg, __VA_ARGS__); NB_DEBUGBREAK(); } }
    #define NB_INTERNAL_ASSERT_WITH_MSG(type, check, ...) NB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
    #define NB_INTERNAL_ASSERT_NO_MSG(type, check) NB_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", NB_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

    #define NB_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
    #define NB_INTERNAL_ASSERT_GET_MACRO(...) NB_EXPAND_MACRO( NB_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, NB_INTERNAL_ASSERT_WITH_MSG, NB_INTERNAL_ASSERT_NO_MSG) )

    #define NB_ASSERT(...) NB_EXPAND_MACRO( NB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
    #define NB_CORE_ASSERT(...) NB_EXPAND_MACRO( NB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
    #define NB_ASSERT(...)
	#define NB_CORE_ASSERT(...)
#endif

#endif //NEBULAENGINE_ASSERT_H
