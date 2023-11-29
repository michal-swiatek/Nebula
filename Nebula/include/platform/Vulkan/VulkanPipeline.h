//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANPIPELINE_H
#define VULKANPIPELINE_H

#include <vector>

#include "renderer/PipelineState.h"
#include "platform/Vulkan/VulkanAPI.h"

namespace nebula::rendering {

    class VulkanGraphicsPipelineInfo
    {
    public:
        explicit VulkanGraphicsPipelineInfo(const GraphicsPipelineState& graphics_pipeline_state);

        VkGraphicsPipelineCreateInfo buildPipelineCreateInfo(VkRenderPass renderpass, uint32_t subpass) const;

    private:
        VkPipelineDynamicStateCreateInfo m_dynamic_state_create_info = {};
        VkPipelineVertexInputStateCreateInfo m_vertex_create_info = {};
        VkPipelineInputAssemblyStateCreateInfo m_assembly_create_info = {};
        VkPipelineViewportStateCreateInfo m_viewport_create_info = {};
        VkPipelineRasterizationStateCreateInfo m_rasterization_create_info = {};
        VkPipelineMultisampleStateCreateInfo m_multisampling_create_info = {};
        VkPipelineColorBlendAttachmentState m_color_blend_attachment = {};
        VkPipelineColorBlendStateCreateInfo m_color_blend_create_info = {};
        VkPipelineLayoutCreateInfo m_pipeline_layout_create_info = {};

        std::vector<VkDynamicState> m_dynamic_states = {};
        VkPipelineLayout m_pipeline_layout = {};
        uint32_t m_multisampling_mask = 0;
    };

    VkGraphicsPipelineCreateInfo getGraphicsPipelineCreateInfo(const GraphicsPipelineState& graphics_pipeline_state, VkRenderPass renderpass_handle, uint32_t subpass);

}

#endif //VULKANPIPELINE_H
