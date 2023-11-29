//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERER_H
#define RENDERER_H

#include "core/Core.h"
#include "core/Assert.h"

#include "rendering/RenderPass.h"
#include "rendering/commands/RenderCommandBuffer.h"

#include "ForwardRendererBackend.h"

namespace nebula::rendering {

    class NEBULA_API Renderer
    {
    public:
        virtual ~Renderer() = default;

        [[nodiscard]] View<RenderPass> getRenderPass() const;
        [[nodiscard]] Scope<RenderPass> releaseRenderPass();
        void setRenderPass(Scope<RenderPass>&& renderpass);
        void setRenderPass(const Reference<RenderPassTemplate>& renderpass_template);

        void beginRenderPass();
        void endRenderPass();
        void nextRenderStage();

        template <typename RenderCommandType, typename... Args>
        void submitCommand(Args&&... args)
        {
            NB_CORE_ASSERT(m_command_buffer, "Begin RenderPass before submitting RenderCommands!");
            m_command_buffer->submit<RenderCommandType>(std::forward<Args>(args)...);
        }

        template <typename RendererType, typename RendererBackendType = ForwardRendererBackend>
        static Scope<Renderer> create()
        {
            Scope<RendererBackend> renderer_backend = createScope<RendererBackendType>();
            return createScopeFromPointer(new RendererType(std::move(renderer_backend)));
        }

    protected:
        explicit Renderer(Scope<RendererBackend>&& renderer_backend);

    private:
        Scope<RendererBackend> m_renderer_backend = nullptr;

        Scope<RenderPass> m_renderpass = nullptr;
        Scope<RenderCommandBuffer> m_command_buffer = nullptr;
    };

}

#endif //RENDERER_H