//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_KEYBOARDEVENTS_H
#define NEBULAENGINE_KEYBOARDEVENTS_H

#include "Event.h"
#include "core/InputCodes.h"

namespace nebula {

    class NEBULA_API KeyEvent : public Event
    {
    public:
        [[nodiscard]] Keycode getKeycode() const { return m_keycode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        explicit KeyEvent(const Keycode keycode) : m_keycode(keycode) {}

        Keycode m_keycode;
    };

    class NEBULA_API KeyPressedEvent : public KeyEvent
    {
    public:
        explicit KeyPressedEvent(const Keycode keycode, bool repeated = false) : KeyEvent(keycode), m_repeated(repeated) {}

        [[nodiscard]] bool isRepeated() const { return m_repeated; }

        [[nodiscard]] std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent(keycode=" << m_keycode << ", repeated=" << m_repeated << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        bool m_repeated;
    };

    class NEBULA_API KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(const Keycode keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent(keycode=" << m_keycode << ')';
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class NEBULA_API KeyTypedEvent : public KeyEvent
    {
    public:
        explicit KeyTypedEvent(const Keycode keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent(keycode=" << m_keycode << ')';
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };

}

#endif //NEBULAENGINE_KEYBOARDEVENTS_H
