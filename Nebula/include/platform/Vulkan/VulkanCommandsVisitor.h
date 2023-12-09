//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANCOMMANDSVISITOR_H
#define VULKANCOMMANDSVISITOR_H

#include "rendering/commands/RenderCommandVisitor.h"
#include "platform/Vulkan/VulkanAPI.h"

namespace nebula::rendering {

    class VulkanRecordCommandsVisitor final : public RecordCommandVisitor
    {
    public:
        explicit VulkanRecordCommandsVisitor(VkCommandBuffer command_buffer);

        Scope<RecordedCommandBuffer> recordCommands(Scope<RenderCommandBuffer>&& commands) override;

    private:
        VkCommandBuffer m_command_buffer = VK_NULL_HANDLE;
    };

    class VulkanExecuteCommandsVisitor final : public ExecuteCommandVisitor
    {
    public:
        void executeCommands(Scope<RecordedCommandBuffer>&& commands) override;
    };

}

#endif //VULKANCOMMANDSVISITOR_H
