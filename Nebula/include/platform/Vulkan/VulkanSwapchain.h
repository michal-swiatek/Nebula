//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANSWAPCHAIN_H
#define VULKANSWAPCHAIN_H

#include "core/Types.h"
#include "VulkanFramebuffer.h"

namespace nebula::rendering {

    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    class VulkanSwapchainImages
    {
    public:
        VulkanSwapchainImages(VkSwapchainKHR swapchain, VkSurfaceFormatKHR surface_format, uint32_t width, uint32_t height);
        ~VulkanSwapchainImages();

        [[nodiscard]] const std::vector<VkImageView>& viewImageViews() const;

    private:
        uint32_t m_width;
        uint32_t m_height;
        VkSurfaceFormatKHR m_surface_format;

        std::vector<VkImage> m_swapchain_images{};
        std::vector<VkImageView> m_swapchain_image_views{};
    };

    class VulkanSwapchain
    {
    public:
        explicit VulkanSwapchain(VkSurfaceKHR surface);
        ~VulkanSwapchain();

        void recreateSwapchain(uint32_t width, uint32_t height, bool vsync);

        [[nodiscard]] bool checkVSync() const;
        static const Reference<FramebufferTemplate>& viewFramebufferTemplate();

    private:
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;

        Scope<VulkanSwapchainImages> m_swapchain_images = nullptr;
        std::vector<Scope<VulkanSwapchainFramebuffer>> m_framebuffers;

        VkExtent2D m_extent{};
        VkPresentModeKHR m_present_mode{};
        VkSurfaceFormatKHR m_surface_format{};

        void createSwapchain(const VkSurfaceCapabilitiesKHR& capabilities);
        void createFramebuffers();
    };

}

#endif //VULKANSWAPCHAIN_H
