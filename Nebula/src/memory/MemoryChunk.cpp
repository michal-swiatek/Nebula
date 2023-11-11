//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "memory/MemoryChunk.h"

#include <cstdlib>

#include "core/Assert.h"

namespace nebula::memory::impl {

    MemoryChunk::MemoryChunk(const std::size_t size) : m_size(size)
    {
        m_chunk = std::malloc(size);
        NB_CORE_ASSERT(m_chunk, std::format("Cannot allocate memory chunk of size: {}", size));
    }

    MemoryChunk::~MemoryChunk()
    {
        std::free(m_chunk);
    }

    MemoryChunk::MemoryChunk(MemoryChunk&& rhs) noexcept : m_size(rhs.m_size)
    {
        m_chunk = rhs.m_chunk;
        rhs.m_chunk = nullptr;
        rhs.m_size = 0;
    }

    MemoryChunk& MemoryChunk::operator=(MemoryChunk&& rhs) noexcept
    {
        m_chunk = rhs.m_chunk;
        m_size = rhs.m_size;

        rhs.m_chunk = nullptr;
        rhs.m_size = 0;

        return *this;
    }

}
