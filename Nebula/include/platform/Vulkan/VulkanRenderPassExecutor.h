//
// Created by michal-swiatek on 09.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANRENDERPASSEXECUTOR_H
#define VULKANRENDERPASSEXECUTOR_H

#include "platform/Vulkan/VulkanRecordedBuffer.h"
#include "rendering/renderpass/RenderPassExecutor.h"

namespace nebula::rendering {

    class VulkanRenderPassExecutor final : public RenderPassExecutor
    {
    public:
        explicit VulkanRenderPassExecutor(Scope<Renderer>&& renderer);
        explicit VulkanRenderPassExecutor(Scope<RenderPass>&& renderpass);

        void resetResources(uint32_t frame_in_flight) override;

    private:
        Scope<VulkanCommandPool> m_command_pool;

        Scope<RecordedCommandBuffer> recordCommands(Scope<RenderCommandBuffer>&& commands, std::optional<uint32_t> frame_in_flight) const override;
    };

}

#endif //VULKANRENDERPASSEXECUTOR_H
