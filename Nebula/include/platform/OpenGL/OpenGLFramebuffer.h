//
// Created by michal-swiatek on 24.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H

#include "renderer/Framebuffer.h"

#include "core/Types.h"

namespace nebula::rendering {

    class NEBULA_API OpenGlFramebuffer final : public Framebuffer
    {
    public:
        explicit OpenGlFramebuffer(const Reference<FramebufferTemplate>& framebuffer_template);
        ~OpenGlFramebuffer() override;

        void bind() override;
        void unbind() override;

        [[nodiscard]] bool attached() const override;
        void attachTo(void* renderpass_handle) override;

        [[nodiscard]] const Reference<FramebufferTemplate>& getFramebufferTemplate() const override;

    private:
        bool m_attached = false;

        Reference<FramebufferTemplate> m_framebuffer_template = nullptr;
    };

}

#endif //OPENGLFRAMEBUFFER_H
