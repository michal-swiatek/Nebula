//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanRendererAPI.h"

#include "platform/Vulkan/VulkanPipeline.h"

namespace nebula::rendering {

    void VulkanRendererApi::init()
    {

    }

    void VulkanRendererApi::shutdown()
    {

    }

    void VulkanRendererApi::compilePipeline(const GraphicsPipelineState& graphics_pipeline_state, void* renderpass_handle)
    {
        auto create_info = getGraphicsPipelineCreateInfo(graphics_pipeline_state, static_cast<VkRenderPass>(renderpass_handle));
    }

}
