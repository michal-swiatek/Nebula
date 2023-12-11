//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanCommandsVisitor.h"

#include <rendering/commands/DrawRenderCommands.h>
#include <rendering/commands/RenderPassCommands.h>

#include "core/Application.h"
#include "debug/ImGuiLayer.h"
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

    void VulkanRecordCommandsVisitor::visit(BindGraphicsPipelineCommand& command)
    {
        VkPipeline graphics_pipeline = static_cast<VkPipeline>(command.graphics_pipeline_handle);
        vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

        VkViewport viewport = {};
        viewport.x = static_cast<float>(command.viewport.x_offset);
        viewport.y = static_cast<float>(command.viewport.y_offset);
        viewport.width = static_cast<float>(command.viewport.width);
        viewport.height = static_cast<float>(command.viewport.height);
        viewport.minDepth = command.viewport.min_depth;
        viewport.maxDepth = command.viewport.max_depth;
        vkCmdSetViewport(m_command_buffer, 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.offset.x = command.scissor.x_offset;
        scissor.offset.y = command.scissor.x_offset;
        scissor.extent.width = command.scissor.width;
        scissor.extent.height = command.scissor.height;
        vkCmdSetScissor(m_command_buffer, 0, 1, &scissor);
    }

    void VulkanRecordCommandsVisitor::visit(DrawImGuiCommand& command)
    {
        if (Application::get().closed())
            return;

        ImGuiLayer::begin();

        for (const auto& layer : Application::get().m_layer_stack)
            layer->onImGuiRender();

        ImGuiLayer::end(m_command_buffer);
    }

    //
    //  Draw Commands
    //

    void VulkanRecordCommandsVisitor::visit(DrawDummyIndicesCommand& command)
    {
        vkCmdDraw(m_command_buffer, command.num_indices, 1, 0, 0);
    }

    ////////////////////////////////////////////////////////////////////
    //////  VulkanExecuteCommandsVisitor  //////////////////////////////
    ////////////////////////////////////////////////////////////////////

    VulkanExecuteCommandsVisitor::VulkanExecuteCommandsVisitor(VulkanFrameSynchronization& frame_synchronization) :
            m_frame_synchronization(frame_synchronization)
    {}

    void VulkanExecuteCommandsVisitor::executeCommands(Scope<RecordedCommandBuffer>&& commands)
    {
        m_vulkan_commands.push_back(static_cast<VkCommandBuffer>(commands->getBufferHandle()));
    }

    void VulkanExecuteCommandsVisitor::submitCommands()
    {
        VkSubmitInfo submit_info = {};
        constexpr VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.pNext = nullptr;
        submit_info.pWaitDstStageMask = &wait_stage;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &m_frame_synchronization.image_available;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &m_frame_synchronization.render_finished;
        submit_info.commandBufferCount = m_vulkan_commands.size();
        submit_info.pCommandBuffers = m_vulkan_commands.data();

        vkResetFences(VulkanAPI::getDevice(), 1, &m_frame_synchronization.frame_resources_free);

        const auto queues_info = VulkanAPI::getQueuesInfo();
        const auto result = vkQueueSubmit(queues_info.graphics_queue, 1, &submit_info, m_frame_synchronization.frame_resources_free);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Failed submitting render commands!");
    }

}
