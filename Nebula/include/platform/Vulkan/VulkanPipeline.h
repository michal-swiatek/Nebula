//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANPIPELINE_H
#define VULKANPIPELINE_H

#include <vector>
#include <utility>
#include <unordered_map>

#include "rendering/PipelineState.h"
#include "rendering/PipelineStateCache.h"
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

    struct hash_pair
    {
        template <class T1, class T2>
        size_t operator()(const std::pair<T1, T2>& p) const
        {
            auto hash1 = std::hash<T1>{}(p.first);
            auto hash2 = std::hash<T2>{}(p.second);

            if (hash1 != hash2) {
                return hash1 ^ hash2;
            }

            // If hash1 == hash2, their XOR is zero.
            return hash1;
        }
    };

    class VulkanPipelineCache
    {
    public:
        explicit VulkanPipelineCache(const std::string& cache_root);
        ~VulkanPipelineCache();

        void addPipelines(VkRenderPass renderpass, std::vector<VkPipeline>&& pipelines);
        [[nodiscard]] VkPipelineCache getCache() const { return m_pipeline_cache; }

    private:
        using RenderPassID = std::pair<VkRenderPass, uint32_t>;

        std::unordered_map<RenderPassID, VkPipeline, hash_pair> m_handle_map{};
        VkPipelineCache m_pipeline_cache = VK_NULL_HANDLE;
        std::string m_cache_path;
    };

}

#endif //VULKANPIPELINE_H
