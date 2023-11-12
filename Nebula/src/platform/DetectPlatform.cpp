//
// Created by michal-swiatek on 10.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/DetectPlatform.h"

#include "memory/MemoryManager.h"

#include "core/Window.h"
#include "core/Input.h"
#include "core/Types.h"

#include "renderer/RendererAPI.h"
#include "platform/OpenGL/OpenGLContext.h"
#include "platform/OpenGL/OpenGLRendererAPI.h"

#ifdef NB_PLATFORM_WINDOWS
    #include "platform/Windows/WindowsWindow.h"
    #include "platform/Windows/WindowsInput.h"
#endif

namespace nebula {

    Input* Input::s_instance = nullptr;

    Scope<Window> Window::create(const WindowProperties& properties)
    {
        #ifdef NB_PLATFORM_WINDOWS
        return createScope<WindowsWindow>(properties);
        #else
        NB_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
        #endif
    }

    Scope<Input> Input::create(View<Window> window)
    {
        #ifdef NB_PLATFORM_WINDOWS
        return createScope<WindowsInput>(*window);
        #else
        NB_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
        #endif
    }

    namespace rendering::impl {

        Scope<RenderContext> RenderContext::create(const API api, void* window_handle)
        {
            switch (api)
            {
                case API::cOpenGL:    return createScope<OpenGLContext>(static_cast<GLFWwindow*>(window_handle));
                default:                      NB_CORE_ASSERT(false, "Undefined Rendering API!");  return nullptr;
            }
        }

        View<RendererApi> RendererApi::create(const API api)
        {
            switch (api)
            {
                case API::cOpenGL:    return memory::MemoryManager::create<OpenGlRendererApi>();
                default:     NB_CORE_ASSERT(false, "Undefined Rendering API!");  return nullptr;
            }
        }

        void RendererApi::destroy(RendererApi* api)
        {
            memory::MemoryManager::destroy(api);
        }

    }

}
