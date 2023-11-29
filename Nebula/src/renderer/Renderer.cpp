//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/renderer/Renderer.h"

namespace nebula::rendering {

    Renderer::Renderer(Scope<RendererBackend>&& renderer_backend) : m_renderer_backend(std::move(renderer_backend)) {}

    void Renderer::beginRenderPass()
    {
        NB_CORE_ASSERT(m_renderpass, "No RenderPass set!");
        NB_CORE_ASSERT(!m_command_buffer, "Finish previous renderpass before starting new one!");

        m_command_buffer = RenderCommandBuffer::create();

        m_renderpass->startPass();
        nextRenderStage();
    }

    void Renderer::endRenderPass()
    {
        NB_CORE_ASSERT(m_command_buffer, "Start renderpass first!");

        m_renderpass->finishPass();
        m_command_buffer.reset();   //  TODO: Pass to RendererBackend
    }

    void Renderer::nextRenderStage()
    {
        NB_CORE_ASSERT(m_command_buffer, "Start renderpass first!");

        auto graphics_pipeline_state = m_renderpass->nextStage();
    }

    View<RenderPass> Renderer::getRenderPass() const
    {
        return m_renderpass.get();
    }

    void Renderer::setRenderPass(Scope<RenderPass>&& renderpass)
    {
        m_renderpass = std::move(renderpass);
    }

    void Renderer::setRenderPass(const Reference<RenderPassTemplate>& renderpass_template)
    {
        m_renderpass = RenderPass::create(renderpass_template);
    }

}
