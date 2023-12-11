//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanSwapchain.h"

#include "platform/Vulkan/VulkanAPI.h"

VkImageViewCreateInfo createSwapchainImageViewInfo(VkImage image, VkFormat surface_format);

namespace nebula::rendering {

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes, bool vsync);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);

    //////////////////////////////////////////////////////////////////
    /////  VulkanSwapchainImages  ////////////////////////////////////
    //////////////////////////////////////////////////////////////////

    VulkanSwapchainImages::VulkanSwapchainImages(
        VkSwapchainKHR swapchain,
        VkSurfaceFormatKHR surface_format,
        uint32_t width,
        uint32_t height
    ) :
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

    VulkanSwapchainImages::~VulkanSwapchainImages()
    {
        for (const auto& image_view : m_swapchain_image_views)
            vkDestroyImageView(VulkanAPI::getDevice(), image_view, nullptr);
    }

    uint32_t VulkanSwapchainImages::getImageCount() const
    {
        return m_swapchain_images.size();
    }

    const std::vector<VkImageView>& VulkanSwapchainImages::viewImageViews() const
    {
        return m_swapchain_image_views;
    }

    //////////////////////////////////////////////////////////////////
    /////  VulkanSwapchain  //////////////////////////////////////////
    //////////////////////////////////////////////////////////////////

    VulkanSwapchain::VulkanSwapchain(VkSurfaceKHR surface) : m_surface(surface) {}

    VulkanSwapchain::~VulkanSwapchain()
    {
        m_framebuffers.clear();
        m_swapchain_images.reset();
        vkDestroySwapchainKHR(VulkanAPI::getDevice(), m_swapchain, nullptr);
    }

    void VulkanSwapchain::presentImage(VkSemaphore render_finished) const
    {
        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &render_finished;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &m_swapchain;
        present_info.pImageIndices = &m_current_image_index;
        present_info.pResults = nullptr;

        const auto queue_info = VulkanAPI::getQueuesInfo();
        const auto result = vkQueuePresentKHR(queue_info.presentation_queue, &present_info);

        NB_CORE_ASSERT(result == VK_SUCCESS || result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR);
    }

    Reference<Framebuffer> VulkanSwapchain::getNextImage(VkSemaphore image_available)
    {
        const auto result = vkAcquireNextImageKHR(VulkanAPI::getDevice(), m_swapchain, UINT64_MAX, image_available, VK_NULL_HANDLE, &m_current_image_index);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
            return nullptr;
        if (result != VK_SUCCESS)
            throw std::runtime_error("Failed to acquire Vulkan swapchain image!");

        return m_framebuffers[m_current_image_index];
    }

    void VulkanSwapchain::recreateSwapchain(const uint32_t width, const uint32_t height, const bool vsync)
    {
        if (width == 0 || height == 0)
            return;

        vkDeviceWaitIdle(VulkanAPI::getDevice());

        if (m_swapchain)
        {
            m_framebuffers.clear();
            m_swapchain_images.reset();
            vkDestroySwapchainKHR(VulkanAPI::getDevice(), m_swapchain, nullptr);
        }

        const auto swapchain_details = querySwapchainSupport(VulkanAPI::getPhysicalDevice(), m_surface);

        m_surface_format = chooseSwapSurfaceFormat(swapchain_details.formats);
        m_extent = chooseSwapExtent(swapchain_details.capabilities, width, height);
        m_present_mode = chooseSwapPresentMode(swapchain_details.present_modes, vsync);

        createSwapchain(swapchain_details.capabilities);
        createFramebuffers();

        NB_CORE_INFO("Recreated swapchain: {} images ({}, {}), VSync: {}", m_swapchain_images->getImageCount(), width, height, vsync ? "on" : "off");
    }

    void VulkanSwapchain::createSwapchain(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        uint32_t image_count = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount)
            image_count = capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = m_surface;
        create_info.minImageCount = image_count;
        create_info.imageFormat = m_surface_format.format;
        create_info.imageColorSpace = m_surface_format.colorSpace;
        create_info.imageExtent = m_extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        create_info.preTransform = capabilities.currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = m_present_mode;
        create_info.clipped = VK_TRUE;
        create_info.oldSwapchain = VK_NULL_HANDLE;

        const auto [_1, _2, graphics_family_index, presentation_family_index] = VulkanAPI::getQueuesInfo();
        const uint32_t queue_family_indices[] = {graphics_family_index, presentation_family_index};
        if (graphics_family_index != presentation_family_index)
        {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_family_indices;
        }
        else
        {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            create_info.queueFamilyIndexCount = 0;
            create_info.pQueueFamilyIndices = nullptr;
        }

        const VkResult status = vkCreateSwapchainKHR(VulkanAPI::getDevice(), &create_info, nullptr, &m_swapchain);
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to create Vulkan swapchain!");
    }

    void VulkanSwapchain::createFramebuffers()
    {
        m_swapchain_images = createScope<VulkanSwapchainImages>(m_swapchain, m_surface_format, m_extent.width, m_extent.height);
        m_swapchain_framebuffer_template = createReference<SwapchainFramebufferTemplate>(m_extent.width, m_extent.height);

        for (auto image_view : m_swapchain_images->viewImageViews())
            m_framebuffers.emplace_back(createScope<VulkanSwapchainFramebuffer>(image_view, m_swapchain_framebuffer_template));
    }

    bool VulkanSwapchain::checkVSync() const
    {
        return m_present_mode == VK_PRESENT_MODE_FIFO_KHR;
    }

    const Reference<FramebufferTemplate>& VulkanSwapchain::viewFramebufferTemplate() const
    {
        return m_swapchain_framebuffer_template;
    }

    //////////////////////////////////////////////////////////////////
    /////  Utility functions  ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////

    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        SwapchainSupportDetails details;
        uint32_t format_count;
        uint32_t present_mode_count;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

        if (format_count > 0)
        {
            details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
        }

        if (present_mode_count > 0)
        {
            details.present_modes.resize(present_mode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats)
    {
        for (const auto& format : available_formats)
            if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return format;
        NB_CORE_ASSERT(false, "Vulkan surface format not supported!");
        return {};
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes, const bool vsync)
    {
        for (const auto& present_mode : available_present_modes)
            if (!vsync && present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
                return present_mode;
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            return capabilities.currentExtent;

        VkExtent2D extent = {width, height};

        extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return extent;
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
