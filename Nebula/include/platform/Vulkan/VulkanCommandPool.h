//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANCOMMANDPOOL_H
#define VULKANCOMMANDPOOL_H

#include <vector>
#include <vulkan/vulkan.h>

#include "renderer/commands/RenderCommandBuffer.h"

namespace nebula::rendering {

    class VulkanCommandBuffer final : public RenderCommandBuffer
    {
    public:
        explicit VulkanCommandBuffer(VkCommandBuffer buffer_handle, size_t buffer_size);

        void* getBufferHandle() override;

    private:
        VkCommandBuffer m_command_buffer = VK_NULL_HANDLE;
    };

    class VulkanCommandPool final : public RenderCommandPool
    {
    public:
        explicit VulkanCommandPool(size_t command_buffer_size);
        ~VulkanCommandPool() override;

        Scope<RenderCommandBuffer> createBuffer() override;
        void reset() override;

    private:
        VkCommandPool m_command_pool = VK_NULL_HANDLE;

        int m_current_buffer_index = 0;
        std::vector<VkCommandBuffer> m_command_buffers{};

        [[nodiscard]] VkCommandBuffer createVulkanCommandBufferHandle() const;
    };

}

#endif //VULKANCOMMANDPOOL_H
