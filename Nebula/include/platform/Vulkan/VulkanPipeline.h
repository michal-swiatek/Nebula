//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANPIPELINE_H
#define VULKANPIPELINE_H

#include <vector>

#include "rendering/PipelineState.h"
#include "platform/Vulkan/VulkanAPI.h"

namespace nebula::rendering {

    class VulkanGraphicsPipelineInfo
    {
    public:
        explicit VulkanGraphicsPipelineInfo(const GraphicsPipelineState& graphics_pipeline_state);
        ~VulkanGraphicsPipelineInfo();

        VkGraphicsPipelineCreateInfo buildPipelineCreateInfo(VkRenderPass renderpass, uint32_t subpass);

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

        uint32_t m_multisampling_mask = 0;
        VkPipelineLayout m_pipeline_layout = {};

        std::vector<VkDynamicState> m_dynamic_states = {};
        std::vector<VkPipelineShaderStageCreateInfo> m_shader_stages = {};

        void loadVertexShader(const Reference<Shader>& shader, const VertexShader& shader_template);
    };

    VkGraphicsPipelineCreateInfo getGraphicsPipelineCreateInfo(const GraphicsPipelineState& graphics_pipeline_state, VkRenderPass renderpass_handle, uint32_t subpass);

}

#endif //VULKANPIPELINE_H
