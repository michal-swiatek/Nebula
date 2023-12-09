//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RECORDCOMMANDSVISITOR_H
#define RECORDCOMMANDSVISITOR_H

#include "rendering/commands/RenderCommandVisitor.h"

namespace nebula::rendering {

    class OpenGlRecordCommandsVisitor final : public RecordCommandVisitor
    {
    public:
        Scope<RecordedCommandBuffer> recordCommands(Scope<RenderCommandBuffer>&& commands) override;
    };

    class OpenGlExecuteCommandsVisitor final : public ExecuteCommandVisitor
    {
    public:
        void executeCommands(Scope<RecordedCommandBuffer>&& commands) override;
        void submitCommands() override;
    };

}

#endif //RECORDCOMMANDSVISITOR_H
