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
#include "platform/PlatformAPI.h"

namespace nebula {

    struct NEBULA_API WindowProperties
    {
        std::string title;
        int32_t width;
        int32_t height;
        bool vsync;

        explicit WindowProperties(
            std::string title = "Nebula Engine",
            int32_t width = 1600,
            int32_t height = 900,
            bool vsync = true
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

        [[nodiscard]] virtual bool checkVSync() const = 0;
        [[nodiscard]] virtual uint32_t getWidth() const = 0;
        [[nodiscard]] virtual uint32_t getHeight() const = 0;
        [[nodiscard]] virtual WindowProperties getProperties() const = 0;

        virtual void setVSync(bool enabled) = 0;
        virtual void setProperties(const WindowProperties& window_properties) = 0;

        [[nodiscard]] virtual void* getWindowHandle() const = 0;

    private:
        friend class Application;

        virtual void onUpdate() = 0;
        virtual void setEventManager(EventManager& event_manager) = 0;

        static Scope<Window> create(const WindowProperties& properties, rendering::API api);
    };

}

#endif //NEBULAENGINE_WINDOW_H
