//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include "RendererAPI.h"
#include "core/Types.h"

namespace nebula::rendering {

    class RenderCommand
    {
    public:
        virtual ~RenderCommand() = default;

        virtual void execute() = 0;

    protected:
        static View<impl::RendererApi> s_renderer_api;

        friend class Renderer;
    };

}

#endif //RENDERCOMMAND_H
