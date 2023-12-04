//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <vector>
#include <optional>

#include <glm/glm.hpp>

#include "core/Core.h"
#include "core/Types.h"

#include "Framebuffer.h"
#include "AttachmentInfo.h"
#include "PipelineState.h"
#include "CachedPipelineState.h"

namespace nebula::rendering {

    struct ClearColor
    {
        glm::vec4 color = {};
        std::optional<glm::vec4> depth_stencil = std::nullopt;

        ClearColor() : color(), depth_stencil(std::nullopt) {}
        ClearColor(float r, float g, float b, float a) : color(r, g, b, a) {}
    };

    struct RenderStage
    {
        GraphicsPipelineHandle graphics_pipeline_handle = NULL_GRAPHICS_PIPELINE;
        std::vector<AttachmentReference> attachment_references{};
    };

    class RenderPassTemplate;

    class RenderPass
    {
    public:
        virtual ~RenderPass() = default;

        [[nodiscard]] virtual void* getRenderPassHandle() { return this; }

        void startPass();
        void finishPass() const;
        void attachFramebuffer(const Reference<Framebuffer>& framebuffer = nullptr);
        [[nodiscard]] const GraphicsPipelineState& nextStage();

        [[nodiscard]] View<RenderPassTemplate> viewRenderPassTemplate() const;
        [[nodiscard]] View<FramebufferTemplate> viewFramebufferTemplate() const;
        [[nodiscard]] ClearColor getClearColor() const { return m_clear_color; }
        void setClearColor(const ClearColor& clear_color) { m_clear_color = clear_color; }

        [[nodiscard]] static Scope<RenderPass> create(const Reference<RenderPassTemplate>& renderpass_template);

    protected:
        explicit RenderPass(const Reference<RenderPassTemplate>& renderpass_template);

    private:
        ClearColor m_clear_color{};
        int m_current_render_stage = 0;
        Reference<Framebuffer> m_framebuffer = nullptr;

        Scope<RenderPassTemplate> m_renderpass_template;
    };

    class NEBULA_API RenderPassTemplate
    {
    public:
        explicit RenderPassTemplate(const ClearColor& clear_color = {}, const Reference<FramebufferTemplate>& framebuffer_template = nullptr);
        RenderPassTemplate(const RenderPassTemplate& rhs);
        virtual ~RenderPassTemplate() = default;

        [[nodiscard]] ClearColor getClearColor() const { return m_clear_color; }
        [[nodiscard]] const std::vector<RenderStage>& viewRenderStages() const { return m_render_stages; }
        [[nodiscard]] View<FramebufferTemplate> viewFramebufferTemplate() const { return m_framebuffer_template.get(); }

        [[nodiscard]] Scope<RenderPassTemplate> clone() const;

    protected:
        void addStage(const GraphicsPipelineState& graphics_pipeline_state, const std::vector<AttachmentReference>& attachment_references);
        void setFramebufferTemplate(const Reference<FramebufferTemplate>& framebuffer_template) { m_framebuffer_template = framebuffer_template->clone(); }
        void setClearColor(const ClearColor& clear_color) { m_clear_color = clear_color; }

        void preserveAttachments();

    private:
        ClearColor m_clear_color{};
        std::vector<RenderStage> m_render_stages{};
        Scope<FramebufferTemplate> m_framebuffer_template = nullptr;
    };

}

#endif //RENDERPASS_H
