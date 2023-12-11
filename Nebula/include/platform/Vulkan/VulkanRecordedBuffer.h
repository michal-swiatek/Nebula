//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANRECORDEDBUFFER_H
#define VULKANRECORDEDBUFFER_H

#include <vector>

#include "rendering/commands/RenderCommandBuffer.h"

#include "VulkanAPI.h"

namespace nebula::rendering {

    class VulkanCommandPool
    {
    public:
        VulkanCommandPool();
        ~VulkanCommandPool();

        void reset(uint32_t frame_in_flight);
        VkCommandBuffer getCommandBuffer(uint32_t frame_in_flight);

    private:
        using CachedBuffers = std::vector<VkCommandBuffer>;

        std::vector<uint32_t> m_cache_indices;
        std::vector<VkCommandPool> m_command_pools;
        std::vector<CachedBuffers> m_cached_buffers;

        void createPools();
    };

    class VulkanRecordedBuffer final : public RecordedCommandBuffer
    {
    public:
        explicit VulkanRecordedBuffer(VkCommandBuffer command_buffer) : m_command_buffer(command_buffer) {}

        void* getBufferHandle() override { return m_command_buffer; }
        [[nodiscard]] const std::vector<RenderCommand*>& viewCommands() const override { throw std::runtime_error("No implementation for Vulkan"); }

    private:
        VkCommandBuffer m_command_buffer;
    };

}

#endif //VULKANRECORDEDBUFFER_H
