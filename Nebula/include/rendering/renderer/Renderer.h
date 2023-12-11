//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERER_H
#define RENDERER_H

#include <optional>

#include "core/Core.h"
#include "core/Assert.h"

#include "ForwardRendererBackend.h"
#include "rendering/RenderObjectVisitor.h"
#include "rendering/renderpass/RenderPass.h"
#include "rendering/commands/RenderPassCommands.h"
#include "rendering/commands/RenderCommandBuffer.h"

namespace nebula::rendering {

    class NEBULA_API Renderer : public RenderObjectVisitor
    {
    public:
        ~Renderer() override = default;

        void setRenderPass(Scope<RenderPass>&& renderpass);
        void setRenderPass(const Reference<RenderPassTemplate>& renderpass_template, bool create_framebuffer = false);
        void setFramebuffer(const Reference<Framebuffer>& framebuffer) const;

        void setRenderArea(const std::optional<RenderArea>& render_area = {});
        void setViewport(const std::optional<RenderArea>& render_area = {});
        void setScissor(const std::optional<RenderArea>& render_area = {});

        [[nodiscard]] View<RenderPass> viewRenderPass() const;
        [[nodiscard]] Scope<RenderPass> releaseRenderPass();

        void beginRenderPass();
        void endRenderPass();
        void nextRenderStage();

        [[nodiscard]] Scope<RenderCommandBuffer> getCommandBuffer() const;

        template <typename RendererType, typename RendererBackendType = ForwardRendererBackend>
        static Scope<Renderer> create()
        {
            Scope<RendererBackend> renderer_backend = createScope<RendererBackendType>();
            return createScopeFromPointer(new RendererType(std::move(renderer_backend)));
        }

    protected:
        explicit Renderer(Scope<RendererBackend>&& renderer_backend);

        template <typename RenderCommandType, typename... Args>
        void submitCommand(Args&&... args)
        {
            NB_CORE_ASSERT(m_command_buffer, "Command buffer not set! Start RenderPass to set new command buffer.");
            m_command_buffer->submit<RenderCommandType>(std::forward<Args>(args)...);
        }

        void draw(const ImGuiRenderObject& imgui_layer) override;
        void draw(const DummyVerticesRenderObject& render_object) override;

    private:
        Scope<RendererBackend> m_renderer_backend = nullptr;
        Scope<RenderCommandBuffer> m_command_buffer = nullptr;
        Scope<RenderPass> m_renderpass = nullptr;

        RenderArea m_render_area{};
        RenderArea m_viewport{};
        RenderArea m_scissor{};

        void setRenderAreas();

        enum RenderPassState
        {
            cUndefined,
            cStarted,
            cFinished
        };

        RenderPassState m_renderpass_state = cUndefined;
    };

}

#endif //RENDERER_H
