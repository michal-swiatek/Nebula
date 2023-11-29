//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANRENDERERAPI_H
#define VULKANRENDERERAPI_H

#include "renderer/RendererAPI.h"

namespace nebula::rendering {

    struct GraphicsPipelineState;

    class VulkanRendererApi final : public RendererApi
    {
    public:
        void init() override;
        void shutdown() override;

        void compilePipeline(const GraphicsPipelineState& graphics_pipeline_state, void* renderpass_handle) override;
    };

}

#endif //VULKANRENDERERAPI_H
