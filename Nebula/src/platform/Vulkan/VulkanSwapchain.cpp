//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanSwapchain.h"

#include "platform/Vulkan/VulkanAPI.h"

namespace nebula::rendering {

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes, bool vsync);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);

    VulkanSwapchain::VulkanSwapchain(VkSurfaceKHR surface) : m_surface(surface) {}

    VulkanSwapchain::~VulkanSwapchain()
    {
        if (m_swapchain)
            vkDestroySwapchainKHR(VulkanAPI::getDevice(), m_swapchain, nullptr);
    }

    void VulkanSwapchain::recreateSwapchain(const uint32_t width, const uint32_t height, bool vsync)
    {
        vkDeviceWaitIdle(VulkanAPI::getDevice());

        if (m_swapchain)
        {
            m_framebuffers.reset();
            vkDestroySwapchainKHR(VulkanAPI::getDevice(), m_swapchain, nullptr);
        }

        const auto swapchain_details = querySwapchainSupport(VulkanAPI::getPhysicalDevice(), m_surface);

        m_surface_format = chooseSwapSurfaceFormat(swapchain_details.formats);
        m_extent = chooseSwapExtent(swapchain_details.capabilities, width, height);
        m_present_mode = chooseSwapPresentMode(swapchain_details.present_modes, vsync);

        createSwapchain(swapchain_details.capabilities);
        createFramebuffers();
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
        create_info.oldSwapchain = m_swapchain;

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
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to create swapchain!");
    }

    void VulkanSwapchain::createFramebuffers()
    {
        m_framebuffers = createScope<VulkanSwapchainFramebuffers>(m_swapchain, m_surface_format, m_extent.width, m_extent.height);
    }

    bool VulkanSwapchain::checkVSync() const
    {
        return m_present_mode == VK_PRESENT_MODE_FIFO_KHR;
    }

    const Reference<FramebufferTemplate>& VulkanSwapchain::viewFramebufferTemplate() const
    {
        return m_framebuffers->viewFramebufferTemplate();
    }

    //
    //  Utility functions
    //

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
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return format;
        return available_formats[0];
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
