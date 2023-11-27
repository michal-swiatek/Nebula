//
// Created by michal-swiatek on 13.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "renderer/RenderPass.h"

#include "core/Assert.h"

namespace nebula::rendering {

    RenderPass::RenderPass(const Reference<RenderPassTemplate>& renderpass_template)
    {
        m_renderpass_template = renderpass_template;

        m_clear_color = renderpass_template->getClearColor();
        m_framebuffer = Framebuffer::create(renderpass_template->viewFramebufferTemplate());
    }

    void RenderPass::attachFramebuffer(const Reference<Framebuffer>& framebuffer)
    {
        NB_CORE_ASSERT(framebuffer->getFramebufferTemplate() == m_renderpass_template->viewFramebufferTemplate(), "Incompatible Framebuffer!");
        m_framebuffer = framebuffer;
        m_framebuffer->attachTo(getRenderPassHandle());
    }

    void RenderPass::startPass()
    {
        NB_CORE_ASSERT(m_current_render_stage >= 0, "Iterate over all Render stages before starting new RenderPass!");
        m_current_render_stage = -1;
    }

    void RenderPass::finishPass() const
    {
        NB_CORE_ASSERT(m_current_render_stage == m_renderpass_template->viewRenderStages().size(), "Iterate over all Render stages before finishing RenderPass!");
    }

    const GraphicsPipelineState& RenderPass::nextStage()
    {
        NB_CORE_ASSERT(m_current_render_stage < m_renderpass_template->viewRenderStages().size() - 1, "RenderStage index out of range!");
        const auto& render_stages = m_renderpass_template->viewRenderStages();
        return render_stages[++m_current_render_stage].graphics_pipeline_state;
    }

    const Reference<FramebufferTemplate>& RenderPass::getFramebufferTemplate() const
    {
        return m_renderpass_template->viewFramebufferTemplate();
    }

}
