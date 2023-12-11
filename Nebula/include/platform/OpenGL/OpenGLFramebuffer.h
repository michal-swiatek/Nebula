//
// Created by michal-swiatek on 24.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H

#include <glad/glad.h>
#include "rendering/Framebuffer.h"

#include "core/Types.h"

namespace nebula::rendering {

    class NEBULA_API OpenGlFramebuffer : public Framebuffer
    {
    public:
        explicit OpenGlFramebuffer(const Reference<FramebufferTemplate>& framebuffer_template);
        ~OpenGlFramebuffer() override;

        void bind() override;
        void unbind() override;

        [[nodiscard]] bool attached() const override;
        void attachTo(void* renderpass_handle) override;

        void* getFramebufferHandle() override;

        [[nodiscard]] const Reference<FramebufferTemplate>& viewFramebufferTemplate() const override;

    protected:
        GLuint m_id = 0;
        bool m_attached = false;

        Reference<FramebufferTemplate> m_framebuffer_template = nullptr;
    };

    class OpenGlDefaultFramebuffer final : public OpenGlFramebuffer
    {
    public:
        explicit OpenGlDefaultFramebuffer(const Reference<FramebufferTemplate>& framebuffer_template);
    };

}

#endif //OPENGLFRAMEBUFFER_H
