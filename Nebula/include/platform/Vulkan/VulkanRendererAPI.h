//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANRENDERERAPI_H
#define VULKANRENDERERAPI_H

#include "renderer/RendererAPI.h"

namespace nebula::rendering {

    class VulkanRendererApi final : public RendererApi
    {
    public:
        void init() override;
        void shutdown() override;

        void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        void setClearColor(const glm::vec4& color) override;
        void clear(ClearBufferType flags) override;
    };

}

#endif //VULKANRENDERERAPI_H
