//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/OpenGL/OpenGLContext.h"

#include <format>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/Assert.h"
#include "platform/EngineConfiguration.h"
#include "platform/OpenGL/OpenGLConfiguration.h"

namespace nebula::rendering {

    OpenGLContext::OpenGLContext(GLFWwindow* window_handle) : m_window(window_handle)
    {
        NB_CORE_ASSERT(m_window, "Window handle in null!");

        glfwMakeContextCurrent(m_window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        NB_CORE_ASSERT(status, "Failed to initialize Glad!");

        setVSync(true);

        if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= 1)
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
    }

    void OpenGLContext::bind()
    {
        glfwMakeContextCurrent(m_window);
    }

    void OpenGLContext::unbind()
    {
        glfwMakeContextCurrent(nullptr);
    }

    void OpenGLContext::setVSync(const bool vsync)
    {
        if (vsync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        m_vsync = vsync;
    }

    bool OpenGLContext::checkVSync()
    {
        return m_vsync;
    }

    void OpenGLContext::swapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

    const Reference<FramebufferTemplate>& OpenGLContext::viewFramebufferTemplate() const
    {
        return nullptr;
    }

}
