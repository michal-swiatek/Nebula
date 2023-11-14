//
// Created by michal-swiatek on 13.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "renderer/RenderPass.h"

#include "core/Config.h"
#include "memory/MemoryManager.h"

namespace nebula::rendering {

    RenderPass::RenderPass() : m_allocator()
    {
        const auto& engine_config = Config::getEngineConfig();
        int render_queue_memory_size = engine_config["memory"]["render_queue_memory_size"].as<int>();
        void* memory_chunk = memory::MemoryManager::requestMemory(render_queue_memory_size);
        m_allocator = std::move(memory::LinearAllocator(memory_chunk, render_queue_memory_size));
    }

    RenderPass::~RenderPass()
    {
        memory::MemoryManager::freeMemory(m_allocator.getMemoryPointer());
    }

    void RenderPass::dispatch()
    {
        prepareQueues();

        for (const auto& command : m_configuration_queue)
            command->execute();

        for (const auto& command : m_render_queue)
            command->execute();
    }

    RenderPassTemplate::~RenderPassTemplate() = default;

    View<RenderPass> RenderPassTemplate::getNextPass()
    {
        if (m_current_pass >= m_render_passes.size())
            throw std::out_of_range(
                std::format(
                    "Render pass out of range: index {} with {} available passes",
                    m_current_pass,
                    m_render_passes.size()
                )
            );

        return m_render_passes[m_current_pass++].get();
    }

    void RenderPassTemplate::dispatchRenderPasses() const
    {
        for (const auto& render_pass : m_render_passes)
            render_pass->dispatch();
    }

}
