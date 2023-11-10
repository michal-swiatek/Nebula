//
// Created by michal-swiatek on 10.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef INPUT_H
#define INPUT_H

#include "Core.h"
#include "Types.h"
#include "Window.h"
#include "InputCodes.h"

#include <glm/glm.hpp>

namespace nebula {

    class NEBULA_API Input
    {
    public:
        virtual ~Input() = default;

        static bool isKeyPressed(const Keycode key) { return s_instance->isKeyPressedImplementation(key); }

        static bool isMousePressed(const MouseCode button) { return s_instance->isMousePressedImplementation(button); }
        static float getMouseX() { return s_instance->getMouseXImplementation(); }
        static float getMouseY() { return s_instance->getMouseYImplementation(); }
        static glm::vec2 getMousePosition() { return s_instance->getMousePositionImplementation(); }

    protected:
        [[nodiscard]] virtual bool isKeyPressedImplementation(Keycode key) const = 0;

        [[nodiscard]] virtual bool isMousePressedImplementation(MouseCode button) const = 0;
        [[nodiscard]] virtual float getMouseXImplementation() const = 0;
        [[nodiscard]] virtual float getMouseYImplementation() const = 0;
        [[nodiscard]] virtual glm::vec2 getMousePositionImplementation() const = 0;

        static Input* s_instance;

    private:
        friend class Application;
        static Scope<Input> create(View<Window> window);
    };

}

#endif //INPUT_H
