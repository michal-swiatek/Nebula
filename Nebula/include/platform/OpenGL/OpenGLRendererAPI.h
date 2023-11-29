//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef OPENGLRENDERERAPI_H
#define OPENGLRENDERERAPI_H

#include "renderer/RendererAPI.h"

namespace nebula::rendering {

    class OpenGlRendererApi final : public RendererApi
    {
    public:
        void init() override;
        void shutdown() override;

        void compilePipeline(const GraphicsPipelineState& graphics_pipeline_state, void* renderpass_handle) override;
    };

}

#endif //OPENGLRENDERERAPI_H
