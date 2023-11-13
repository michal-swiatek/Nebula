//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "renderer/Renderer.h"
#include "renderer/RenderCommand.h"

namespace nebula::rendering {

    using namespace impl;

    View<RendererApi> Renderer::s_renderer_api = nullptr;
    View<RendererApi> RenderCommand::s_renderer_api = nullptr;
    View<RenderManager> Renderer::s_render_manager = nullptr;

    void Renderer::renderScene()
    {
        s_render_manager->dispatchPasses();
    }

    void Renderer::beginPass(View<RenderPass> pass)
    {
        NB_CORE_ASSERT(!m_current_pass, "Starting render pass without finishing previous one!");
        m_current_pass = pass;
    }

    void Renderer::beginPass(RenderPass::PassID id)
    {
        NB_CORE_ASSERT(!m_current_pass, "Starting render pass without finishing previous one!");
        m_current_pass = s_render_manager->getPassByID(id);
    }

    void Renderer::endPass()
    {
        NB_CORE_ASSERT(m_current_pass, "Start render pass first!");
        m_current_pass = nullptr;
    }

    void Renderer::init(API api, View<RenderManager> render_manager)
    {
        NB_CORE_ASSERT(!s_renderer_api, "Renderer should only be initialized once!");
        NB_CORE_ASSERT(!s_render_manager, "RenderManager should only be initialized once!");

        s_render_manager = render_manager;
        setRenderingApi(api);
    }

    void Renderer::shutdown()
    {
        NB_CORE_ASSERT(s_renderer_api, "Uninitialized Rendering API!");
        NB_CORE_ASSERT(s_render_manager, "Uninitialized RenderManager!");
        RendererApi::destroy(s_renderer_api);
        s_renderer_api = nullptr;
        s_render_manager = nullptr;
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
