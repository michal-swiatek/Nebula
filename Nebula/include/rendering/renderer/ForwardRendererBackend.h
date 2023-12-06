//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef FORWARDRENDERERBACKEND_H
#define FORWARDRENDERERBACKEND_H

#include "RendererBackend.h"

namespace nebula::rendering {

    class NEBULA_API ForwardRendererBackend final : public RendererBackend
    {
        using RendererBackend::RendererBackend;

    protected:
        Scope<RenderCommandBuffer> optimizeCommands(Scope<RenderCommandBuffer>&& render_commands) override;
    };

}

#endif //FORWARDRENDERERBACKEND_H
