//
// Created by michal-swiatek on 13.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERPASSTEMPLATES_H
#define RENDERPASSTEMPLATES_H

#include "renderer/RenderPass.h"

namespace nebula::rendering {

    class DefaultRenderPass final : public RenderPassTemplate
    {
    public:
        DefaultRenderPass()
        {
            addPass<RenderPass>();
        }
    };

}

#endif //RENDERPASSTEMPLATES_H
