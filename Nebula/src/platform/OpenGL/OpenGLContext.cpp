//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/OpenGL/OpenGLContext.h"

#include <format>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <rendering/Framebuffer.h>

#include "core/Assert.h"
#include "platform/EngineConfiguration.h"
#include "platform/OpenGL/OpenGLConfiguration.h"
#include "platform/OpenGL/OpenGLCommandsVisitor.h"

namespace nebula::rendering {

    OpenGLContext::OpenGLContext(GLFWwindow* window_handle) : m_window(window_handle)
    {
        NB_CORE_ASSERT(m_window, "Window handle in null!");

        glfwMakeContextCurrent(m_window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        NB_CORE_ASSERT(status, "Failed to initialize Glad!");

        setVSync(true);

        if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= NEBULA_INITIALIZATION_VERBOSITY_LOW)
        {
            NB_CORE_INFO("OpenGL Info:");
            NB_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
            NB_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
            NB_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
        }

        NB_CORE_ASSERT(
            GLVersion.major > OPENGL_MAJOR_VERSION || (GLVersion.major == OPENGL_MAJOR_VERSION && GLVersion.minor >= OPENGL_MINOR_VERSION),
            std::format("Nebula requires at least OpenGL version {}.{}!", OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION)
        );

        m_framebuffer_template = createReference<OpenGLFramebufferTemplate>();
        m_framebuffer = Framebuffer::create(m_framebuffer_template);
    }

    void OpenGLContext::bind()
    {
        glfwMakeContextCurrent(m_window);
    }

    void OpenGLContext::unbind()
    {
        glfwMakeContextCurrent(nullptr);
    }

    void OpenGLContext::reload()
    {
        if (checkVSync())
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
    }

    void OpenGLContext::presentImage()
    {
        glfwSwapBuffers(m_window);
    }

    Reference<Framebuffer> OpenGLContext::getNextImage()
    {
        return m_framebuffer;
    }

    Scope<ExecuteCommandVisitor> OpenGLContext::getCommandExecutor()
    {
        return createScope<OpenGlExecuteCommandsVisitor>();
    }

    void OpenGLContext::waitForFrameResources(uint32_t frame)
    {

    }

    ApiInfo OpenGLContext::getApiInfo() const
    {
        NB_CORE_INFO("OpenGL Info:");
        NB_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
        NB_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        NB_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

        ApiInfo api_info;
        api_info.api_name = "OpenGL";
        api_info.vendor_name = (const char*)glGetString(GL_VENDOR);
        api_info.renderer_name = (const char*)glGetString(GL_RENDERER);
        api_info.api_version = std::format("{}.{}.{}", OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION, OPENGL_PATCH_VERSION);
        api_info.driver_version = (const char*)glGetString(GL_VERSION);

        return api_info;
    }

    const Reference<FramebufferTemplate>& OpenGLContext::viewFramebufferTemplate() const
    {
        return m_framebuffer_template;
    }

}
