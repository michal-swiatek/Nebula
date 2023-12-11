//
// Created by michal-swiatek on 11.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanImGuiBackend.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "core/Application.h"
#include "rendering/RenderContext.h"

using namespace nebula::rendering;

namespace nebula {

    VkDescriptorPool VulkanImGuiBackend::s_descriptor_pool = VK_NULL_HANDLE;

    VulkanImGuiBackend::VulkanImGuiBackend(RenderPass& renderpass) : ImGuiBackend(), m_renderpass(renderpass) {}

    void VulkanImGuiBackend::onAttach()
    {
        NB_CORE_ASSERT(s_descriptor_pool != VK_NULL_HANDLE);

        auto* window = static_cast<GLFWwindow*>(Application::getWindow().getWindowHandle());
        const auto queues_info = VulkanAPI::getQueuesInfo();

        ImGui_ImplGlfw_InitForVulkan(window, true);

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = VulkanAPI::getInstance();
        init_info.PhysicalDevice = VulkanAPI::getPhysicalDevice();
        init_info.Device = VulkanAPI::getDevice();
        init_info.QueueFamily = queues_info.graphics_family_index;
        init_info.Queue = queues_info.graphics_queue;
        init_info.PipelineCache = nullptr;
        init_info.DescriptorPool = s_descriptor_pool;
        init_info.Subpass = 0;
        init_info.MinImageCount = 2;
        init_info.ImageCount = RenderContext::get().getFramesInFlightNumber();
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = nullptr;

        ImGui_ImplVulkan_Init(&init_info, static_cast<VkRenderPass>(m_renderpass.getRenderPassHandle()));
    }

    void VulkanImGuiBackend::onDetach()
    {
        ImGui_ImplVulkan_Shutdown();
    }

    void VulkanImGuiBackend::begin()
    {
        ImGui_ImplVulkan_NewFrame();
    }

    void VulkanImGuiBackend::end(void* command_buffer_handle)
    {
        VkCommandBuffer vulkan_command_buffer = static_cast<VkCommandBuffer>(command_buffer_handle);

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vulkan_command_buffer);
    }

    void VulkanImGuiBackend::init()
    {
        NB_CORE_ASSERT(s_descriptor_pool == VK_NULL_HANDLE);

        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1;
        pool_info.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE(pool_sizes));
        pool_info.pPoolSizes = pool_sizes;

        const auto result = vkCreateDescriptorPool(VulkanAPI::getDevice(), &pool_info, nullptr, &s_descriptor_pool);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan DescriptorPool for ImGui!");
    }

    void VulkanImGuiBackend::shutdown()
    {
        NB_CORE_ASSERT(s_descriptor_pool != VK_NULL_HANDLE);
        vkDestroyDescriptorPool(VulkanAPI::getDevice(), s_descriptor_pool, nullptr);
    }

}
