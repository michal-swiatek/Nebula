//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERERBACKEND_H
#define RENDERERBACKEND_H

#include "rendering/commands/RenderCommandBuffer.h"

namespace nebula::rendering {

    class NEBULA_API RendererBackend
    {
    public:
        virtual ~RendererBackend() = default;

        void processRenderCommands(Scope<RenderCommandBuffer>&& render_commands);
        [[nodiscard]] Scope<RenderCommandBuffer> getCommandBuffer();

    protected:
        virtual Scope<RenderCommandBuffer> optimizeCommands(Scope<RenderCommandBuffer>&& render_commands) = 0;

    private:
        Scope<RenderCommandBuffer> m_optimized_commands = nullptr;
    };

}

#endif //RENDERERBACKEND_H
