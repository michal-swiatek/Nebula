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

    RendererBackend::RendererBackend()
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
        Scope<RecordedCommandBuffer> recorded_commands = m_record_command_visitor->recordCommands(std::move(optimized_commands));
        submitRenderCommands(std::move(recorded_commands));
    }

    void RendererBackend::submitRenderCommands(Scope<RecordedCommandBuffer>&& render_commands)
    {
        //  TODO: Implement!!!
    }

}
