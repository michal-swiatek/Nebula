//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/renderer/RendererBackend.h"

#include "core/Application.h"

namespace nebula::rendering {

    void RendererBackend::processRenderCommands(Scope<RenderCommandBuffer>&& render_commands)
    {
        m_optimized_commands = optimizeCommands(std::move(render_commands));
    }

    Scope<RenderCommandBuffer> RendererBackend::getCommandBuffer()
    {
        return std::move(m_optimized_commands);
    }

}
