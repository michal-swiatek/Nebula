//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include "rendering/Framebuffer.h"
#include "rendering/RenderContext.h"

struct GLFWwindow;

namespace nebula::rendering {

    class OpenGLFramebufferTemplate final : public FramebufferTemplate
    {
    public:
        //  TODO: implement
        OpenGLFramebufferTemplate() : FramebufferTemplate(0, 0) {}
    };

    class OpenGLContext final : public RenderContext
    {
    public:
        explicit OpenGLContext(GLFWwindow* window_handle);

        void waitForFrameResources(uint32_t frame) override;

        void bind() override;
        void unbind() override;
        void reload() override;

        void presentImage() override;
        Reference<Framebuffer> getNextImage() override;

        Scope<ExecuteCommandVisitor> getCommandExecutor() override;

        [[nodiscard]] const Reference<FramebufferTemplate>& viewFramebufferTemplate() const override;

    private:
        GLFWwindow* m_window;
        bool m_vsync = true;

        Reference<FramebufferTemplate> m_framebuffer_template;
    };

}

#endif //OPENGLCONTEXT_H
