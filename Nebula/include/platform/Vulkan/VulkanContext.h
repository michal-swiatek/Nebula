//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANCONTEXT_H
#define VULKANCONTEXT_H

#include "rendering/RenderContext.h"

struct GLFWwindow;
struct VkSurfaceKHR_T;

namespace nebula::rendering {

    struct VulkanAPI;
    struct VulkanSwapchain;

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
        VkSurfaceKHR_T* m_surface = nullptr;

        Scope<VulkanAPI> m_vulkan_api;
        Scope<VulkanSwapchain> m_swapchain;
    };

}

#endif //VULKANCONTEXT_H
