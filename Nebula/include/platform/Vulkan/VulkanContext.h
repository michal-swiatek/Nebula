//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANCONTEXT_H
#define VULKANCONTEXT_H

#include <optional>
#include <vulkan/vulkan.h>

#include "renderer/RenderContext.h"

struct GLFWwindow;

namespace nebula::rendering {

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> presentation_family;

        [[nodiscard]] bool checkMinimalSupport() const
        {
            return graphics_family && presentation_family;
        }
    };

    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    class VulkanContext final : public RenderContext
    {
    public:
        explicit VulkanContext(GLFWwindow* window_handle);
        ~VulkanContext() override;

        void bind() override;
        void unbind() override;

        bool checkVSync() override;
        void setVSync(bool vsync) override;

        void swapBuffers() override;

    private:
        GLFWwindow* m_window;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;

        bool m_vsync = true;
        VkExtent2D m_extent{};
        VkPresentModeKHR m_present_mode{};
        VkSurfaceFormatKHR m_surface_format{};

        std::vector<VkImage> m_swapchain_images{};
        std::vector<VkImageView> m_swapchain_image_views{};
        std::vector<const char*> m_device_extensions{};

        VkInstance m_instance{};
        VkDevice m_device{};
        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;

        VkQueue m_graphics_queue = VK_NULL_HANDLE;
        VkQueue m_presentation_queue = VK_NULL_HANDLE;

        QueueFamilyIndices m_queue_family_indices{};
        SwapchainSupportDetails m_swapchain_details{};

        void createVulkanInstance();
        void createSurface();
        void createPhysicalDevice();
        void createLogicalDevice();
        void createSwapchain();
        void createImageViews();

        [[nodiscard]] VkSurfaceFormatKHR chooseSwapSurfaceFormat() const;
        [[nodiscard]] VkPresentModeKHR chooseSwapPresentMode() const;
        [[nodiscard]] VkExtent2D chooseSwapExtent() const;

        #ifdef NB_DEBUG_BUILD
        VkDebugUtilsMessengerEXT m_debug_messenger{};
        #endif
    };

}

#endif //VULKANCONTEXT_H
