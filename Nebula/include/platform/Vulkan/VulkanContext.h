//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANCONTEXT_H
#define VULKANCONTEXT_H

#include <mutex>
#include <vector>

#include "rendering/RenderContext.h"
#include "platform/Vulkan/VulkanAPI.h"

struct GLFWwindow;
struct VkSurfaceKHR_T;

namespace nebula::rendering {

    class VulkanAPI;
    class VulkanSwapchain;

    struct VulkanFrameSynchronization
    {
        VkSemaphore image_available = VK_NULL_HANDLE;
        VkSemaphore render_finished = VK_NULL_HANDLE;
        VkFence frame_resources_free = VK_NULL_HANDLE;

        //  To protect fence between threads
        std::mutex mutex;

        VulkanFrameSynchronization();
        ~VulkanFrameSynchronization();
    };

    class VulkanContext final : public RenderContext
    {
    public:
        explicit VulkanContext(GLFWwindow* window_handle);
        ~VulkanContext() override;

        void waitForFrameResources(uint32_t frame) override;

        void bind() override;
        void unbind() override;

        bool checkVSync() override;
        void setVSync(bool vsync) override;

        void presentImage() override;
        Reference<Framebuffer> getNextImage() override;

        Scope<ExecuteCommandVisitor> getCommandExecutor() override;

        [[nodiscard]] const Reference<FramebufferTemplate>& viewFramebufferTemplate() const override;

    private:
        GLFWwindow* m_window;
        VkSurfaceKHR_T* m_surface = nullptr;

        Scope<VulkanAPI> m_vulkan_api;
        Scope<VulkanSwapchain> m_swapchain;

        std::vector<VulkanFrameSynchronization> m_frame_synchronizations;
    };

}

#endif //VULKANCONTEXT_H
