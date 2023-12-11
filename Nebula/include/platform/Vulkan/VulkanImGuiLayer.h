//
// Created by michal-swiatek on 11.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANIMGUILAYER_H
#define VULKANIMGUILAYER_H

#include "debug/ImGuiLayer.h"

namespace nebula {

    class VulkanImGuiLayer final : public ImGuiLayer
    {
    public:
        explicit VulkanImGuiLayer(rendering::RenderPass& renderpass);

    private:
        void apiSection() override;
    };

}

#endif //VULKANIMGUILAYER_H
