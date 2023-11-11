//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef ALLOCATORS_H
#define ALLOCATORS_H

#include <cstddef>

#include "core/Core.h"

namespace nebula::memory {

    namespace impl {

        class NEBULA_API Allocator
        {
        public:
            Allocator(const void* memory_chunk, std::size_t size) noexcept;
            virtual ~Allocator() noexcept;

            Allocator(const Allocator&) = delete;
            Allocator& operator = (Allocator&) = delete;
            Allocator(Allocator&&) noexcept;
            Allocator& operator = (Allocator&&) noexcept;

            virtual void* allocate(std::size_t size, std::uintptr_t alignment) = 0;
            virtual void deallocate(void* address) = 0;

            [[nodiscard]] std::size_t getSize() const { return m_size; }

            #ifdef NB_DEBUG_BUILD
            [[nodiscard]] std::size_t getUsedMemory() const { return m_used; }
            [[nodiscard]] std::size_t getNumAllocations() const { return m_num_allocations; }
            #endif

        protected:
            const void* m_chunk;
            std::size_t m_size;

            #ifdef NB_DEBUG_BUILD
            std::size_t m_used = 0;
            std::size_t m_num_allocations = 0;
            #endif
        };

    }

    class NEBULA_API LinearAllocator final : public impl::Allocator
    {
    public:
        LinearAllocator(const void* memory_chunk, std::size_t size) noexcept;
        ~LinearAllocator() override;

        LinearAllocator(const LinearAllocator&) = delete;
        LinearAllocator& operator = (LinearAllocator&) = delete;
        LinearAllocator(LinearAllocator&&) noexcept;
        LinearAllocator& operator = (LinearAllocator&&) noexcept;

        void* allocate(std::size_t size, std::uintptr_t alignment) override;
        void deallocate(void* address) override;

        void clear();

    private:
        std::uintptr_t m_offset = 0;

        template <typename Allocator, std::size_t Size>
        friend class ScopedAllocator;
    };

    class NEBULA_API StackAllocator final : public impl::Allocator
    {
    public:
        StackAllocator(const void* memory_chunk, std::size_t size) noexcept;

        StackAllocator(const StackAllocator&) = delete;
        StackAllocator& operator = (StackAllocator&) = delete;
        StackAllocator(StackAllocator&&) noexcept;
        StackAllocator& operator = (StackAllocator&&) noexcept;

        void* allocate(std::size_t size, std::uintptr_t alignment) override;
        void deallocate(void* address) override;

    private:
        void* m_current_address;
        #ifdef NB_DEBUG_BUILD
        void* m_previous_address = nullptr;
        #endif

        struct Header
        {
            std::size_t adjustment;
            #ifdef NB_DEBUG_BUILD
            void* previous_address;
            #endif
        };

        template <typename Allocator, std::size_t Size>
        friend class ScopedAllocator;
    };

}

#endif //ALLOCATORS_H
