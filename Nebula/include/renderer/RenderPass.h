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
#include "GraphicsPipelineState.h"

namespace nebula::rendering {

    struct ClearColor
    {
        glm::vec4 color = {};
        std::optional<glm::vec4> depth_stencil = std::nullopt;
    };

    struct RenderStage
    {
        GraphicsPipelineState graphics_pipeline_state;
        std::vector<AttachmentReference> attachment_references;
    };

    class RenderPassTemplate;

    class RenderPass
    {
    public:
        explicit RenderPass(const Reference<RenderPassTemplate>& renderpass_template);
        virtual ~RenderPass() = default;

        [[nodiscard]] virtual void* getRenderPassHandle() { return this; }

        void startPass();
        void finishPass() const;
        void attachFramebuffer(const Reference<Framebuffer>& framebuffer);
        [[nodiscard]] const GraphicsPipelineState& nextStage();

        [[nodiscard]] const Reference<FramebufferTemplate>& getFramebufferTemplate() const;
        [[nodiscard]] ClearColor getClearColor() const { return m_clear_color; }
        void setClearColor(const ClearColor& clear_color) { m_clear_color = clear_color; }

        [[nodiscard]] static Scope<RenderPass> create(const Reference<RenderPassTemplate>& renderpass_template);

    private:
        ClearColor m_clear_color{};
        int m_current_render_stage = 0;
        Reference<Framebuffer> m_framebuffer = nullptr;

        Reference<RenderPassTemplate> m_renderpass_template = nullptr;
    };

    class NEBULA_API RenderPassTemplate
    {
    public:
        explicit RenderPassTemplate(const ClearColor& clear_color = {}, const Reference<FramebufferTemplate>& framebuffer_template = nullptr);
        virtual ~RenderPassTemplate() = default;

        [[nodiscard]] ClearColor getClearColor() const { return m_clear_color; }
        [[nodiscard]] const std::vector<RenderStage>& viewRenderStages() const { return m_render_stages; }
        [[nodiscard]] const Reference<FramebufferTemplate>& viewFramebufferTemplate() const { return m_framebuffer_template; }

    protected:
        void addStage(const RenderStage& render_stage);
        void addStage(const GraphicsPipelineState& graphics_pipeline_state, const std::vector<AttachmentReference>& attachment_references);
        void setFramebufferTemplate(const Reference<FramebufferTemplate>& framebuffer_template) { m_framebuffer_template = framebuffer_template; }
        void setClearColor(const ClearColor& clear_color) { m_clear_color = clear_color; }

        void preserveAttachments();

    private:
        ClearColor m_clear_color{};
        std::vector<RenderStage> m_render_stages{};
        Reference<FramebufferTemplate> m_framebuffer_template = nullptr;
    };

}

#endif //RENDERPASS_H
