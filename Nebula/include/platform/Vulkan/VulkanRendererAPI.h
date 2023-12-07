//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANRENDERERAPI_H
#define VULKANRENDERERAPI_H

#include "rendering/renderer/RendererAPI.h"
#include "platform/Vulkan/VulkanPipeline.h"

namespace nebula::rendering {

    class VulkanRendererApi final : public RendererApi
    {
    public:
        VulkanRendererApi();
        void compilePipelines(RenderPass& renderpass) override;

    private:
        Scope<VulkanPipelineCache> m_pipeline_cache = nullptr;
    };

}

#endif //VULKANRENDERERAPI_H
