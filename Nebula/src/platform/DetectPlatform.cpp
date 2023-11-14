//
// Created by michal-swiatek on 10.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/DetectPlatform.h"

#include "memory/MemoryManager.h"

#include "core/Window.h"
#include "core/Input.h"
#include "core/Timer.h"
#include "core/Types.h"

#include "renderer/RendererAPI.h"
#include "platform/OpenGL/OpenGLContext.h"
#include "platform/OpenGL/OpenGLRendererAPI.h"

#ifdef NB_PLATFORM_WINDOWS
    #include <Windows.h>
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

    bool Timer::sleep(double seconds)
    {
        const auto nanoseconds = static_cast<long long>(seconds * 1'000'000'000);

        #ifdef NB_PLATFORM_WINDOWS
        // HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
        HANDLE timer = CreateWaitableTimerEx(NULL, NULL, CREATE_WAITABLE_TIMER_MANUAL_RESET | CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_MODIFY_STATE | SYNCHRONIZE);
        NB_CORE_ASSERT(timer, "Unable to create WINDOWS high resolution timer!");
        if(!timer)
            return false;

        LARGE_INTEGER li;
        li.QuadPart = -nanoseconds / 100;
        if(!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE))
        {
            NB_CORE_ASSERT(false, "Unable to set WINDOWS high resolution timer!");
            CloseHandle(timer);
            return false;
        }

        WaitForSingleObject(timer, INFINITE);
        CloseHandle(timer);

        return true;
        #else
        NB_CORE_ASSERT(false, "Unknown platform!");
        #endif
    }

}
