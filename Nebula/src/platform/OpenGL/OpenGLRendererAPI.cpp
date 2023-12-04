//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "core/Core.h"
#include "core/Logging.h"

namespace nebula::rendering {

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

    OpenGlRendererApi::OpenGlRendererApi()
    {
        #ifdef NB_DEBUG_BUILD
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(OpenGLMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
        #endif
    }

    OpenGlRendererApi::~OpenGlRendererApi()
    {

    }

    void OpenGlRendererApi::compilePipelines(RenderPass* renderpass)
    {

    }

}
