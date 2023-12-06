//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERERBACKEND_H
#define RENDERERBACKEND_H

#include "rendering/commands/RenderCommandBuffer.h"
#include "rendering/commands/RenderCommandVisitor.h"

namespace nebula::rendering {

    class NEBULA_API RendererBackend
    {
    public:
        explicit RendererBackend(bool submit_commands);
        virtual ~RendererBackend() = default;

        void processRenderCommands(Scope<RenderCommandBuffer>&& render_commands);
        [[nodiscard]] View<RenderCommandBuffer> viewCommandBuffer() const;

    protected:
        virtual Scope<RenderCommandBuffer> optimizeCommands(Scope<RenderCommandBuffer>&& render_commands) = 0;

    private:
        bool m_submit_commands;
        Scope<RenderCommandBuffer> m_optimized_commands = nullptr;
        Scope<RecordCommandVisitor> m_record_command_visitor = nullptr;

        void submitRenderCommands(Scope<RecordedCommandBuffer>&& render_commands);
    };

}

#endif //RENDERERBACKEND_H
