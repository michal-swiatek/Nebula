//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERER_H
#define RENDERER_H

#include "RendererAPI.h"
#include "core/Types.h"

namespace nebula {

    class Application;

    namespace rendering {

        class Renderer
        {
        public:
            virtual ~Renderer() = default;

        protected:
            static View<impl::RendererApi> s_renderer_api;

        private:
            static void init(API api);
            static void shutdown();

            static void setRenderingApi(API api);

            friend class nebula::Application;
        };

    }

}

#endif //RENDERER_H
