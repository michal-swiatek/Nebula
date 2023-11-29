//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include "core/Types.h"
#include "platform/PlatformAPI.h"

#include "renderer/PipelineState.h"

namespace nebula { class Application; }

namespace nebula::rendering {

    class RendererApi
    {
    public:
        virtual ~RendererApi() = default;

        virtual void init() = 0;
        virtual void shutdown() = 0;

        virtual void compilePipeline(const GraphicsPipelineState& graphics_pipeline_state, void* renderpass_handle) = 0;

        static View<RendererApi> get();

    private:
        static Scope<RendererApi> s_renderer_api;

        static void create(API api);
        static void destroy();

        //  TODO: Friend MainRenderThread instead
        friend class nebula::Application;
    };

}

#endif //RENDERERAPI_H
