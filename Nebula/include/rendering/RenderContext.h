//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "core/Types.h"

namespace nebula {

    class Application;

    namespace rendering {

        class RenderContext
        {
        public:
            virtual ~RenderContext() = default;

            virtual void bind() = 0;
            virtual void unbind() = 0;

            virtual bool checkVSync() = 0;
            virtual void setVSync(bool vsync) = 0;

            virtual void swapBuffers() = 0;

        protected:
            RenderContext() = default;

        private:
            friend class nebula::Application;
            static Scope<RenderContext> create(void* window_handle);
        };

    }

}

#endif //RENDERCONTEXT_H