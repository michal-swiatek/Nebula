//
// Created by michal-swiatek on 10.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/DetectPlatform.h"

#include "core/Window.h"
#include "core/Input.h"

#ifdef NB_PLATFORM_WINDOWS
    #include "platform/Windows/WindowsWindow.h"
    #include "platform/Windows/WindowsInput.h"
#endif

namespace nebula {

    Input* Input::s_instance = nullptr;

    Scope<Window> Window::create(const WindowProperties& properties)
    {
        #ifdef NB_PLATFORM_WINDOWS
        return std::make_unique<WindowsWindow>(properties);
        #else
        NB_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
        #endif
    }

    Scope<Input> Input::create(View<Window> window)
    {
        #ifdef NB_PLATFORM_WINDOWS
        return std::make_unique<WindowsInput>(*window);
        #else
        NB_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
        #endif
    }

}
