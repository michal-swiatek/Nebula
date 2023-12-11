//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef OPENGLRENDERERAPI_H
#define OPENGLRENDERERAPI_H

#include "rendering/renderer/RendererAPI.h"

namespace nebula::rendering {

    class OpenGlRendererApi final : public RendererApi
    {
    public:
        OpenGlRendererApi();
        ~OpenGlRendererApi() override;

        void compilePipelines(RenderPass& renderpass) override;
        void destroyPipeline(RenderPass& renderpass, uint32_t stage) override;
        void* getPipelineHandle(RenderPass& renderpass, uint32_t stage) override;
    };

}

#endif //OPENGLRENDERERAPI_H
