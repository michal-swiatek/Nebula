//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANCOMMANDSVISITOR_H
#define VULKANCOMMANDSVISITOR_H

#include "rendering/commands/RenderCommandVisitor.h"

namespace nebula::rendering {

    class VulkanRecordCommandsVisitor final : public RecordCommandVisitor
    {
    public:
        Scope<RecordedCommandBuffer> recordCommands(Scope<RenderCommandBuffer>&& commands) override;
    };

    class VulkanExecuteCommandsVisitor final : public ExecuteCommandVisitor
    {
    public:
        void executeCommands(Scope<RecordedCommandBuffer>&& commands) override;
    };

}

#endif //VULKANCOMMANDSVISITOR_H
