//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanRendererAPI.h"

#include "core/Config.h"
#include "core/Application.h"
#include "utility/Filesystem.h"
#include "platform/Vulkan/VulkanPipeline.h"

namespace nebula::rendering {

    VulkanRendererApi::VulkanRendererApi()
    {
        auto& engine_config = Config::getEngineConfig();
        const auto rendering_cache_path = engine_config["rendering"]["cache_path"].as<std::string>();

        auto pipeline_cache_root = Application::getResourcesPath(true) / rendering_cache_path;
        m_pipeline_cache = createScope<VulkanPipelineCache>(pipeline_cache_root.make_preferred().string());
    }

    void VulkanRendererApi::compilePipelines(RenderPass& renderpass)
    {
        const auto& render_stages = renderpass.viewRenderPassTemplate()->viewRenderStages();
        const auto renderpass_handle = static_cast<VkRenderPass>(renderpass.getRenderPassHandle());

        std::vector<VulkanGraphicsPipelineInfo> pipeline_infos{};
        std::vector<VkGraphicsPipelineCreateInfo> pipeline_create_infos{};
        pipeline_infos.reserve(render_stages.size());
        pipeline_create_infos.reserve(render_stages.size());

        for (int i = 0; i < render_stages.size(); ++i)
        {
            const auto& [graphics_pipeline_state, _] = render_stages[i];
            pipeline_infos.emplace_back(graphics_pipeline_state);

            auto create_info = pipeline_infos.back().buildPipelineCreateInfo(renderpass_handle, i);
            pipeline_create_infos.push_back(create_info);
        }

        //  TODO: Implement loading thread!!!
        std::vector<VkPipeline> graphic_pipelines(render_stages.size());
        const auto result = vkCreateGraphicsPipelines(
            VulkanAPI::getDevice(),
            m_pipeline_cache->getCache(),
            pipeline_create_infos.size(),
            pipeline_create_infos.data(),
            nullptr,
            graphic_pipelines.data()
        );

        NB_CORE_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan GraphicsPipelines!");
        m_pipeline_cache->addPipelines(renderpass_handle, std::move(graphic_pipelines));
    }

    void* VulkanRendererApi::getPipelineHandle(RenderPass& renderpass, uint32_t stage)
    {
        return m_pipeline_cache->getPipeline(static_cast<VkRenderPass>(renderpass.getRenderPassHandle()), stage);
    }

}
