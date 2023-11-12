//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef OPENGLRENDERERAPI_H
#define OPENGLRENDERERAPI_H

#include "renderer/RendererAPI.h"

namespace nebula::rendering::impl {

    class OpenGlRendererApi final : public RendererApi
    {
    public:
        void init() override;

        void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        void setClearColor(const glm::vec4& color) override;
        void clear(ClearBufferType flags) override;
    };

}

#endif //OPENGLRENDERERAPI_H
