//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanContext.h"

#include <GLFW/glfw3.h>
#include <platform/Vulkan/VulkanCommandsVisitor.h>

#include "core/Assert.h"
#include "core/Logging.h"

#include "platform/EngineConfiguration.h"
#include "platform/Vulkan/VulkanAPI.h"
#include "platform/Vulkan/VulkanSwapchain.h"

namespace nebula::rendering {

    VulkanFrameSynchronization::VulkanFrameSynchronization()
    {
        VkSemaphoreCreateInfo semaphore_info = {};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_info = {};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        const auto fence_result = vkCreateFence(VulkanAPI::getDevice(), &fence_info, nullptr, &frame_resources_free);
        const auto image_available_semaphore = vkCreateSemaphore(VulkanAPI::getDevice(), &semaphore_info, nullptr, &image_available);
        const auto render_finished_semaphore = vkCreateSemaphore(VulkanAPI::getDevice(), &semaphore_info, nullptr, &render_finished);

        NB_CORE_ASSERT(fence_result == VK_SUCCESS);
        NB_CORE_ASSERT(image_available_semaphore == VK_SUCCESS);
        NB_CORE_ASSERT(render_finished_semaphore == VK_SUCCESS);
    }

    VulkanFrameSynchronization::~VulkanFrameSynchronization()
    {
        vkDestroySemaphore(VulkanAPI::getDevice(), image_available, nullptr);
        vkDestroySemaphore(VulkanAPI::getDevice(), render_finished, nullptr);
        vkDestroyFence(VulkanAPI::getDevice(), frame_resources_free, nullptr);
    }

    VulkanContext::VulkanContext(GLFWwindow* window_handle) : m_window(window_handle)
    {
        NB_CORE_ASSERT(m_window);

        m_vulkan_api = VulkanAPI::create(m_window);
        m_surface = m_vulkan_api->getSurface();

        m_swapchain = createScope<VulkanSwapchain>(m_surface);
        setVSync(true); //  Creates swapchain with proper parameters

        m_frame_synchronizations = std::vector<VulkanFrameSynchronization>(getFramesInFlightNumber());

        if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= NEBULA_INITIALIZATION_VERBOSITY_LOW)
        {
            const auto device_properties = VulkanAPI::getPhysicalDeviceInfo();
            NB_CORE_INFO("Vulkan Info:");
            NB_CORE_INFO("  Vendor: {0}", device_properties.vendor);
            NB_CORE_INFO("  Renderer: {0}", device_properties.renderer);
            NB_CORE_INFO("  Vulkan version: {0}", device_properties.api_version);
            NB_CORE_INFO("  Driver version: {0}", device_properties.driver_version);
        }
    }

    VulkanContext::~VulkanContext()
    {
        m_frame_synchronizations.clear();
        m_swapchain.reset();
        m_vulkan_api.reset();
    }

    void VulkanContext::bind()
    {

    }

    void VulkanContext::unbind()
    {

    }

    void VulkanContext::reload()
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);

        m_swapchain->recreateSwapchain(width, height, m_swapchain->checkVSync());
    }

    void VulkanContext::presentImage()
    {
        m_swapchain->presentImage(m_frame_synchronizations[getCurrentRenderFrame()].render_finished);
        m_current_render_frame = (m_current_render_frame + 1) % m_frames_in_flight_number;
    }

    Reference<Framebuffer> VulkanContext::getNextImage()
    {
        return m_swapchain->getNextImage(m_frame_synchronizations[getCurrentRenderFrame()].image_available);
    }

    Scope<ExecuteCommandVisitor> VulkanContext::getCommandExecutor()
    {
        return createScope<VulkanExecuteCommandsVisitor>(m_frame_synchronizations[getCurrentRenderFrame()]);
    }

    void VulkanContext::waitForFrameResources(const uint32_t frame)
    {
        auto& frame_synchronization = m_frame_synchronizations[frame];
        std::lock_guard lock{frame_synchronization.mutex};  //  TODO: Check for deadlocks
        vkWaitForFences(VulkanAPI::getDevice(), 1, &frame_synchronization.frame_resources_free, VK_TRUE, UINT64_MAX);
    }

    const Reference<FramebufferTemplate>& VulkanContext::viewFramebufferTemplate() const
    {
        return m_swapchain->viewFramebufferTemplate();
    }

    bool VulkanContext::checkVSync()
    {
        return m_swapchain->checkVSync();
    }

    void VulkanContext::setVSync(const bool vsync)
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);

        m_swapchain->recreateSwapchain(width, height, vsync);
    }

}
