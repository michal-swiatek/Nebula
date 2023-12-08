//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanContext.h"

#include <GLFW/glfw3.h>

#include "core/Assert.h"
#include "core/Logging.h"

#include "platform/EngineConfiguration.h"
#include "platform/Vulkan/VulkanAPI.h"
#include "platform/Vulkan/VulkanSwapchain.h"

namespace nebula::rendering {

    VulkanContext::VulkanContext(GLFWwindow* window_handle) : m_window(window_handle)
    {
        NB_CORE_ASSERT(m_window, "Window handle in null!");

        m_vulkan_api = VulkanAPI::create(m_window);
        m_surface = m_vulkan_api->getSurface();

        m_swapchain = createScope<VulkanSwapchain>(m_surface);
        setVSync(true); //  Creates swapchain with proper parameters

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

    VulkanContext::~VulkanContext() = default;

    void VulkanContext::bind()
    {

    }

    void VulkanContext::unbind()
    {

    }

    void VulkanContext::swapBuffers()
    {

    }

    const Reference<FramebufferTemplate>& VulkanContext::viewFramebufferTemplate() const
    {
        return VulkanSwapchain::viewFramebufferTemplate();
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
