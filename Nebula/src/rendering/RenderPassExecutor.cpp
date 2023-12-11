//
// Created by michal-swiatek on 07.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/renderpass/RenderPassExecutor.h"

#include "core/Assert.h"

namespace nebula::rendering {

    RenderPassExecutor::RenderPassExecutor(Scope<Renderer>&& renderer)
    {
        NB_CORE_ASSERT(renderer);
        NB_CORE_ASSERT(renderer->viewRenderPass(), "Renderer has to hold RenderPass!");
        m_renderer = std::move(renderer);
    }

    RenderPassExecutor::RenderPassExecutor(Scope<RenderPass>&& renderpass)
    {
        NB_CORE_ASSERT(renderpass);
        m_renderpass = std::move(renderpass);
    }

    void RenderPassExecutor::resetResources(uint32_t frame_in_flight)
    {

    }

    Scope<RecordedCommandBuffer> RenderPassExecutor::execute(const RenderPassObjects& renderpass_objects, const std::optional<uint32_t> frame_in_flight) const
    {
        NB_CORE_ASSERT(m_renderer, "Renderer has to be set to begin execution!");

        const auto renderpass = m_renderer->viewRenderPass();

        m_renderer->beginRenderPass();

        for (uint32_t stage = 0; stage < renderpass->getNumberOfStages(); stage++)
        {
            const auto& stage_objects = renderpass_objects.viewStageObjects(stage);
            for (const auto object : stage_objects)
                object->accept(*m_renderer);

            //  Don't move to next stage if this was the last stage
            if (stage < renderpass->getNumberOfStages() - 1)
                m_renderer->nextRenderStage();
        }

        m_renderer->endRenderPass();

        return recordCommands(m_renderer->getCommandBuffer(), frame_in_flight);
    }

    Scope<RecordedCommandBuffer> RenderPassExecutor::recordCommands(Scope<RenderCommandBuffer>&& commands, std::optional<uint32_t> frame_in_flight) const
    {
        return commands;
    }

    void RenderPassExecutor::setRenderer(Scope<Renderer>&& renderer)
    {
        NB_CORE_ASSERT(renderer);
        NB_CORE_ASSERT(!renderer->viewRenderPass(), "Cannot set Renderer with existing RenderPass!");

        if (m_renderer)
            m_renderpass = m_renderer->releaseRenderPass();

        m_renderer = std::move(renderer);
        m_renderer->setRenderPass(std::move(m_renderpass));
    }

    void RenderPassExecutor::setFramebuffer(const Reference<Framebuffer>& framebuffer) const
    {
        NB_CORE_ASSERT(framebuffer);
        m_renderer->setFramebuffer(framebuffer);
    }

    View<Renderer> RenderPassExecutor::viewRenderer() const
    {
        return m_renderer.get();
    }

    View<RenderPass> RenderPassExecutor::viewRenderPass() const
    {
        return m_renderer->viewRenderPass();
    }

}
