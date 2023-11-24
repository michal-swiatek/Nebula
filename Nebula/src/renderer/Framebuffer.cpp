//
// Created by michal-swiatek on 23.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "renderer/Framebuffer.h"

#include "core/Assert.h"
#include "core/Application.h"

#include "platform/Vulkan/VulkanFramebuffer.h"
#include "platform/OpenGL/OpenGLFramebuffer.h"

namespace nebula::rendering {

    Reference<Framebuffer> Framebuffer::createFramebuffer(const Reference<FramebufferTemplate>& framebuffer_template)
    {
        switch (Application::get().getRenderingAPI())
        {
            case API::cVulkan:  return createReference<VulkanFramebuffer>(framebuffer_template);
            case API::cOpenGL:  return createReference<OpenGlFramebuffer>(framebuffer_template);
            default:    NB_CORE_ASSERT(false, "Unknown rendering API!");
        }

        return nullptr;
    }

    FramebufferTemplate::FramebufferTemplate(const uint32_t width, const uint32_t height, const uint32_t layers)
        : m_width(width), m_height(height), m_layers(layers) {}

    void FramebufferTemplate::addTextureAttachment(const AttachmentDescription& attachment_description)
    {
        m_texture_attachments.emplace_back(attachment_description);
    }

    void FramebufferTemplate::setDepthStencilAttachment(const std::optional<AttachmentDescription>& depth_stencil_attachment)
    {
        m_depth_stencil_attachment = depth_stencil_attachment;
    }

    const std::vector<AttachmentDescription>& FramebufferTemplate::viewTextureAttachmentsDescriptions() const
    {
        return m_texture_attachments;
    }

    const std::optional<AttachmentDescription>& FramebufferTemplate::viewDepthStencilAttachmentDescription() const
    {
        return m_depth_stencil_attachment;
    }

}
