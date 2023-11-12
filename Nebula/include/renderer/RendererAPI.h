//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include <glm/glm.hpp>

#include "core/Types.h"

namespace nebula::rendering {

    enum class API
    {
        cUndefined = 0,
        cOpenGL = 1
    };

    enum ClearBufferType : uint8_t
    {
        cColorBuffer = 1,
        cDepthBuffer = 2,
        cStencilBuffer = 4
    };

    class Renderer;

    namespace impl {

        class RendererApi
        {
        public:
            virtual ~RendererApi() = default;

            virtual void init() = 0;

            virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

            virtual void setClearColor(const glm::vec4& color) = 0;
            virtual void clear(ClearBufferType flags) = 0;

        private:
            static View<RendererApi> create(API api);
            static void destroy(RendererApi* api);

            friend class nebula::rendering::Renderer;
        };

    }

}

#endif //RENDERERAPI_H
