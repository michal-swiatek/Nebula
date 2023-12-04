//
// Created by michal-swiatek on 13.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/RenderPass.h"

#include "core/Assert.h"

namespace nebula::rendering {

    RenderPass::RenderPass(const Reference<RenderPassTemplate>& renderpass_template)
    {
        m_renderpass_template = renderpass_template->clone();

        m_clear_color = renderpass_template->getClearColor();
        m_framebuffer = Framebuffer::create(m_renderpass_template->viewFramebufferTemplate());
    }

    void RenderPass::attachFramebuffer(const Reference<Framebuffer>& framebuffer)
    {
        NB_CORE_ASSERT(*framebuffer->viewFramebufferTemplate() == *m_renderpass_template->viewFramebufferTemplate(), "Incompatible Framebuffer!");
        m_framebuffer = framebuffer;
        m_framebuffer->attachTo(getRenderPassHandle());
    }

    void RenderPass::startPass()
    {
        NB_CORE_ASSERT(m_framebuffer, "Attach framebuffer before starting RenderPass!");
        NB_CORE_ASSERT(m_current_render_stage >= 0, "Iterate over all Render stages before starting new RenderPass!");
        if (!m_framebuffer->attached())
            attachFramebuffer(m_framebuffer);
        m_current_render_stage = -1;
    }

    void RenderPass::finishPass() const
    {
        NB_CORE_ASSERT(m_current_render_stage == m_renderpass_template->viewRenderStages().size() - 1, "Iterate over all Render stages before finishing RenderPass!");
    }

    const GraphicsPipelineState& RenderPass::nextStage()
    {
        NB_CORE_ASSERT(m_current_render_stage < static_cast<int>(m_renderpass_template->viewRenderStages().size() - 1), "RenderStage index out of range!");
        const auto& render_stages = m_renderpass_template->viewRenderStages();
        const auto pipeline_handle = render_stages[++m_current_render_stage].graphics_pipeline_handle;
        return CachedPipelineState::getPipelineState(pipeline_handle);
    }

    View<RenderPassTemplate> RenderPass::viewRenderPassTemplate() const
    {
        return m_renderpass_template.get();
    }

    View<FramebufferTemplate> RenderPass::viewFramebufferTemplate() const
    {
        return m_renderpass_template->viewFramebufferTemplate();
    }

    RenderPassTemplate::RenderPassTemplate(const ClearColor& clear_color, const Reference<FramebufferTemplate>& framebuffer_template)
    {
        m_clear_color = clear_color;
        m_framebuffer_template = framebuffer_template->clone();
    }

    RenderPassTemplate::RenderPassTemplate(const RenderPassTemplate& rhs)
    {
        m_clear_color = rhs.m_clear_color;
        m_render_stages = rhs.m_render_stages;
        m_framebuffer_template = rhs.m_framebuffer_template->clone();
    }

    void RenderPassTemplate::addStage(const GraphicsPipelineState& graphics_pipeline_state, const std::vector<AttachmentReference>& attachment_references)
    {
        NB_CORE_ASSERT(m_framebuffer_template, "Set FramebufferTemplate before adding RenderStages!");
        const size_t attachment_count = m_framebuffer_template->getAttachmentCount();
        for (const auto& attachment_reference : attachment_references)
            NB_CORE_ASSERT(attachment_reference.index < attachment_count, "Attachment reference index out of bounds!");

        const auto pipeline_handle = CachedPipelineState::getPipelineHandle(graphics_pipeline_state);
        m_render_stages.emplace_back(pipeline_handle, attachment_references);
    }

    void RenderPassTemplate::preserveAttachments()
    {
        //  TODO: Implement
    }

    Scope<RenderPassTemplate> RenderPassTemplate::clone() const
    {
        return createScope<RenderPassTemplate>(*this);
    }

}
