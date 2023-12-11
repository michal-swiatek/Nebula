//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanPipeline.h"

#include "core/Logging.h"
#include "utility/Filesystem.h"
#include "platform/EngineConfiguration.h"
#include "platform/Vulkan/VulkanConfiguration.h"
#include "platform/Vulkan/VulkanTextureFormats.h"

namespace nebula::rendering {

    std::vector<VkDynamicState> getVulkanDynamicState(const DynamicState dynamic_state_flags)
    {
        static std::vector s_dynamic_state_flags = {cViewport, cScissor, cLineWidth, cDepthBias, cStencilMask, cBlendConstant};
        std::vector<VkDynamicState> dynamic_state;
        for (const DynamicState flag : s_dynamic_state_flags)
        {
            if (dynamic_state_flags & flag)
            {
                switch (flag)
                {
                    case cViewport:         dynamic_state.push_back(VK_DYNAMIC_STATE_VIEWPORT);         break;
                    case cScissor:          dynamic_state.push_back(VK_DYNAMIC_STATE_SCISSOR);          break;
                    case cLineWidth:        dynamic_state.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);       break;
                    case cDepthBias:        dynamic_state.push_back(VK_DYNAMIC_STATE_DEPTH_BIAS);       break;
                    case cBlendConstant:    dynamic_state.push_back(VK_DYNAMIC_STATE_BLEND_CONSTANTS);  break;
                    case cStencilMask:
                        dynamic_state.insert(dynamic_state.end(), {VK_DYNAMIC_STATE_STENCIL_WRITE_MASK, VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK});    break;
                }
            }
        }

