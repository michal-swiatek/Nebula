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
