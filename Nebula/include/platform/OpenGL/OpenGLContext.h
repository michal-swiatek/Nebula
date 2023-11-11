//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include "renderer/RenderContext.h"

struct GLFWwindow;

namespace nebula::rendering {

    class OpenGLContext final : public RenderContext
    {
    public:
        explicit OpenGLContext(GLFWwindow* window_handle);

        void swapBuffers() override;

    private:
        GLFWwindow* m_window;
    };

}

#endif //OPENGLCONTEXT_H
