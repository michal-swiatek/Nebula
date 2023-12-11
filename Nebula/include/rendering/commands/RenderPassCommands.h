//
// Created by michal-swiatek on 09.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERPASSCOMMANDS_H
#define RENDERPASSCOMMANDS_H

#include <utility>

#include "RenderCommand.h"
#include "rendering/PipelineState.h"
#include "rendering/renderpass/RenderPass.h"

namespace nebula::rendering {

    struct NEBULA_API RenderArea
    {
        int32_t x_offset = 0;
        int32_t y_offset = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        float min_depth = 0.0f;
        float max_depth = 1.0f;
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

    struct NEBULA_API BindGraphicsPipelineCommand final : RenderCommand
    {
        RenderArea scissor;
        RenderArea viewport;
        void* graphics_pipeline_handle;
        GraphicsPipelineState graphics_pipeline_state;

        explicit BindGraphicsPipelineCommand(
            RenderArea scissor,
            RenderArea viewport,
            GraphicsPipelineState pipeline,
            void* pipeline_handle
        ) :
                scissor(scissor),
                viewport(viewport),
                graphics_pipeline_handle(pipeline_handle),
                graphics_pipeline_state(std::move(pipeline))
        {}

        void accept(RenderCommandVisitor& command_visitor) override { command_visitor.visit(*this); }
    };

    struct NEBULA_API DrawImGuiCommand final : RenderCommand
    {
        void accept(RenderCommandVisitor& command_visitor) override { command_visitor.visit(*this); }
    };

}

#endif //RENDERPASSCOMMANDS_H
