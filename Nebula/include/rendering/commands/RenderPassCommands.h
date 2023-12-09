//
// Created by michal-swiatek on 09.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERPASSCOMMANDS_H
#define RENDERPASSCOMMANDS_H

#include "RenderCommand.h"
#include "RenderCommandVisitor.h"
#include "rendering/renderpass/RenderPass.h"

namespace nebula::rendering {

    struct NEBULA_API RenderArea
    {
        int32_t x_offset = 0;
        int32_t y_offset = 0;
        uint32_t width = 0;
        uint32_t height = 0;
    };

    struct NEBULA_API BeginRenderPassCommand final : RenderCommand
    {
        RenderPass& renderpass;
        RenderArea render_area;

        BeginRenderPassCommand(RenderPass& renderpass, const RenderArea& render_area) : renderpass(renderpass), render_area(render_area) {}
        void accept(RenderCommandVisitor& command_visitor) override { command_visitor.visit(*this); }
    };

    struct NEBULA_API EndRenderPassCommand final : RenderCommand
    {
        RenderPass& renderpass;

        explicit EndRenderPassCommand(RenderPass& renderpass) : renderpass(renderpass) {}
        void accept(RenderCommandVisitor& command_visitor) override { command_visitor.visit(*this); }
    };

}

#endif //RENDERPASSCOMMANDS_H
