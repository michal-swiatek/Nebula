//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "memory/MemoryManager.h"

namespace nebula::memory {

    std::vector<impl::MemoryChunk> MemoryManager::s_memory_chunks{};

    void* MemoryManager::requestMemory(std::size_t size)
    {
        s_memory_chunks.emplace_back(size);
        return s_memory_chunks.back().getAddress();
    }

    void MemoryManager::freeMemory(void* address)
    {
        if (
            const auto it = std::ranges::find_if(s_memory_chunks, [address](const auto& chunk){ return chunk.getAddress() == address; });
            it != s_memory_chunks.end()
        )
            s_memory_chunks.erase(it);
    }

    void MemoryManager::init(std::size_t block_size)
    {
        NB_CORE_INFO("Initializing Engine with memory block of size: {} bytes", block_size);
    }

    void MemoryManager::shutdown()
    {
        //  Frees all remaining memory chunks
        s_memory_chunks.clear();
    }

}
