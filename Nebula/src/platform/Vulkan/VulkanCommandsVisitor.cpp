//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanCommandsVisitor.h"

#include "platform/Vulkan/VulkanRecordedBuffer.h"

namespace nebula::rendering {

    VulkanRecordCommandsVisitor::VulkanRecordCommandsVisitor(VkCommandBuffer command_buffer) : m_command_buffer(command_buffer) {}

    Scope<RecordedCommandBuffer> VulkanRecordCommandsVisitor::recordCommands(Scope<RenderCommandBuffer>&& commands)
    {
        for (const auto command : commands->viewCommands())
            command->accept(*this);

        return createScope<VulkanRecordedBuffer>(m_command_buffer);
    }

    void VulkanExecuteCommandsVisitor::executeCommands(Scope<RecordedCommandBuffer>&& commands)
    {

    }

}
