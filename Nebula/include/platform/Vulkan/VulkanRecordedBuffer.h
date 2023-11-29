//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANRECORDEDBUFFER_H
#define VULKANRECORDEDBUFFER_H

#include "rendering/commands/RenderCommandBuffer.h"

#include "VulkanAPI.h"

namespace nebula::rendering {

    class VulkanRecordedBuffer final : public RecordedCommandBuffer
    {
    public:
        explicit VulkanRecordedBuffer(VkCommandBuffer command_buffer) : m_command_buffer(command_buffer) {}

        void* getBufferHandle() override { return m_command_buffer; }

    private:
        VkCommandBuffer m_command_buffer;
    };

}

#endif //VULKANRECORDEDBUFFER_H
