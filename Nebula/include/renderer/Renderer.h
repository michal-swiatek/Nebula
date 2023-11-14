//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERER_H
#define RENDERER_H

#include "core/Core.h"
#include "core/Types.h"
#include "core/Assert.h"

#include "Frame.h"
#include "RenderPass.h"
#include "RendererAPI.h"

namespace nebula {

    namespace threads {

        class RenderThread;

    }

    namespace rendering {

        class NEBULA_API Renderer
        {
        public:
            virtual ~Renderer() = default;

            void beginFrame(Scope<RenderPassTemplate>&& render_passes = nullptr);
            void endFrame();

            void beginPass();
            void endPass();

            template <typename RenderCommand, typename... Args>
            void submit(Args&&... args)
            {
                NB_CORE_ASSERT(m_current_pass, "Start render pass before submitting commands!");
                m_current_pass->submit<RenderCommand>(std::forward<Args>(args)...);
            }

            template <typename RenderPassesTemplate>
            void beginFrame()
            {
                auto render_passes = createScope<RenderPassesTemplate>();
                beginFrame(std::move(render_passes));
            }

        protected:
            View<RenderPass> m_current_pass = nullptr;
            Scope<impl::Frame> m_current_frame = nullptr;

            static View<impl::RendererApi> s_renderer_api;
            static View<threads::RenderThread> s_render_thread;

        private:
            static void init(API api, View<threads::RenderThread> render_thread);
            static void shutdown();

            static void setRenderingApi(API api);

            friend class nebula::threads::RenderThread;
        };

    }

}

#endif //RENDERER_H
