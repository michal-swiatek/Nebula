//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANCOMMANDSVISITOR_H
#define VULKANCOMMANDSVISITOR_H

#include "platform/Vulkan/VulkanAPI.h"
#include "platform/Vulkan/VulkanContext.h"
#include "rendering/commands/RenderCommandVisitor.h"

namespace nebula::rendering {

    class VulkanRecordCommandsVisitor final : public RecordCommandVisitor
    {
    public:
        explicit VulkanRecordCommandsVisitor(VkCommandBuffer command_buffer);

        Scope<RecordedCommandBuffer> recordCommands(Scope<RenderCommandBuffer>&& commands) override;

        void visit(BeginRenderPassCommand& command) override;
        void visit(EndRenderPassCommand& command) override;

    private:
        VkCommandBuffer m_command_buffer = VK_NULL_HANDLE;

        void startRecording() const;
        void endRecording() const;
    };

    class VulkanExecuteCommandsVisitor final : public ExecuteCommandVisitor
    {
    public:
        explicit VulkanExecuteCommandsVisitor(VulkanFrameSynchronization& frame_synchronization);

        void executeCommands(Scope<RecordedCommandBuffer>&& commands) override;
        void submitCommands() override;

    private:
        VulkanFrameSynchronization& m_frame_synchronization;
        std::vector<VkCommandBuffer> m_vulkan_commands;
    };

}

#endif //VULKANCOMMANDSVISITOR_H
