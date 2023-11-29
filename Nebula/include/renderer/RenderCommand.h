//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include "core/Types.h"

namespace nebula::rendering {

    class RendererApi;

    struct NEBULA_API RenderCommand
    {
        virtual ~RenderCommand() = default;

        virtual void execute(void* command_buffer_handle) = 0;

    protected:
        static View<RendererApi> s_renderer_api;
        friend class nebula::rendering::RendererApi;
    };

}

#endif //RENDERCOMMAND_H
