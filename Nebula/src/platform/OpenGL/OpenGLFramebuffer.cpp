//
// Created by michal-swiatek on 24.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/OpenGL/OpenGLFramebuffer.h"

namespace nebula::rendering {

    OpenGlFramebuffer::OpenGlFramebuffer(const Reference<FramebufferTemplate>& framebuffer_template) :
            m_framebuffer_template(framebuffer_template)
    {

    }

    OpenGlFramebuffer::~OpenGlFramebuffer()
    {

    }

    void OpenGlFramebuffer::bind()
    {

    }

    void OpenGlFramebuffer::unbind()
    {

    }

    bool OpenGlFramebuffer::attached() const
    {
        return m_attached;
    }

    void OpenGlFramebuffer::attachTo(void* renderpass_handle)
    {
        NB_ASSERT(renderpass_handle, "Recieved null renderpass handle!");
        m_attached = true;
    }

    View<FramebufferTemplate> OpenGlFramebuffer::viewFramebufferTemplate() const
    {
        return m_framebuffer_template.get();
    }

}
