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
        NB_CORE_ASSERT(m_renderpass_state != cStarted, "Finish previous renderpass before starting new one!");

        m_renderpass_state = cStarted;
        m_command_buffer = RenderCommandBuffer::create();

        m_renderpass->startPass();
        nextRenderStage();

        submitCommand<BeginRenderPassCommand>(*m_renderpass.get(), m_render_area);
    }

    void Renderer::endRenderPass()
    {
        NB_CORE_ASSERT(m_renderpass_state == cStarted, "Start renderpass first!");

        submitCommand<EndRenderPassCommand>(*m_renderpass.get());

        m_renderpass->finishPass();
        m_renderer_backend->processRenderCommands(std::move(m_command_buffer));

        m_renderpass_state = cFinished;
    }

    void Renderer::nextRenderStage() const
    {
        NB_CORE_ASSERT(m_renderpass_state == cStarted, "Start renderpass before moving to next RenderStage!");

        auto graphics_pipeline_state = m_renderpass->nextStage();
    }

    Scope<RenderCommandBuffer> Renderer::getCommandBuffer() const
    {
        NB_CORE_ASSERT(m_renderpass_state == cFinished, "Finish RenderPass to retrieve RenderCommands!");
        if (m_renderpass_state == cFinished)
            return m_renderer_backend->getCommandBuffer();
        return nullptr;
    }

    void Renderer::setRenderPass(Scope<RenderPass>&& renderpass)
    {
        m_renderpass = std::move(renderpass);
        setRenderArea();
    }

    void Renderer::setRenderPass(const Reference<RenderPassTemplate>& renderpass_template, const bool create_framebuffer)
    {
        m_renderpass = RenderPass::create(renderpass_template, create_framebuffer);
        setRenderArea();
    }

    void Renderer::setFramebuffer(const Reference<Framebuffer>& framebuffer) const
    {
        NB_CORE_ASSERT(m_renderpass);
        m_renderpass->attachFramebuffer(framebuffer);
    }

    void Renderer::setRenderArea(const std::optional<RenderArea>& render_area)
    {
        const auto& framebuffer_template = m_renderpass->viewFramebufferTemplate();
        if (render_area)
            m_render_area = *render_area;
        else
            m_render_area = RenderArea(0, 0, framebuffer_template->getWidth(), framebuffer_template->getHeight());
    }

    Scope<RenderPass> Renderer::releaseRenderPass()
    {
        return createScopeFromPointer(m_renderpass.release());
    }

    View<RenderPass> Renderer::viewRenderPass() const
    {
        return m_renderpass.get();
    }

}
