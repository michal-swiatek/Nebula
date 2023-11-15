//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "core/Core.h"
#include "core/Logging.h"

namespace nebula::rendering::impl {

    void OpenGLMessageCallback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const char* message,
        const void* userParam
    )
    {
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         NB_CORE_FATAL(message); return;
            case GL_DEBUG_SEVERITY_MEDIUM:       NB_CORE_ERROR(message); return;
            case GL_DEBUG_SEVERITY_LOW:          NB_CORE_WARN(message);  return;
            case GL_DEBUG_SEVERITY_NOTIFICATION: NB_CORE_TRACE(message); return;
            default:                             NB_CORE_ASSERT(false, "Unknown severity level!");
        }
    }

    GLbitfield convertClearFlags(ClearBufferType flags)
    {
        GLbitfield gl_flags = 0;

        gl_flags |= flags & ClearBufferType::cColorBuffer ? GL_COLOR_BUFFER_BIT : 0;
        gl_flags |= flags & ClearBufferType::cDepthBuffer ? GL_DEPTH_BUFFER_BIT : 0;
        gl_flags |= flags & ClearBufferType::cStencilBuffer ? GL_STENCIL_BUFFER_BIT : 0;

        return gl_flags;
    }

    void OpenGlRendererApi::init()
    {
        #ifdef NB_DEBUG_BUILD
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(OpenGLMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
        #endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LINE_SMOOTH);
    }

    void OpenGlRendererApi::shutdown()
    {

    }

    void OpenGlRendererApi::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGlRendererApi::setClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGlRendererApi::clear(ClearBufferType flags)
    {
        glClear(convertClearFlags(flags));
    }

}
