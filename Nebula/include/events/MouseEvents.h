//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_MOUSEEVENTS_H
#define NEBULAENGINE_MOUSEEVENTS_H

#include "Event.h"
#include "core/InputCodes.h"

namespace nebula {

    class NEBULA_API MouseMovedEvent final : public Event
    {
    public:
        MouseMovedEvent(const float x, const float y) : m_mouse_x(x), m_mouse_y(y) {}

        [[nodiscard]] float getX() const { return m_mouse_x; }
        [[nodiscard]] float getY() const { return m_mouse_y; }

        [[nodiscard]] std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent(x=" << m_mouse_x << ", x=" << m_mouse_y << ')';
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_mouse_x;
        float m_mouse_y;
    };

    class NEBULA_API MouseScrolledEvent final : public Event
    {
    public:
        MouseScrolledEvent(const float x_offset, const float y_offset) : m_offset_x(x_offset), m_offset_y(y_offset) {}

        [[nodiscard]] float getXOffset() const { return m_offset_x; }
        [[nodiscard]] float getYOffset() const { return m_offset_y; }

        [[nodiscard]] std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent(x_offset=" << m_offset_x << ", y_offset=" << m_offset_y << ')';
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_offset_x;
        float m_offset_y;
    };

    class NEBULA_API MouseButtonEvent : public Event
    {
    public:
        [[nodiscard]] MouseCode getMouseButton() const { return m_button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

    protected:
        explicit MouseButtonEvent(const MouseCode button) : m_button(button) {}

        MouseCode m_button;
    };

    class NEBULA_API MouseButtonPressedEvent final : public MouseButtonEvent
    {
    public:
        explicit MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button) {}

        [[nodiscard]] std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent(button=" << static_cast<uint16_t>(m_button) << ')';
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class NEBULA_API MouseButtonReleasedEvent final : public MouseButtonEvent
    {
    public:
        explicit MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button) {}

        [[nodiscard]] std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent(button=" << static_cast<uint16_t>(m_button) << ')';
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

}

#endif //NEBULAENGINE_MOUSEEVENTS_H
