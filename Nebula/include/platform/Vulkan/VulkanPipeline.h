//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANPIPELINE_H
#define VULKANPIPELINE_H

#include "renderer/PipelineState.h"

#include "platform/Vulkan/VulkanAPI.h"

namespace nebula::rendering {

    VkGraphicsPipelineCreateInfo getGraphicsPipelineCreateInfo(const GraphicsPipelineState& graphics_pipeline_state, VkRenderPass renderpass_handle);

}

#endif //VULKANPIPELINE_H
