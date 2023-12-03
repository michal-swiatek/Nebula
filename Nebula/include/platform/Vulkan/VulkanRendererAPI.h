//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANRENDERERAPI_H
#define VULKANRENDERERAPI_H

#include "rendering/renderer/RendererAPI.h"

namespace nebula::rendering {

    struct GraphicsPipelineState;

    class VulkanRendererApi final : public RendererApi
    {
    public:
        void init() override;
        void shutdown() override;

        void compilePipelines(RenderPass* renderpass) override;
    };

}

#endif //VULKANRENDERERAPI_H
