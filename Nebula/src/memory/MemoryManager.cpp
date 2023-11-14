//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "memory/MemoryManager.h"

namespace nebula::memory {

    using namespace impl;

    std::mutex MemoryManager::s_mutex{};
    std::vector<std::unique_ptr<impl::MemoryChunk>> MemoryManager::s_memory_chunks{};

    void* MemoryManager::requestMemory(std::size_t size)
    {
        std::lock_guard<std::mutex> lock{s_mutex};
        s_memory_chunks.emplace_back(std::make_unique<MemoryChunk>(size));
        return s_memory_chunks.back()->getAddress();
    }

    void MemoryManager::freeMemory(void* address)
    {
        std::lock_guard<std::mutex> lock{s_mutex};
        const auto it = std::ranges::find_if(s_memory_chunks, [address](const auto& chunk){ return chunk->getAddress() == address; });
        if (it != s_memory_chunks.end())
        {
            NB_CORE_INFO("Freeing memory at address: {}", reinterpret_cast<std::uintptr_t>(address));
            s_memory_chunks.erase(it);
        }
        else
            NB_CORE_ASSERT(false, "Attempt to free memory that was not initialized by MemoryManager!");
    }

    void MemoryManager::init()
    {

    }

    void MemoryManager::shutdown()
    {
        //  Frees all remaining memory chunks
        s_memory_chunks.clear();
    }

}
