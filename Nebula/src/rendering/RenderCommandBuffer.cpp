//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/commands/RenderCommandBuffer.h"

#include "core/Config.h"
#include "memory/MemoryManager.h"

namespace nebula::rendering {

    RenderCommandBuffer::RenderCommandBuffer(const size_t buffer_size)
    {
        void* memory_chunk = memory::MemoryManager::requestMemory(buffer_size);
        m_allocator = memory::LinearAllocator(memory_chunk, buffer_size);
    }

    RenderCommandBuffer::~RenderCommandBuffer()
    {
        reset();
        memory::MemoryManager::freeMemory(m_allocator.getMemoryPointer());
    }

    void RenderCommandBuffer::reset()
    {
        m_allocator.clear();
        m_commands.clear();
    }

    void* RenderCommandBuffer::getBufferHandle()
    {
        return m_commands.data();
    }

    const std::vector<RenderCommand*>& RenderCommandBuffer::viewCommands() const
    {
        return m_commands;
    }

    Scope<RenderCommandBuffer> RenderCommandBuffer::create()
    {
        auto& config = Config::getEngineConfig();
        const auto command_buffer_size = config["memory"]["event_queue_size"].as<size_t>();

        return createScopeFromPointer(new RenderCommandBuffer(command_buffer_size));
    }

}
