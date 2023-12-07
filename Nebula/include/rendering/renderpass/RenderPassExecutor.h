//
// Created by michal-swiatek on 07.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERPASSEXECUTOR_H
#define RENDERPASSEXECUTOR_H

#include "core/Core.h"

#include "RenderPass.h"
#include "rendering/renderer/Renderer.h"
#include "rendering/renderpass/RenderPassObjects.h"
#include "rendering/commands/RenderCommandVisitor.h"

namespace nebula::rendering {

    class NEBULA_API RenderPassExecutor
    {
    public:
        explicit RenderPassExecutor(Scope<Renderer>&& renderer);
        explicit RenderPassExecutor(Scope<RenderPass>&& renderpass);

        void execute(const RenderPassObjects& renderpass_objects);
        Scope<RecordedCommandBuffer> getCommands();

        void setRenderer(Scope<Renderer>&& renderer);
        void setRecordCommandVisitor(Scope<RecordCommandVisitor>&& render_command_visitor);

    private:
        Scope<Renderer> m_renderer;
        Scope<RenderPass> m_renderpass;

        Scope<RecordCommandVisitor> m_record_command_visitor;
        Scope<RecordedCommandBuffer> m_recorded_command_buffer;
    };

}

#endif //RENDERPASSEXECUTOR_H
