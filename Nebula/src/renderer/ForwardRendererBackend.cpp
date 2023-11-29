//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/renderer/ForwardRendererBackend.h"

namespace nebula::rendering {

    Scope<RenderCommandBuffer> ForwardRendererBackend::optimizeCommands(Scope<RenderCommandBuffer>&& render_commands)
    {
        return render_commands;
    }

}
