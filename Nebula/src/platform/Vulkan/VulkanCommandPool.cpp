//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanCommandPool.h"

#include "platform/Vulkan/VulkanAPI.h"

namespace nebula::rendering {

    VulkanCommandBuffer::VulkanCommandBuffer(
        VkCommandBuffer buffer_handle,
        const size_t buffer_size
    ) :
            RenderCommandBuffer(buffer_size),
            m_command_buffer(buffer_handle)
    {
        VkCommandBufferBeginInfo begin_info{};

        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        begin_info.pInheritanceInfo = nullptr;

        const auto result = vkBeginCommandBuffer(m_command_buffer, &begin_info);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Unable to start Vulkan command buffer recording!");
    }

    void* VulkanCommandBuffer::getBufferHandle()
    {
        return m_command_buffer;
    }

    VulkanCommandPool::VulkanCommandPool(const size_t command_buffer_size) : RenderCommandPool(command_buffer_size)
    {
        const auto queues_info = VulkanAPI::getQueuesInfo();

        VkCommandPoolCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        create_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        create_info.queueFamilyIndex = queues_info.graphics_family_index;

        const auto result = vkCreateCommandPool(VulkanAPI::getDevice(), &create_info, nullptr, &m_command_pool);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Unable to create Vulkan command pool!");
    }

    VulkanCommandPool::~VulkanCommandPool()
    {
        vkDestroyCommandPool(VulkanAPI::getDevice(), m_command_pool, nullptr);
    }

    Scope<RenderCommandBuffer> VulkanCommandPool::createBuffer()
    {
        if (m_current_buffer_index >= m_command_buffers.size())
            m_command_buffers.push_back(createVulkanCommandBufferHandle());

        return createScope<VulkanCommandBuffer>(m_command_buffers[m_current_buffer_index++], m_command_buffer_size);
    }

    void VulkanCommandPool::reset()
    {
        const auto result = vkResetCommandPool(VulkanAPI::getDevice(), m_command_pool, 0);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Unable to reset Vulkan command pool!");

        m_current_buffer_index = 0;
    }

    VkCommandBuffer VulkanCommandPool::createVulkanCommandBufferHandle() const
    {
        VkCommandBuffer command_buffer = {};

        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.commandPool = m_command_pool;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandBufferCount = 1;

        const auto result = vkAllocateCommandBuffers(VulkanAPI::getDevice(), &alloc_info, &command_buffer);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Unable to create Vulkan command buffer!");

        return command_buffer;
    }

}
