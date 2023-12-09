//
// Created by michal-swiatek on 09.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanRenderPassExecutor.h"

#include "platform/Vulkan/VulkanCommandsVisitor.h"

namespace nebula::rendering {

    VulkanRenderPassExecutor::VulkanRenderPassExecutor(Scope<Renderer>&& renderer) : RenderPassExecutor(std::move(renderer))
    {
        m_command_pool = createScope<VulkanCommandPool>();
    }

    VulkanRenderPassExecutor::VulkanRenderPassExecutor(Scope<RenderPass>&& renderpass) : RenderPassExecutor(std::move(renderpass))
    {
        m_command_pool = createScope<VulkanCommandPool>();
    }

    void VulkanRenderPassExecutor::resetResources(uint32_t frame_in_flight)
    {
        m_command_pool->reset(frame_in_flight);
    }

    Scope<RecordedCommandBuffer> VulkanRenderPassExecutor::recordCommands(Scope<RenderCommandBuffer>&& commands, std::optional<uint32_t> frame_in_flight) const
    {
        NB_ASSERT(frame_in_flight.has_value());

        const auto vulkan_command_buffer = m_command_pool->getCommandBuffer(*frame_in_flight);
        const auto command_recorder = createScope<VulkanRecordCommandsVisitor>(vulkan_command_buffer);

        return command_recorder->recordCommands(std::move(commands));
    }

}
