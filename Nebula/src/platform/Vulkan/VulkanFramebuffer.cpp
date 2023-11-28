//
// Created by michal-swiatek on 24.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanFramebuffer.h"

#include "platform/Vulkan/VulkanTextureFormats.h"

VkImageViewCreateInfo createSwapchainImageViewInfo(VkImage image, VkFormat surface_format);

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
        if (m_framebuffer)
            vkDestroyFramebuffer(VulkanAPI::getDevice(), m_framebuffer, nullptr);

        for (const auto& image_view : m_image_views)
            vkDestroyImageView(VulkanAPI::getDevice(), image_view, nullptr);

        for (auto [image, allocation] : m_image_buffers)
            vmaDestroyImage(VulkanAPI::getVmaAllocator(), image, allocation);
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

        const auto result = vkCreateFramebuffer(VulkanAPI::getDevice(), &create_info, nullptr, &m_framebuffer);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Unable to create framebuffer!");
    }

    const Reference<FramebufferTemplate>& VulkanFramebuffer::getFramebufferTemplate() const
    {
        return m_framebuffer_template;
    }

    void VulkanFramebuffer::createAttachment(const AttachmentDescription& attachment_description, const bool depth_stencil)
    {
        VkApiAllocatedImage image_buffer;
        VkImageView image_view;
        VkResult result;

        VkImageCreateInfo image_create_info{};
        image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
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

        // result = vkCreateImage(VulkanAPI::getDevice(), &image_create_info, nullptr, &image);
        VmaAllocationCreateInfo allocation_info{};
        allocation_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        result = vmaCreateImage(VulkanAPI::getVmaAllocator(), &image_create_info, &allocation_info, &image_buffer.image, &image_buffer.allocation, nullptr);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Unable to create Vulkan framebuffer image!");

        VkImageViewCreateInfo image_view_create_info{};
        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.image = image_buffer.image;
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

        result = vkCreateImageView(VulkanAPI::getDevice(), &image_view_create_info, nullptr, &image_view);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Unable to create Vulkan framebuffer image view!");

        m_image_buffers.push_back(image_buffer);
        m_image_views.push_back(image_view);
    }

    ///////////////////////////////////////////////////////////////////////////////////
    ////  VulkanSwapchainFramebuffers  ////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////

    VulkanSwapchainFramebuffers::VulkanSwapchainFramebuffers(
        VkSwapchainKHR swapchain,
        VkSurfaceFormatKHR surface_format,
        const uint32_t width,
        const uint32_t height
    ):
            m_width(width),
            m_height(height),
            m_surface_format(surface_format)
    {
        uint32_t image_count;
        vkGetSwapchainImagesKHR(VulkanAPI::getDevice(), swapchain, &image_count, nullptr);

        m_swapchain_images.resize(image_count);
        vkGetSwapchainImagesKHR(VulkanAPI::getDevice(), swapchain, &image_count, m_swapchain_images.data());

        m_swapchain_image_views.resize(image_count);
        for (int i = 0; i < m_swapchain_images.size(); ++i)
        {
            auto create_info = createSwapchainImageViewInfo(m_swapchain_images[i], m_surface_format.format);
            const VkResult status = vkCreateImageView(VulkanAPI::getDevice(), &create_info, nullptr, &m_swapchain_image_views[i]);
            NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to create swapchain image view!");
        }
    }

    VulkanSwapchainFramebuffers::~VulkanSwapchainFramebuffers()
    {
        for (const auto& framebuffer : m_swapchain_framebuffers)
            vkDestroyFramebuffer(VulkanAPI::getDevice(), framebuffer, nullptr);

        for (const auto& image_view : m_swapchain_image_views)
            vkDestroyImageView(VulkanAPI::getDevice(), image_view, nullptr);
    }

    void VulkanSwapchainFramebuffers::bind()
    {
        NB_CORE_ASSERT(attached(), "Cannot bind framebuffer that is not attached to any renderpass!");
    }

    void VulkanSwapchainFramebuffers::unbind()
    {

    }

    bool VulkanSwapchainFramebuffers::attached() const
    {
        return !m_swapchain_framebuffers.empty();
    }

    void VulkanSwapchainFramebuffers::attachTo(void* renderpass_handle)
    {
        NB_ASSERT(renderpass_handle, "Recieved null renderpass handle!");
        m_swapchain_framebuffers.resize(m_swapchain_image_views.size());

        for (int i = 0; i < m_swapchain_framebuffers.size(); ++i)
        {
            VkFramebufferCreateInfo create_info{};
            const VkImageView attachments[] = {m_swapchain_image_views[i]};

            create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            create_info.renderPass = static_cast<VkRenderPass>(renderpass_handle);
            create_info.attachmentCount = 1;
            create_info.pAttachments = attachments;
            create_info.width = m_width;
            create_info.height = m_height;
            create_info.layers = 1;

            const auto result = vkCreateFramebuffer(VulkanAPI::getDevice(), &create_info, nullptr, &m_swapchain_framebuffers[i]);
            NB_CORE_ASSERT(result == VK_SUCCESS, "Unable to create swapchain framebuffer!");
        }
    }

    const Reference<FramebufferTemplate>& VulkanSwapchainFramebuffers::getFramebufferTemplate() const
    {
        NB_CORE_ASSERT(false, "VulkanSwapchainFramebuffer has no FramebufferTemplate!");
        throw std::exception();
    }

}

VkImageViewCreateInfo createSwapchainImageViewInfo(VkImage image, const VkFormat surface_format)
{
    VkImageViewCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = image;
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = surface_format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    return create_info;
}
