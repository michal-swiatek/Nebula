//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/OpenGL/OpenGLCommandsVisitor.h"

#include "rendering/commands/RenderCommandBuffer.h"

namespace nebula::rendering {

    Scope<RecordedCommandBuffer> OpenGlRecordCommandsVisitor::recordCommands(Scope<RenderCommandBuffer>&& commands)
    {
        return std::move(commands);
    }

    void OpenGlExecuteCommandsVisitor::executeCommands(Scope<RecordedCommandBuffer>&& commands)
    {

    }

}
