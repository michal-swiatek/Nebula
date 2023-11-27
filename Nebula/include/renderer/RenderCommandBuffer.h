//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERCOMMANDBUFFER_H
#define RENDERCOMMANDBUFFER_H

#include <vector>

#include "memory/Allocators.h"
#include "memory/MemoryManager.h"

#include "renderer/RenderCommand.h"

namespace nebula::rendering {

    class RenderCommandBuffer
    {
    public:
        explicit RenderCommandBuffer(const size_t buffer_size)
        {
            void* memory_chunk = memory::MemoryManager::requestMemory(buffer_size);
            m_allocator = memory::LinearAllocator(memory_chunk, buffer_size);
        }

        virtual ~RenderCommandBuffer()
        {
            reset();
            memory::MemoryManager::freeMemory(m_allocator.getMemoryPointer());
        }

        virtual void* getBufferHandle() { return this; }

        void dispatch()
        {
            for (RenderCommand* command : m_commands)
                command->execute(getBufferHandle());
            reset();
        }

        template <typename RenderCommand, typename... Args>
        void submit(Args&&... args)
        {
            m_commands.push_back(m_allocator.create<RenderCommand>(std::forward<Args>(args)...));
        }

        template <typename RenderCommand, typename... Args>
        void replace(const int index, Args&&... args)
        {
            RenderCommand* new_command = m_allocator.create<RenderCommand>(std::forward<Args>(args)...);
            m_commands[index] = new_command;
        }

        void reset()
        {
            m_allocator.clear();
            m_commands.clear();
        }

    private:
        memory::LinearAllocator m_allocator{};
        std::vector<RenderCommand*> m_commands{};
    };

}

#endif //RENDERCOMMANDBUFFER_H
