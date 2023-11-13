//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERER_H
#define RENDERER_H

#include "core/Core.h"
#include "core/Types.h"
#include "core/Assert.h"

#include "RendererAPI.h"
#include "RenderManager.h"

namespace nebula {

    class Application;

    namespace rendering {

        class NEBULA_API Renderer
        {
        public:
            virtual ~Renderer() = default;

            template <typename RenderCommand, typename... Args>
            void submit(Args&&... args)
            {
                NB_CORE_ASSERT(m_current_pass, "Start render pass before submitting commands!");
                m_current_pass->submit<RenderCommand>(std::forward<Args>(args)...);
            }

            void beginPass(View<RenderPass> pass);
            void beginPass(RenderPass::PassID id = 0);
            void endPass();

        protected:
            View<RenderPass> m_current_pass = nullptr;

            static View<impl::RendererApi> s_renderer_api;
            static View<impl::RenderManager> s_render_manager;

        private:
            static void init(API api, View<impl::RenderManager> render_manager);
            static void shutdown();

            static void renderScene();
            static void setRenderingApi(API api);

            friend class nebula::Application;
        };

    }

}

#endif //RENDERER_H
