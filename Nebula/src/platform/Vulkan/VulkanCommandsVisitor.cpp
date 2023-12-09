//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanCommandsVisitor.h"

#include <rendering/commands/RenderPassCommands.h>

#include "rendering/renderpass/RenderPass.h"
#include "platform/Vulkan/VulkanRecordedBuffer.h"

namespace nebula::rendering {

    ////////////////////////////////////////////////////////////////////
    //////  VulkanRecordCommandsVisitor  ///////////////////////////////
    ////////////////////////////////////////////////////////////////////

    VulkanRecordCommandsVisitor::VulkanRecordCommandsVisitor(VkCommandBuffer command_buffer) : m_command_buffer(command_buffer) {}

    Scope<RecordedCommandBuffer> VulkanRecordCommandsVisitor::recordCommands(Scope<RenderCommandBuffer>&& commands)
    {
        startRecording();

        for (const auto command : commands->viewCommands())
            command->accept(*this);

        endRecording();

        return createScope<VulkanRecordedBuffer>(m_command_buffer);
    }

    void VulkanRecordCommandsVisitor::startRecording() const
    {
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.pNext = nullptr;
        begin_info.pInheritanceInfo = nullptr;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        const auto result = vkBeginCommandBuffer(m_command_buffer, &begin_info);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Failed to start recording VulkanCommandBuffer!");
    }

    void VulkanRecordCommandsVisitor::endRecording() const
    {
        const auto result = vkEndCommandBuffer(m_command_buffer);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Failed to finish recording VulkanCommandBuffer!");
    }

    //
    //  RenderPass Commands
    //

    void VulkanRecordCommandsVisitor::visit(BeginRenderPassCommand& command)
    {
        RenderPass& renderpass = command.renderpass;
        const ClearColor clear_color = renderpass.getClearColor();
        const auto& framebuffer_template = renderpass.viewFramebufferTemplate();

        std::vector<VkClearValue> clear_values(framebuffer_template->getAttachmentCount());
        for (uint32_t i = 0; i < framebuffer_template->getAttachmentCount(); ++i)
            clear_values[i].color = {clear_color.color.r, clear_color.color.g, clear_color.color.b, clear_color.color.a};
        if (framebuffer_template->hasDepthStencilAttachment())
            clear_values.back().depthStencil = {clear_color.depth_stencil.r, static_cast<uint32_t>(clear_color.depth_stencil.g)};

        VkRenderPassBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        begin_info.pNext = nullptr;
        begin_info.renderPass = static_cast<VkRenderPass>(renderpass.getRenderPassHandle());
        begin_info.renderArea.offset.x = command.render_area.x_offset;
        begin_info.renderArea.offset.y = command.render_area.y_offset;
        begin_info.renderArea.extent.width = command.render_area.width;
        begin_info.renderArea.extent.height = command.render_area.height;
        begin_info.framebuffer = static_cast<VkFramebuffer>(renderpass.getFramebufferHandle());

        //  Clear values
        begin_info.clearValueCount = clear_values.size();
        begin_info.pClearValues = clear_values.data();

        vkCmdBeginRenderPass(m_command_buffer, &begin_info, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VulkanRecordCommandsVisitor::visit(EndRenderPassCommand& command)
    {
        vkCmdEndRenderPass(m_command_buffer);
    }

    ////////////////////////////////////////////////////////////////////
    //////  VulkanExecuteCommandsVisitor  //////////////////////////////
    ////////////////////////////////////////////////////////////////////

    void VulkanExecuteCommandsVisitor::executeCommands(Scope<RecordedCommandBuffer>&& commands)
    {

    }

}
