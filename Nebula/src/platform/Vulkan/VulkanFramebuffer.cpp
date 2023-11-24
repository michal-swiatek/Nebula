//
// Created by michal-swiatek on 24.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanFramebuffer.h"

#include "platform/Vulkan/VulkanContext.h"
#include "platform/Vulkan/VulkanTextureFormats.h"

namespace nebula::rendering {

    VulkanFramebuffer::VulkanFramebuffer(const Reference<FramebufferTemplate>& framebuffer_template) :
            m_framebuffer_template(framebuffer_template)
    {
        for (const auto& attachment_description : framebuffer_template->viewTextureAttachmentsDescriptions())
            createAttachment(attachment_description, false);

        if (framebuffer_template->viewDepthStencilAttachmentDescription())
            createAttachment(*framebuffer_template->viewDepthStencilAttachmentDescription(), true);
    }

    VulkanFramebuffer::~VulkanFramebuffer()
    {
        for (const auto& image_view : m_image_views)
            vkDestroyImageView(VulkanContext::getDevice(), image_view, nullptr);

        for (const auto& image : m_images)
            vkDestroyImage(VulkanContext::getDevice(), image, nullptr);
    }

    void VulkanFramebuffer::bind()
    {
        NB_CORE_ASSERT(attached(), "Cannot bind framebuffer that is not attached to any renderpass!");
    }

    void VulkanFramebuffer::unbind()
    {

    }

    bool VulkanFramebuffer::attached() const
    {
        return m_framebuffer != VK_NULL_HANDLE;
    }

    void VulkanFramebuffer::attachTo(void* renderpass_handle)
    {
        NB_ASSERT(renderpass_handle, "Recieved null renderpass handle!");
        VkFramebufferCreateInfo create_info{};

        create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        create_info.renderPass = static_cast<VkRenderPass>(renderpass_handle);
        create_info.attachmentCount = static_cast<uint32_t>(m_image_views.size());
        create_info.pAttachments = m_image_views.data();
        create_info.width = m_framebuffer_template->getWidth();
        create_info.height = m_framebuffer_template->getHeight();
        create_info.layers = m_framebuffer_template->getLayers();

        const auto result = vkCreateFramebuffer(VulkanContext::getDevice(), &create_info, nullptr, &m_framebuffer);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Unable to create framebuffer!");
    }

    const Reference<FramebufferTemplate>& VulkanFramebuffer::getFramebufferTemplate() const
    {
        return m_framebuffer_template;
    }

    void VulkanFramebuffer::createAttachment(const AttachmentDescription& attachment_description, const bool depth_stencil)
    {
        VkImageCreateInfo image_create_info{};

        image_create_info.imageType = VK_IMAGE_TYPE_2D;
        image_create_info.format = getVulkanTextureFormat(attachment_description.format);
        image_create_info.extent.width = m_framebuffer_template->getWidth();
        image_create_info.extent.height = m_framebuffer_template->getHeight();
        image_create_info.extent.depth = m_framebuffer_template->getLayers();
        image_create_info.mipLevels = 1;
        image_create_info.arrayLayers = 1;
        image_create_info.samples = getVulkanTextureSampling(attachment_description.samples);
        image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        if (depth_stencil)
            image_create_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        else
            image_create_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

        VkImageViewCreateInfo image_view_create_info{};

        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = getVulkanTextureFormat(attachment_description.format);
        image_view_create_info.flags = 0;
        image_view_create_info.subresourceRange = {};
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;
        if (depth_stencil)
        {
            image_view_create_info.subresourceRange.aspectMask = 0;
            if (formatHasDepth(attachment_description.format))
                image_view_create_info.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
            if (formatHasStencil(attachment_description.format))
                image_view_create_info.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
        else
            image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        VkImage image;
        VkImageView image_view;
        VkResult result;

        result = vkCreateImage(VulkanContext::getDevice(), &image_create_info, nullptr, &image);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Unable to create Vulkan framebuffer image!");

        //  TODO: Allocate memory for image and pass it to GPU

        result = vkCreateImageView(VulkanContext::getDevice(), &image_view_create_info, nullptr, &image_view);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Unable to create Vulkan framebuffer image view!");

        m_images.push_back(image);
        m_image_views.push_back(image_view);
    }

}
