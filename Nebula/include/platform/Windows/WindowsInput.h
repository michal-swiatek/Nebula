//
// Created by michal-swiatek on 10.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef WINDOWSINPUT_H
#define WINDOWSINPUT_H

#include <GLFW/glfw3.h>

#include "core/Input.h"

namespace nebula {

    class WindowsInput final : public Input
    {
    public:
        explicit WindowsInput(const Window& window);

        [[nodiscard]] bool isKeyPressedImplementation(Keycode key) const override;

        [[nodiscard]] bool isMousePressedImplementation(MouseCode button) const override;
        [[nodiscard]] float getMouseXImplementation() const override;
        [[nodiscard]] float getMouseYImplementation() const override;
        [[nodiscard]] glm::vec2 getMousePositionImplementation() const override;

    private:
        GLFWwindow* m_window = nullptr;
    };

}

#endif //WINDOWSINPUT_H
