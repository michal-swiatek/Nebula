//
// Created by michal-swiatek on 13.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef FRAME_H
#define FRAME_H

#include "core/Types.h"
#include "RenderPass.h"

namespace nebula::rendering::impl {

    class Frame
    {
    public:
        explicit Frame(Scope<RenderPassTemplate>&& render_passes) : m_render_passes(std::move(render_passes)) {}

        [[nodiscard]] RenderPassTemplate& getRenderPasses() const { return *m_render_passes; }

    private:
        Scope<RenderPassTemplate> m_render_passes;
    };

}

#endif //FRAME_H
