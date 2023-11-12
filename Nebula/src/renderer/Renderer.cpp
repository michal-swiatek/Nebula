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

    void Renderer::init(API api)
    {
        NB_CORE_ASSERT(!s_renderer_api, "Renderer should only be initialized once!");
        setRenderingApi(api);
    }

    void Renderer::shutdown()
    {
        NB_CORE_ASSERT(s_renderer_api, "Uninitialized Rendering API!");
        RendererApi::destroy(s_renderer_api);
        s_renderer_api = nullptr;
    }

    void Renderer::setRenderingApi(API api)
    {
        if (s_renderer_api)
            RendererApi::destroy(s_renderer_api);

        s_renderer_api = RendererApi::create(api);
        RenderCommand::s_renderer_api = s_renderer_api;
    }

}
