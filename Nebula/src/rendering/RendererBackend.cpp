//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/renderer/RendererBackend.h"

#include "platform/Vulkan/VulkanCommandsVisitor.h"
#include "platform/OpenGL/OpenGLCommandsVisitor.h"

#include "core/Assert.h"
#include "core/Application.h"

namespace nebula::rendering {

    RendererBackend::RendererBackend(const bool submit_commands) : m_submit_commands(submit_commands)
    {
        switch (Application::get().getRenderingAPI())
        {
            case API::cVulkan:  m_record_command_visitor = createScope<VulkanRecordCommandsVisitor>();  break;
            case API::cOpenGL:  m_record_command_visitor = createScope<OpenGlRecordCommandsVisitor>();  break;
            default:            NB_CORE_ASSERT(false, "Unknown rendering API!");
        }
    }

    void RendererBackend::processRenderCommands(Scope<RenderCommandBuffer>&& render_commands)
    {
        //  Customizable pipeline
        Scope<RenderCommandBuffer> optimized_commands = optimizeCommands(std::move(render_commands));

        //  Fixed pipeline
        if (m_submit_commands)
        {
            Scope<RecordedCommandBuffer> recorded_commands = m_record_command_visitor->recordCommands(std::move(optimized_commands));
            submitRenderCommands(std::move(recorded_commands));
        }
        else
            m_optimized_commands = std::move(optimized_commands);
    }

    View<RenderCommandBuffer> RendererBackend::viewCommandBuffer() const
    {
        NB_CORE_ASSERT(!m_submit_commands, "Cannot view RenderCommands submitted to MainRenderThread!");
        return m_optimized_commands.get();
    }

    void RendererBackend::submitRenderCommands(Scope<RecordedCommandBuffer>&& render_commands)
    {
        //  TODO: Implement!!!
    }

}
