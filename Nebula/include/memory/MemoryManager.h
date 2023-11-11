//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <vector>

#include "core/Core.h"
#include "MemoryChunk.h"

void initSubsystems();
void shutdownSubsystems();

namespace nebula::memory {

    class NEBULA_API MemoryManager
    {
    public:
        static void* requestMemory(std::size_t size);
        static void freeMemory(void* address);

    private:
        static std::vector<impl::MemoryChunk> s_memory_chunks;

        static void init();
        static void shutdown();

        friend void ::initSubsystems();
        friend void ::shutdownSubsystems();
    };

}

#endif //MEMORYMANAGER_H
