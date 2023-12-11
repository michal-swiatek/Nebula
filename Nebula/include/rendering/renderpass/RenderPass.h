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

#include "rendering/Framebuffer.h"
#include "rendering/AttachmentInfo.h"
#include "rendering/PipelineState.h"

namespace nebula::rendering {

    struct ClearColor
    {
        glm::vec4 color = {};
        glm::vec2 depth_stencil = {1.0f, 0.0f};

        ClearColor() : color(0.0f, 0.0f, 0.2f, 1.0f), depth_stencil(1.0f, 255.0f) {}
        ClearColor(float r, float g, float b, float a) : color(r, g, b, a) {}
    };

    struct RenderStage
    {
        GraphicsPipelineState graphics_pipeline_state;
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

        [[nodiscard]] uint32_t getCurrentStage() const;
        [[nodiscard]] const GraphicsPipelineState& nextStage();

        [[nodiscard]] void* getFramebufferHandle() const;
        [[nodiscard]] uint32_t getNumberOfStages() const;

        [[nodiscard]] const Reference<RenderPassTemplate>& viewRenderPassTemplate() const;
        [[nodiscard]] const Reference<FramebufferTemplate>& viewFramebufferTemplate() const;
        [[nodiscard]] ClearColor getClearColor() const { return m_clear_color; }
        void setClearColor(const ClearColor& clear_color) { m_clear_color = clear_color; }

        [[nodiscard]] static Scope<RenderPass> create(const Reference<RenderPassTemplate>& renderpass_templatebool, bool create_framebuffer = false);

    protected:
        explicit RenderPass(const Reference<RenderPassTemplate>& renderpass_template, bool create_framebuffer);

    private:
        ClearColor m_clear_color{};
        int m_current_render_stage = 0;
        Reference<Framebuffer> m_framebuffer = nullptr;

        Reference<RenderPassTemplate> m_renderpass_template;
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
