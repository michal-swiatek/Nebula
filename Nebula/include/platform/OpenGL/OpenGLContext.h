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
        OpenGLFramebufferTemplate() : FramebufferTemplate(0, 0)
        {
            AttachmentDescription attachment_description;
            attachment_description.format = TextureFormat::cFormat_R8G8B8A8_FLOAT_NORM;
            attachment_description.final_layout = AttachmentLayout::cPresentOptimal;
            addTextureAttachment(attachment_description);
        }
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

        Reference<Framebuffer> m_framebuffer;
        Reference<FramebufferTemplate> m_framebuffer_template;
    };

}

#endif //OPENGLCONTEXT_H