        return dynamic_state;
    }

    VkPrimitiveTopology getVulkanTopology(const GeometryTopology topology)
    {
        switch (topology)
        {
            case GeometryTopology::cPointList:      return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
            case GeometryTopology::cLineList:       return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            case GeometryTopology::cLineStrip:      return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
            case GeometryTopology::cTriangleList:   return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            case GeometryTopology::cTriangleStrip:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
            case GeometryTopology::cTriangleFan:    return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        }

        return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
    }

    VkPolygonMode getVulkanPolygonMode(const PolygonMode polygon_mode)
    {
        switch (polygon_mode)
        {
            case PolygonMode::cFill:    return VK_POLYGON_MODE_FILL;
            case PolygonMode::cLine:    return VK_POLYGON_MODE_LINE;
            case PolygonMode::cPoint:   return VK_POLYGON_MODE_POINT;
        }

        return VK_POLYGON_MODE_MAX_ENUM;
    }

    VkCullModeFlags getVulkanCullMode(const CullMode cull_mode)
    {
        switch (cull_mode)
        {
            case CullMode::cNone:           return VK_CULL_MODE_NONE;
            case CullMode::cBack:           return VK_CULL_MODE_BACK_BIT;
            case CullMode::cFront:          return VK_CULL_MODE_FRONT_BIT;
            case CullMode::cBackAndFront:   return VK_CULL_MODE_FRONT_AND_BACK;
        }

        return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
    }

    VkFrontFace getVulkanFrontFace(const bool clockwise)
    {
        if (clockwise)
            return VK_FRONT_FACE_CLOCKWISE;
        return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }

    VulkanGraphicsPipelineInfo::VulkanGraphicsPipelineInfo(const GraphicsPipelineState& graphics_pipeline_state)
    {
        //  DynamicState
        m_dynamic_states = getVulkanDynamicState(graphics_pipeline_state.dynamic_state_flags);
        m_dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        m_dynamic_state_create_info.dynamicStateCount = static_cast<uint32_t>(m_dynamic_states.size());
        m_dynamic_state_create_info.pDynamicStates = m_dynamic_states.data();

        //  VertexInput
        m_vertex_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        m_vertex_create_info.vertexBindingDescriptionCount = 0;
        m_vertex_create_info.pVertexBindingDescriptions = nullptr; // Optional
        m_vertex_create_info.vertexAttributeDescriptionCount = 0;
        m_vertex_create_info.pVertexAttributeDescriptions = nullptr; // Optional

        //  InputAssembly
        m_assembly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        m_assembly_create_info.topology = getVulkanTopology(graphics_pipeline_state.input_assembly.topology);
        m_assembly_create_info.primitiveRestartEnable = graphics_pipeline_state.input_assembly.strip_restart;

        //  Viewport   TODO: Add multiple viewports
        m_viewport_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        m_viewport_create_info.viewportCount = 1;
        m_viewport_create_info.scissorCount = 1;

        //  Rasterization
        m_rasterization_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        m_rasterization_create_info.rasterizerDiscardEnable = !graphics_pipeline_state.rasterization.enabled;
        m_rasterization_create_info.polygonMode = getVulkanPolygonMode(graphics_pipeline_state.rasterization.polygon_mode);
        m_rasterization_create_info.cullMode = getVulkanCullMode(graphics_pipeline_state.rasterization.cull_mode);
        m_rasterization_create_info.frontFace = getVulkanFrontFace(graphics_pipeline_state.rasterization.face_clockwise);
        m_rasterization_create_info.lineWidth = graphics_pipeline_state.rasterization.line_width;
        m_rasterization_create_info.depthClampEnable = graphics_pipeline_state.depth_stencil.depth_clamp;
        m_rasterization_create_info.depthBiasEnable = graphics_pipeline_state.depth_stencil.depth_bias_enabled;
        m_rasterization_create_info.depthBiasConstantFactor = graphics_pipeline_state.depth_stencil.depth_bias_constant;
        m_rasterization_create_info.depthBiasClamp = graphics_pipeline_state.depth_stencil.depth_bias_clamp;
        m_rasterization_create_info.depthBiasSlopeFactor = graphics_pipeline_state.depth_stencil.depth_bias_slope;

        //  Multisampling
        m_multisampling_mask = graphics_pipeline_state.multisampling.sample_mask;
        m_multisampling_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        m_multisampling_create_info.sampleShadingEnable = graphics_pipeline_state.multisampling.enabled;
        m_multisampling_create_info.rasterizationSamples = getVulkanTextureSampling(graphics_pipeline_state.multisampling.samples);
        m_multisampling_create_info.minSampleShading = graphics_pipeline_state.multisampling.min_sample_shading;
        m_multisampling_create_info.pSampleMask = nullptr;
        m_multisampling_create_info.alphaToOneEnable = graphics_pipeline_state.multisampling.alpha_to_one_enable;
        m_multisampling_create_info.alphaToCoverageEnable = graphics_pipeline_state.multisampling.alpha_to_coverage_enable;

        //  DepthStencil    TODO: Implement!!!

        //  ColorBlend      TODO: Implement!!!
        m_color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        m_color_blend_attachment.blendEnable = graphics_pipeline_state.color_blending.enabled;
        m_color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        m_color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        m_color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        m_color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        m_color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        m_color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        m_color_blend_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        m_color_blend_create_info.logicOpEnable = VK_FALSE;
        m_color_blend_create_info.logicOp = VK_LOGIC_OP_COPY; // Optional
        m_color_blend_create_info.attachmentCount = 1;
        m_color_blend_create_info.pAttachments = &m_color_blend_attachment;
        m_color_blend_create_info.blendConstants[0] = 0.0f; // Optional
        m_color_blend_create_info.blendConstants[1] = 0.0f; // Optional
        m_color_blend_create_info.blendConstants[2] = 0.0f; // Optional
        m_color_blend_create_info.blendConstants[3] = 0.0f; // Optional

        //  Shader
        const auto& shader_template = graphics_pipeline_state.shader->getTemplate();
        if (std::holds_alternative<VertexShader>(shader_template))
            loadVertexShader(graphics_pipeline_state.shader, std::get<VertexShader>(shader_template));

        //  PipelineLayout  TODO: Implement!!!
        m_pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        m_pipeline_layout_create_info.setLayoutCount = 0; // Optional
        m_pipeline_layout_create_info.pSetLayouts = nullptr; // Optional
        m_pipeline_layout_create_info.pushConstantRangeCount = 0; // Optional
        m_pipeline_layout_create_info.pPushConstantRanges = nullptr; // Optional

        const auto result = vkCreatePipelineLayout(VulkanAPI::getDevice(), &m_pipeline_layout_create_info, nullptr, &m_pipeline_layout);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan PipelineLayout!");
    }

    VulkanGraphicsPipelineInfo::~VulkanGraphicsPipelineInfo()
    {
        vkDestroyPipelineLayout(VulkanAPI::getDevice(), m_pipeline_layout, nullptr);
    }

    VkGraphicsPipelineCreateInfo VulkanGraphicsPipelineInfo::buildPipelineCreateInfo(VkRenderPass renderpass, const uint32_t subpass)
    {
        VkGraphicsPipelineCreateInfo pipeline_create_info = {};
        pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

        pipeline_create_info.pVertexInputState = &m_vertex_create_info;
        pipeline_create_info.pInputAssemblyState = &m_assembly_create_info;
        pipeline_create_info.pViewportState = &m_viewport_create_info;
        pipeline_create_info.pRasterizationState = &m_rasterization_create_info;
        pipeline_create_info.pMultisampleState = &m_multisampling_create_info;
        pipeline_create_info.pDepthStencilState = nullptr;
        pipeline_create_info.pColorBlendState = &m_color_blend_create_info;
        pipeline_create_info.pDynamicState = &m_dynamic_state_create_info;

        //  Shaders
        pipeline_create_info.layout = m_pipeline_layout;
        pipeline_create_info.stageCount = m_shader_stages.size();
        pipeline_create_info.pStages = m_shader_stages.data();

        pipeline_create_info.renderPass = renderpass;
        pipeline_create_info.subpass = subpass;

        //  TODO: Look into
        pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipeline_create_info.basePipelineIndex = -1; // Optional

        return pipeline_create_info;
    }

    void VulkanGraphicsPipelineInfo::loadVertexShader(View<Shader> shader, const VertexShader& shader_template)
    {
        VkPipelineShaderStageCreateInfo vertex_stage_info = {};
        vertex_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertex_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertex_stage_info.module = static_cast<VkShaderModule>(shader.get()->getStageHandle(ShaderStage::cVertex));
        vertex_stage_info.pName = "main";
        vertex_stage_info.pSpecializationInfo = nullptr;

        m_shader_stages.emplace_back(vertex_stage_info);

        VkPipelineShaderStageCreateInfo fragment_stage_info = {};
        fragment_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragment_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragment_stage_info.module = static_cast<VkShaderModule>(shader.get()->getStageHandle(ShaderStage::cFragment));
        fragment_stage_info.pName = "main";
        fragment_stage_info.pSpecializationInfo = nullptr;

        m_shader_stages.emplace_back(fragment_stage_info);
    }

    VulkanPipelineCache::VulkanPipelineCache(const std::string& cache_root) :
            m_cache_path(filesystem::createPath(cache_root, VULKAN_PIPELINE_CACHE_FILE).string())
    {
        std::vector<std::byte> pipeline_cache_data;
        if (filesystem::checkFile(m_cache_path))
            pipeline_cache_data = filesystem::readBinaryFile(m_cache_path);
        else if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= NEBULA_INITIALIZATION_VERBOSITY_LOW)
            NB_CORE_WARN("Unable to load Vulkan pipeline cache from \"{}\"", m_cache_path);

        VkPipelineCacheCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        create_info.initialDataSize = pipeline_cache_data.size();
        create_info.pInitialData = pipeline_cache_data.data();

        vkCreatePipelineCache(VulkanAPI::getDevice(), &create_info, nullptr, &m_pipeline_cache);
    }

    VulkanPipelineCache::~VulkanPipelineCache()
    {
        for (auto& [_, pipeline] : m_handle_map)
            vkDestroyPipeline(VulkanAPI::getDevice(), pipeline, nullptr);

        std::size_t cache_size;
        vkGetPipelineCacheData(VulkanAPI::getDevice(), m_pipeline_cache, &cache_size, nullptr);

        std::vector<std::byte> data(cache_size);
        vkGetPipelineCacheData(VulkanAPI::getDevice(), m_pipeline_cache, &cache_size, data.data());

        if (!m_cache_path.empty())
            filesystem::saveBinaryFile(m_cache_path, data);

        vkDestroyPipelineCache(VulkanAPI::getDevice(), m_pipeline_cache, nullptr);
    }

    void VulkanPipelineCache::destroyPipeline(VkRenderPass renderpass, uint32_t subpass)
    {
        const auto key = std::make_pair(renderpass, subpass);
        const auto pipeline = m_handle_map.at(key);
        vkDestroyPipeline(VulkanAPI::getDevice(), pipeline, nullptr);
        m_handle_map.erase(key);
    }

    VkPipeline VulkanPipelineCache::getPipeline(VkRenderPass renderpass, uint32_t subpass) const
    {
        return m_handle_map.at(std::make_pair(renderpass, subpass));
    }

    void VulkanPipelineCache::addPipelines(VkRenderPass renderpass, std::vector<VkPipeline>&& pipelines)
    {
        for (uint32_t i = 0; i < pipelines.size(); ++i)
        {
            const auto key = std::make_pair(renderpass, i);
            if (!m_handle_map.contains(key))
                m_handle_map.insert(std::make_pair(key, pipelines[i]));
        }
    }

}
