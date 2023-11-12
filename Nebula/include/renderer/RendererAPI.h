//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include "core/Types.h"

namespace nebula::rendering {

    enum class API
    {
        cUndefined = 0,
        cOpenGL = 1
    };

    namespace impl {

        class RendererApi
        {
        public:
            virtual ~RendererApi() = default;

            static View<RendererApi> create(API api);
            static void destroy(RendererApi* api);
        };

    }

}

#endif //RENDERERAPI_H
