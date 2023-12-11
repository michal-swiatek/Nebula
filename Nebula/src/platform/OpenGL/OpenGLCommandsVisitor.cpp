//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/OpenGL/OpenGLCommandsVisitor.h"

#include <glad/glad.h>

#include "core/Application.h"
#include "debug/ImGuiLayer.h"
#include "platform/OpenGL/OpenGLFramebuffer.h"
#include "rendering/commands/RenderPassCommands.h"
#include "rendering/commands/RenderCommandBuffer.h"

namespace nebula::rendering {

    void OpenGlExecuteCommandsVisitor::executeCommands(Scope<RecordedCommandBuffer>&& commands)
    {
        for (const auto command : commands->viewCommands())
            command->accept(*this);
    }

    void OpenGlExecuteCommandsVisitor::submitCommands()
    {

    }

    void OpenGlExecuteCommandsVisitor::visit(BeginRenderPassCommand& command)
    {
        auto clear_color = command.renderpass.getClearColor();
        auto viewport = command.render_area;
        auto* framebuffer = static_cast<OpenGlFramebuffer*>(command.renderpass.getFramebufferHandle());

        glViewport(viewport.x_offset, viewport.y_offset, viewport.width, viewport.height);
        framebuffer->bind();

        glClearColor(clear_color.color.r, clear_color.color.g, clear_color.color.b, clear_color.color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGlExecuteCommandsVisitor::visit(EndRenderPassCommand& command)
    {
        auto* framebuffer = static_cast<OpenGlFramebuffer*>(command.renderpass.getFramebufferHandle());
        framebuffer->unbind();
    }

    void OpenGlExecuteCommandsVisitor::visit(DrawImGuiCommand& command)
    {
        if (Application::get().closed())
            return;

        ImGuiLayer::begin();

        for (const auto& layer : Application::get().m_layer_stack)
            layer->onImGuiRender();

        ImGuiLayer::end(nullptr);
    }

}
