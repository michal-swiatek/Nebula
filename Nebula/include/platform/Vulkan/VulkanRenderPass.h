//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANRENDERPASS_H
#define VULKANRENDERPASS_H

#include "rendering/renderpass/RenderPass.h"

#include <vulkan/vulkan.h>

namespace nebula::rendering {

    class VulkanRenderPass final : public RenderPass
    {
    public:
        explicit VulkanRenderPass(const Reference<RenderPassTemplate>& renderpass_template);
        ~VulkanRenderPass() override;

        void* getRenderPassHandle() override;

    private:
        VkRenderPass m_renderpass = nullptr;
    };

}

#endif //VULKANRENDERPASS_H
