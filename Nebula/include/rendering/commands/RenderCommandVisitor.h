//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERCOMMANDVISITOR_H
#define RENDERCOMMANDVISITOR_H

#include "core/Core.h"

namespace nebula::rendering {

    class NEBULA_API RenderCommandVisitor
    {
    public:
        virtual ~RenderCommandVisitor() = default;

    protected:
    };

    class RenderCommandBuffer;
    class RecordedCommandBuffer;

    class RecordCommandVisitor : public RenderCommandVisitor
    {
    public:
        virtual Scope<RecordedCommandBuffer> recordCommands(Scope<RenderCommandBuffer>&& commands) = 0;
    };

    class ExecuteCommandVisitor : public RenderCommandVisitor
    {
    public:
        virtual void executeCommands(Scope<RecordedCommandBuffer>&& commands) = 0;
    };

}

#endif //RENDERCOMMANDVISITOR_H
