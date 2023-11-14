//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "renderer/Renderer.h"

#include "threads/RenderThread.h"
#include "renderer/RenderCommand.h"
#include "renderer/utils/RenderPassTemplates.h"

namespace nebula::rendering {

    using namespace impl;

    View<RendererApi> Renderer::s_renderer_api = nullptr;
    View<RendererApi> RenderCommand::s_renderer_api = nullptr;
    View<threads::RenderThread> Renderer::s_render_thread = nullptr;

    void Renderer::beginFrame(Scope<RenderPassTemplate>&& render_passes)
    {
        NB_CORE_ASSERT(!m_current_frame, "End previous frame before starting new one!");
        NB_CORE_ASSERT(s_render_thread, "Renderer must be initialized before calling beginFrame()!");
        if (!render_passes)
            render_passes = createScope<DefaultRenderPass>();

        m_current_frame = createScope<Frame>(std::move(render_passes));
    }

    void Renderer::endFrame()
    {
        NB_CORE_ASSERT(!m_current_pass, "End render pass before ending frame!");
        NB_CORE_ASSERT(s_render_thread, "Renderer must be initialized before calling endFrame()!");
        if (m_current_frame)
            s_render_thread->submitFrame(std::move(m_current_frame));
        m_current_frame = nullptr;
    }

    void Renderer::beginPass()
    {
        NB_CORE_ASSERT(!m_current_pass, "End render pass before staring new one!");
        NB_CORE_ASSERT(s_render_thread, "Renderer must be initialized before calling beginPass()!");
        m_current_pass = m_current_frame->getRenderPasses().getNextPass();
    }

    void Renderer::endPass()
    {
        NB_CORE_ASSERT(m_current_pass, "Start render pass first!");
        NB_CORE_ASSERT(s_render_thread, "Renderer must be initialized before calling endPass()!");
        m_current_pass = nullptr;
    }

    void Renderer::init(API api, View<threads::RenderThread> render_thread)
    {
        NB_CORE_ASSERT(!s_renderer_api, "Renderer should only be initialized once!");
        NB_CORE_ASSERT(!s_render_thread, "Renderer should only be initialized once!");

        setRenderingApi(api);
        s_render_thread = render_thread;
    }

    void Renderer::shutdown()
    {
        NB_CORE_ASSERT(s_renderer_api, "Uninitialized Rendering API!");
        RendererApi::destroy(s_renderer_api);
        s_renderer_api = nullptr;
        s_render_thread = nullptr;
        RenderCommand::s_renderer_api = nullptr;
    }

    void Renderer::setRenderingApi(const API api)
    {
        if (s_renderer_api)
            RendererApi::destroy(s_renderer_api);

        s_renderer_api = RendererApi::create(api);
        RenderCommand::s_renderer_api = s_renderer_api;

        s_renderer_api->init();
    }

}
