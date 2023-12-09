//
// Created by michal-swiatek on 09.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanRecordedBuffer.h"

#include "rendering/RenderContext.h"

namespace nebula::rendering {

    VulkanCommandPool::VulkanCommandPool()
    {
        const uint32_t frames_in_flight = RenderContext::get().getFramesInFlightNumber();

        m_cache_indices.resize(frames_in_flight, 0);
        m_command_pools.resize(frames_in_flight, nullptr);
        m_cached_buffers.resize(frames_in_flight);

        createPools();
    }

    VulkanCommandPool::~VulkanCommandPool()
    {
        for (auto command_pool : m_command_pools)
            vkDestroyCommandPool(VulkanAPI::getDevice(), command_pool, nullptr);
    }

    void VulkanCommandPool::createPools()
    {
        const uint32_t frames_in_flight = RenderContext::get().getFramesInFlightNumber();

        VkCommandPoolCreateInfo command_pool_create_info = {};
        command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        command_pool_create_info.pNext = nullptr;
        command_pool_create_info.queueFamilyIndex = VulkanAPI::getQueuesInfo().graphics_family_index;
        command_pool_create_info.flags = 0;

        for (uint32_t i = 0; i < frames_in_flight; ++i)
        {
            const auto result = vkCreateCommandPool(VulkanAPI::getDevice(), &command_pool_create_info, nullptr, &m_command_pools[i]);
            NB_CORE_ASSERT(result == VK_SUCCESS, "Failed to create VulkanCommandPool!");
        }
    }

    void VulkanCommandPool::reset(const uint32_t frame_in_flight)
    {
        NB_CORE_ASSERT(frame_in_flight < RenderContext::get().getFramesInFlightNumber());

        const auto result = vkResetCommandPool(VulkanAPI::getDevice(), m_command_pools[frame_in_flight], VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Failed to reset VulkanCommandPool!");

        m_cache_indices[frame_in_flight] = 0;
    }

    VkCommandBuffer VulkanCommandPool::getCommandBuffer(const uint32_t frame_in_flight)
    {
        NB_CORE_ASSERT(frame_in_flight < RenderContext::get().getFramesInFlightNumber());

        if (m_cache_indices[frame_in_flight] == m_cached_buffers[frame_in_flight].size())
        {
            VkCommandBufferAllocateInfo allocate_info = {};
            allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocate_info.pNext = nullptr;
            allocate_info.commandPool = m_command_pools[frame_in_flight];
            allocate_info.commandBufferCount = 1;
            allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            m_cached_buffers[frame_in_flight].push_back(VK_NULL_HANDLE);
            const auto result = vkAllocateCommandBuffers(VulkanAPI::getDevice(), &allocate_info, &m_cached_buffers[frame_in_flight].back());
            NB_CORE_ASSERT(result == VK_SUCCESS, "Failed to allocate VulkanCommandBuffer!");
        }

        return m_cached_buffers[frame_in_flight][m_cache_indices[frame_in_flight]++];
    }

}
