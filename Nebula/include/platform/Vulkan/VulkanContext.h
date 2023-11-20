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

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> presentation_family;

    [[nodiscard]] bool checkMinimalSupport() const
    {
        return graphics_family && presentation_family;
    }
};

namespace nebula::rendering {

    class VulkanContext final : public RenderContext
    {
    public:
        explicit VulkanContext(GLFWwindow* window_handle);
        ~VulkanContext() override;

        void bind() override;
        void unbind() override;

        void swapBuffers() override;

    private:
        GLFWwindow* m_window;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;

        VkInstance m_instance{};
        VkDevice m_device{};
        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;

        VkQueue m_graphics_queue = VK_NULL_HANDLE;
        VkQueue m_presentation_queue = VK_NULL_HANDLE;
        QueueFamilyIndices m_queue_family_indices{};

        void createVulkanInstance();
        void pickPhysicalDevice();
        void createSurface();
        void createLogicalDevice();

        QueueFamilyIndices findQueueFamilies() const;

        #ifdef NB_DEBUG_BUILD
        VkDebugUtilsMessengerEXT m_debug_messenger{};
        #endif
    };

}

#endif //VULKANCONTEXT_H
