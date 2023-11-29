//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERERBACKEND_H
#define RENDERERBACKEND_H

#include "rendering/commands/RenderCommandBuffer.h"
#include "rendering/commands/RenderCommandVisitor.h"

namespace nebula::rendering {

    class Renderer;

    class NEBULA_API RendererBackend
    {
    public:
        RendererBackend();
        virtual ~RendererBackend() = default;

        void processRenderCommands(Scope<RenderCommandBuffer>&& render_commands);

    protected:
        virtual Scope<RenderCommandBuffer> optimizeCommands(Scope<RenderCommandBuffer>&& render_commands) = 0;

    private:
        void submitRenderCommands(Scope<RecordedCommandBuffer>&& render_commands);

        Scope<RecordCommandVisitor> m_record_command_visitor = nullptr;
    };

}

#endif //RENDERERBACKEND_H
