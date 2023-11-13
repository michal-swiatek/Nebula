//
// Created by michal-swiatek on 13.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "renderer/RenderPass.h"

#include "core/Config.h"
#include "memory/MemoryManager.h"

namespace nebula::rendering {

    RenderPass::PassID RenderPass::s_id_counter = 0;

    RenderPass::RenderPass() : m_id(s_id_counter++), m_allocator()
    {
        const auto& engine_config = Config::getEngineConfig();
        int render_queue_memory_size = engine_config["memory"]["render_queue_memory_size"].as<int>();
        void* memory_chunk = memory::MemoryManager::requestMemory(render_queue_memory_size);
        m_allocator = std::move(memory::LinearAllocator(memory_chunk, render_queue_memory_size));
    }

    void RenderPass::dispatch()
    {
        prepareQueues();

        for (const auto& command : m_configuration_queue)
            command->execute();

        for (const auto& command : m_render_queue)
            command->execute();

        //  Destructors are not called - RenderCommands shouldn't store dynamic data
        m_configuration_queue.clear();
        m_render_queue.clear();
        m_allocator.clear();
    }

}
