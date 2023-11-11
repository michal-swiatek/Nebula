//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "core/Types.h"
#include "renderer/RendererAPI.h"

namespace nebula {

    class RenderContext
    {
    public:
        virtual ~RenderContext() = default;

        virtual void swapBuffers() = 0;

    private:
        friend class WindowsWindow;
        static Scope<RenderContext> create(renderer::API api, void* window_handle);
    };

}

#endif //RENDERCONTEXT_H
