//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERER_H
#define RENDERER_H

#include "core/Core.h"

#include "RendererBackend.h"
#include "RenderCommandPool.h"

#include "renderer/RenderPass.h"
#include "renderer/RenderCommandPool.h"
#include "renderer/RenderCommandBuffer.h"

namespace nebula::rendering {

    class NEBULA_API Renderer
    {
    public:
        explicit Renderer(RendererBackendType renderer_backend);
        virtual ~Renderer() = default;

        [[nodiscard]] View<RenderPass> getRenderPass() const { return m_renderpass.get(); }
        void setRenderPass(Scope<RenderPass>&& renderpass);
        void setRenderPass(const Reference<RenderPassTemplate>& renderpass_template);

        void beginRenderPass();
        void endRenderPass();
        void nextRenderStage();

        template <typename RendererType>
        static Scope<Renderer> create(RendererBackendType renderer_backend)
        {
            return createScope<RendererType>(renderer_backend);
        }

    private:
        Scope<RendererBackend> m_renderer_backend = nullptr;
        Scope<RenderCommandPool> m_command_pool = nullptr;

        Scope<RenderPass> m_renderpass = nullptr;
        Scope<RenderCommandBuffer> m_command_buffer = nullptr;
    };

}

#endif //RENDERER_H
