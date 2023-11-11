//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "memory/Allocators.h"

namespace nebula::memory {

    //  Helpers
    inline bool checkAddress(const void* current, const void* max)
    {
        return reinterpret_cast<std::uintptr_t>(current) < reinterpret_cast<std::uintptr_t>(max);
    }

    inline void* shiftPointer(const void* ptr, const std::uintptr_t amount, bool forward = true)
    {
        if (forward)
            return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(ptr) + amount);

        return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(ptr) - amount);
    }

    inline std::size_t alignForwardAdjustment(const void* ptr, const std::size_t alignment)
    {
        const auto address = reinterpret_cast<std::uintptr_t>(ptr);
        const auto aligned = (address - 1u + alignment) & -alignment;
        return aligned - address;
    }

    inline std::size_t alignForwardAdjustmentWithHeader(const void* ptr, const std::size_t alignment, const std::size_t header_size)
    {
        auto adjustment = alignForwardAdjustment(ptr, alignment);

        if (adjustment < header_size)
            adjustment += alignment * ((header_size - adjustment - 1) / alignment + 1);

        return adjustment;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    ////    Allocator    ////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

    namespace impl {

        Allocator::Allocator(const void* memory_chunk, std::size_t size) noexcept : m_chunk(memory_chunk), m_size(size)
        {
            NB_CORE_ASSERT(memory_chunk && size > 0, "Invalid memory allocator initialization!");
        }

        Allocator::~Allocator() noexcept
        {
            NB_CORE_ASSERT(m_used == 0 && m_num_allocations == 0, "Allocator hasn't deallocated all of it's memory!");
        }

        Allocator::Allocator(Allocator&& rhs) noexcept : m_chunk(rhs.m_chunk), m_size(rhs.m_size)
        {
            #ifdef NB_DEBUG_BUILD
            m_used = rhs.m_used;
            m_num_allocations = rhs.m_num_allocations;

            rhs.m_chunk = nullptr;
            rhs.m_size = 0;
            rhs.m_used = 0;
            rhs.m_num_allocations = 0;
            #endif
        }

        Allocator& Allocator::operator=(Allocator&& rhs) noexcept
        {
            m_chunk = rhs.m_chunk;
            m_size = rhs.m_size;

            #ifdef NB_DEBUG_BUILD
            m_used = rhs.m_used;
            m_num_allocations = rhs.m_num_allocations;

            rhs.m_chunk = nullptr;
            rhs.m_size = 0;
            rhs.m_used = 0;
            rhs.m_num_allocations = 0;
            #endif

            return *this;
        }

    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    ////    LinearAllocator    //////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

    LinearAllocator::LinearAllocator(const void* memory_chunk, std::size_t size) noexcept : Allocator(memory_chunk, size) {}

    LinearAllocator::~LinearAllocator()
    {
        clear();
    }

    LinearAllocator::LinearAllocator(LinearAllocator&& rhs) noexcept : Allocator(std::move(rhs)), m_offset(rhs.m_offset)
    {
        rhs.m_offset = 0;
    }

    LinearAllocator& LinearAllocator::operator=(LinearAllocator&& rhs) noexcept
    {
        m_offset = rhs.m_offset;
        Allocator::operator=(std::move(rhs));
        rhs.m_offset = 0;
        return *this;
    }

    void* LinearAllocator::allocate(std::size_t size, std::uintptr_t alignment)
    {
        NB_CORE_ASSERT(size > 0 && alignment > 0, "Invalid allocation request!");

        void* current_position = shiftPointer(m_chunk, m_offset);
        auto adjustment = alignForwardAdjustment(current_position, alignment);
        auto new_offset = m_offset + adjustment + size;

        if (new_offset > m_size)
            throw std::bad_alloc();

        m_offset = new_offset;
        void* aligned_address = shiftPointer(current_position, adjustment);

        #ifdef NB_DEBUG_BUILD
        m_used += size + adjustment;
        ++m_num_allocations;
        #endif

        return aligned_address;
    }

    void LinearAllocator::deallocate(void* address)
    {
        //  LinearAllocator deallocates memory through clear() method
    }

    void LinearAllocator::clear()
    {
        m_offset = 0;

        #ifdef NB_DEBUG_BUILD
        m_used = 0;
        m_num_allocations = 0;
        #endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    ////    StackAllocator    ///////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

    StackAllocator::StackAllocator(
        const void* memory_chunk,
        std::size_t size
    ) noexcept :
            Allocator(memory_chunk, size),
            m_current_address(const_cast<void*>(memory_chunk))
    {}

    StackAllocator::StackAllocator(StackAllocator&& rhs) noexcept : Allocator(std::move(rhs)), m_current_address(rhs.m_current_address)
    {
        rhs.m_current_address = nullptr;

        #ifdef NB_DEBUG_BUILD
        m_previous_address = rhs.m_previous_address;
        rhs.m_previous_address = nullptr;
        #endif
    }

    StackAllocator& StackAllocator::operator=(StackAllocator&& rhs) noexcept
    {
        Allocator::operator=(std::move(rhs));

        m_current_address = rhs.m_current_address;
        rhs.m_current_address = nullptr;

        #ifdef NB_DEBUG_BUILD
        m_previous_address = rhs.m_previous_address;
        rhs.m_previous_address = nullptr;
        #endif

        return *this;
    }

    void* StackAllocator::allocate(std::size_t size, std::uintptr_t alignment)
    {
        NB_CORE_ASSERT(size > 0 && alignment > 0, "Invalid allocation request!");

        auto adjustment = alignForwardAdjustmentWithHeader(m_current_address, alignment, sizeof(Header));
        void* new_address = shiftPointer(m_current_address, adjustment + size);
        if (!checkAddress(new_address, shiftPointer(m_chunk, m_size)))
            throw std::bad_alloc();

        void* aligned_address = shiftPointer(m_current_address, adjustment);
        m_current_address = new_address;

        auto* header = reinterpret_cast<Header*>(reinterpret_cast<std::uintptr_t>(aligned_address) - sizeof(Header));
        header->adjustment = adjustment;

        #ifdef NB_DEBUG_BUILD
        m_used += size + adjustment;
        ++m_num_allocations;

        header->previous_address = m_previous_address;
        m_previous_address = aligned_address;
        #endif

        return aligned_address;
    }

    void StackAllocator::deallocate(void* address)
    {
        NB_CORE_ASSERT(address == m_previous_address, "Stack allocator deallocates memory in reverse order!");

        auto* header = reinterpret_cast<Header*>(reinterpret_cast<std::uintptr_t>(address) - sizeof(Header));

        #ifdef NB_DEBUG_BUILD
        m_used -= (reinterpret_cast<std::uintptr_t>(m_current_address) - reinterpret_cast<std::uintptr_t>(address)) + header->adjustment;
        --m_num_allocations;

        m_previous_address = header->previous_address;
        #endif

        m_current_address = shiftPointer(address, header->adjustment, false);
    }

}
