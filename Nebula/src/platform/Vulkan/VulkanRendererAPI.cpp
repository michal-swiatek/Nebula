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

    void VulkanRendererApi::compilePipelines(RenderPass* renderpass)
    {
        const auto& render_stages = renderpass->viewRenderPassTemplate()->viewRenderStages();

        std::vector<VulkanGraphicsPipelineInfo> pipeline_infos{};
        std::vector<VkGraphicsPipelineCreateInfo> pipeline_create_infos{};
        pipeline_infos.reserve(render_stages.size());
        pipeline_create_infos.reserve(render_stages.size());

        for (int i = 0; i < render_stages.size(); ++i)
        {
            const auto& [graphics_pipeline_state, _] = render_stages[i];
            pipeline_infos.emplace_back(graphics_pipeline_state);

            auto create_info = pipeline_infos.back().buildPipelineCreateInfo(static_cast<VkRenderPass>(renderpass->getRenderPassHandle()), i);
            pipeline_create_infos.push_back(create_info);
        }

        //  TODO: Implement caching!!!
        //  TODO: Implement loading thread!!!
        std::vector<VkPipeline> graphic_pipelines(render_stages.size());
        auto result = vkCreateGraphicsPipelines(
            VulkanAPI::getDevice(),
            VK_NULL_HANDLE,
            pipeline_create_infos.size(),
            pipeline_create_infos.data(),
            nullptr,
            graphic_pipelines.data()
        );

        NB_CORE_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan GraphicsPipeline!");
    }

}
