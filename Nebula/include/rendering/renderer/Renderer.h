//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERER_H
#define RENDERER_H

#include "core/Core.h"

#include "RendererBackend.h"
#include "rendering/RenderPass.h"
#include "rendering/commands/RenderCommandBuffer.h"

namespace nebula::rendering {

    class NEBULA_API Renderer
    {
    public:
        explicit Renderer(Scope<RendererBackend>&& renderer_backend);
        virtual ~Renderer() = default;

        [[nodiscard]] View<RenderPass> getRenderPass() const;
        void setRenderPass(Scope<RenderPass>&& renderpass);
        void setRenderPass(const Reference<RenderPassTemplate>& renderpass_template);

        void beginRenderPass();
        void endRenderPass();
        void nextRenderStage();

        template <typename RendererType, typename RendererBackendType = RendererBackend>
        static Scope<Renderer> create()
        {
            auto renderer_backend = createScope<RendererBackendType>();
            return createScope<RendererType>(std::move(renderer_backend));
        }

    private:
        Scope<RendererBackend> m_renderer_backend = nullptr;

        Scope<RenderPass> m_renderpass = nullptr;
        Scope<RenderCommandBuffer> m_command_buffer = nullptr;
    };

}

#endif //RENDERER_H
