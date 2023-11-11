//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef TYPES_H
#define TYPES_H

#include <memory>

#include "MemoryManager.h"

namespace nebula {

    namespace impl {

        template <typename T>
        struct Deleter
        {
            void operator () (T* address)
            {
                address->~T();
                memory::MemoryManager::freeMemory(address);
            }
        };

    }

    constexpr std::size_t operator ""_Kb (std::size_t size) { return size * 1024; }
    constexpr std::size_t operator ""_Mb (std::size_t size) { return size * 1024_Kb; }
    constexpr std::size_t operator ""_Gb (std::size_t size) { return size * 1024_Mb; }
    constexpr std::size_t operator ""_Tb (std::size_t size) { return size * 1024_Gb; }

    constexpr std::size_t operator ""_KB (std::size_t size) { return size * 1024 * 8; }
    constexpr std::size_t operator ""_MB (std::size_t size) { return size * 1024_KB; }
    constexpr std::size_t operator ""_GB (std::size_t size) { return size * 1024_MB; }
    constexpr std::size_t operator ""_TB (std::size_t size) { return size * 1024_GB; }

    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T>
    using Reference = std::shared_ptr<T>;

    template <typename T>
    using View = T*;

    template <typename T, typename... Args>
    constexpr Scope<T> createScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    constexpr Reference<T> createReference(Args&&... args)
    {
        return std::shared_ptr<T>(
            new (memory::MemoryManager::requestMemory(sizeof(T))) T(std::forward<Args>(args)...),
            impl::Deleter<T>()
        );
    }

}

#endif //TYPES_H
