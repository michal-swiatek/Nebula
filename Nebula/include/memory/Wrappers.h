//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef WRAPPERS_H
#define WRAPPERS_H

#include "MemoryChunk.h"

namespace nebula::memory {

    template <typename Allocator, std::size_t MemoryChunkSize>
    class NEBULA_API ScopedAllocator
    {
    public:
        ScopedAllocator() : m_allocator(m_memory_chunk.getAddress(), m_memory_chunk.getSize()) {}
        ~ScopedAllocator()
        {
            #ifdef NB_DEBUG_BUILD
            m_allocator.m_used = 0;
            m_allocator.m_num_allocations = 0;
            #endif
        }

        [[nodiscard]] void* allocate(const std::size_t size, const std::uintptr_t alignment = sizeof(std::intptr_t))
        {
            return m_allocator.allocate(size, alignment);
        }

        template <typename T, typename... Args>
        [[nodiscard]] T* create(Args&&... args)
        {
            return new (allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
        }

        template <typename T>
        void destroy(T* object)
        {
            object->~T();
        }

    private:
        impl::ScopedMemoryChunk<MemoryChunkSize> m_memory_chunk{};
        Allocator m_allocator;
    };

    template <typename T, typename Allocator>
    class NEBULA_API STLAdapter
    {
    public:
        using value_type = T;

        STLAdapter() = delete;
        STLAdapter(Allocator& allocator) noexcept : m_allocator(allocator) {}

        template <typename U>
        STLAdapter(const STLAdapter<U, Allocator>& rhs) noexcept : m_allocator(rhs.m_allocator) {}

        [[nodiscard]] constexpr T* allocate(std::size_t n)
        {
            return reinterpret_cast<T*>(m_allocator.allocate(n * sizeof(T), alignof(T)));
        }

        constexpr void deallocate(T* p, [[maybe_unused]] std::size_t n)
        {
            m_allocator.deallocate(p);
        }

        bool operator == (const STLAdapter& rhs) const noexcept
        {
            return m_allocator.getAddress() == rhs.m_allocator.getAddress();
        }

        bool operator!=(const STLAdapter& rhs) const noexcept
        {
            return !(*this == rhs);
        }

    private:
        Allocator& m_allocator;
    };

}

#endif //WRAPPERS_H
