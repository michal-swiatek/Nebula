//
// Created by michal-swiatek on 23.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/Framebuffer.h"

#include "core/Application.h"

namespace nebula::rendering {

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

    bool FramebufferTemplate::hasDepthStencilAttachment() const
    {
        return m_depth_stencil_attachment.has_value();
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
