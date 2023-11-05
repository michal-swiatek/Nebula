//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Window.h"

#ifdef NB_PLATFORM_WINDOWS
    #include "platform/Windows/WindowsWindow.h"
#endif

namespace nebula {

    Scope<Window> Window::create(const nebula::WindowProperties& properties)
    {
        #ifdef NB_PLATFORM_WINDOWS
            return std::make_unique<WindowsWindow>(properties);
        #else
            NB_CORE_ASSERT(false, "Unknown platform!");
            return nullptr;
        #endif
    }

}
