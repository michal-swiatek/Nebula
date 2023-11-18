//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include "core/Types.h"

namespace nebula::rendering {

    class RendererApi;

    enum RenderCommandCategory : uint8_t
    {
        cClear = 1,
        cDraw = 2,
        cConfigure = 4,
        cUnknown = 8
    };

    class NEBULA_API RenderCommand
    {
    public:
        RenderCommand() = default;
        virtual ~RenderCommand() = default;

        virtual void execute() = 0;
        [[nodiscard]] virtual RenderCommandCategory getCategory() const = 0;

    protected:
        static View<RendererApi> s_renderer_api;

        friend class Renderer;
    };

    #define RENDER_COMMAND_CLASS_CATEGORY(category) static RenderCommandCategory getStaticCategory() { return (category); }\
    [[nodiscard]] RenderCommandCategory getCategory() const override { return category; }

}

#endif //RENDERCOMMAND_H
