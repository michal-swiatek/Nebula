//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanCommandsVisitor.h"

#include "platform/Vulkan/VulkanRecordedBuffer.h"

namespace nebula::rendering {

    Scope<RecordedCommandBuffer> VulkanRecordCommandsVisitor::recordCommands(Scope<RenderCommandBuffer>&& commands)
    {
        return createScope<VulkanRecordedBuffer>(nullptr);
    }

    void VulkanExecuteCommandsVisitor::executeCommands(Scope<RecordedCommandBuffer>&& commands)
    {

    }

}
