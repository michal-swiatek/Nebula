//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_EVENT_H
#define NEBULAENGINE_EVENT_H

#include <string>

#include "core/Core.h"

namespace nebula {

    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory
    {
        None                     = 0,
        EventCategoryApplication = 1,
        EventCategoryInput       = 2,
        EventCategoryKeyboard    = 4,
        EventCategoryMouse       = 8,
        EventCategoryMouseButton = 16
    };

    class NEBULA_API Event
    {
    public:
        virtual ~Event() = default;

        bool handled = false;

        [[nodiscard]] virtual EventType getEventType() const = 0;
        [[nodiscard]] virtual const char* getName() const = 0;
        [[nodiscard]] virtual int getCategoryFlags() const = 0;
        [[nodiscard]] virtual std::string toString() const { return getName(); }

        [[nodiscard]] bool isInCategory(EventCategory category) const { return getCategoryFlags() & category; }
    };

    class NEBULA_API EventDelegate
    {
    public:
        explicit EventDelegate(Event& event) : m_event(event) {}

        template <typename T, typename F>
        bool delegate(const F& function)
        {
            if (m_event.getEventType() == T::getStaticType())
            {
                m_event.handled |= function(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }

    private:
        Event& m_event;
    };

    inline std::ostream& operator << (std::ostream& os, const Event& e) { return os << e.toString(); }

    #define EVENT_CLASS_TYPE(type)  static EventType getStaticType() { return EventType::type; }\
								    virtual EventType getEventType() const override { return getStaticType(); }\
								    virtual const char* getName() const override { return #type"Event"; }

    #define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

} // nebula

#endif //NEBULAENGINE_EVENT_H
