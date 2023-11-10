//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_APPLICATIONEVENTS_H
#define NEBULAENGINE_APPLICATIONEVENTS_H

#include <sstream>

#include "Event.h"

namespace nebula {

    class NEBULA_API WindowResizeEvent final : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {}

        [[nodiscard]] unsigned int getWidth() const { return m_width; }
        [[nodiscard]] unsigned int getHeight() const { return m_height; }

        [[nodiscard]] std::string toString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent(width=" << m_width << ", height=" << m_height << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

    private:
        unsigned int m_width;
        unsigned int m_height;
    };

    class NEBULA_API WindowCloseEvent final : public Event
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class NEBULA_API AppTickEvent final : public Event
    {
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class NEBULA_API AppUpdateEvent final : public Event
    {
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class NEBULA_API AppRenderEvent final : public Event
    {
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

}

#endif //NEBULAENGINE_APPLICATIONEVENTS_H
