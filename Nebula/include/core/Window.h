//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_WINDOW_H
#define NEBULAENGINE_WINDOW_H

#include <string>

#include "Core.h"
#include "Types.h"
#include "events/EventManager.h"

namespace nebula {

    struct NEBULA_API WindowProperties
    {
        std::string title;
        int32_t width;
        int32_t height;
        bool vsync;

        explicit WindowProperties(
            std::string title = "Nebula Engine",
            uint32_t width = 1600,
            uint32_t height = 900,
            bool vsync = false
        ) :
                title(std::move(title)),
                width(width),
                height(height),
                vsync(vsync)
        {}
    };

    class NEBULA_API Window
    {
    public:
        virtual ~Window() = default;

        virtual void onUpdate() = 0;

        [[nodiscard]] virtual bool checkVSync() const = 0;
        [[nodiscard]] virtual uint32_t getWidth() const = 0;
        [[nodiscard]] virtual uint32_t getHeight() const = 0;

        virtual void setVSync(bool enabled) = 0;
        virtual void setEventManager(EventManager& event_manager) = 0;

        [[nodiscard]] virtual void* getNativeWindow() const = 0;

        static Scope<Window> create(const WindowProperties& properties = WindowProperties());
    };

}

#endif //NEBULAENGINE_WINDOW_H
