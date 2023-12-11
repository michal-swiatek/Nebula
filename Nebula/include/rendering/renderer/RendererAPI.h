//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include "core/Types.h"
#include "platform/PlatformAPI.h"

#include "rendering/renderpass/RenderPass.h"

namespace nebula {

    namespace threads { class MainRenderThread; }

    namespace rendering {

        class RendererApi
        {
        public:
            virtual ~RendererApi() = default;

            virtual void compilePipelines(RenderPass& renderpass) = 0;
            virtual void* getPipelineHandle(RenderPass& renderpass, uint32_t stage) = 0;

            static RendererApi& get();

        protected:
            RendererApi() = default;

        private:
            static Scope<RendererApi> s_renderer_api;

            static void create(API api);
            static void destroy();

            friend class nebula::threads::MainRenderThread;
        };

    }

}

#endif //RENDERERAPI_H
