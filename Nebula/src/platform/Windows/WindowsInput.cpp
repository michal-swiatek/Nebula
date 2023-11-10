//
// Created by michal-swiatek on 10.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Windows/WindowsInput.h"

#include "core/Logging.h"

namespace nebula {

    WindowsInput::WindowsInput(const Window& window)
    {
        m_window = static_cast<GLFWwindow*>(window.getWindowHandle());

        NB_CORE_ASSERT(!s_instance, "Can't create another instance of Application!");
        s_instance = this;
    }

    bool WindowsInput::isKeyPressedImplementation(const Keycode key) const
    {
        return glfwGetKey(m_window, static_cast<uint16_t>(key)) == GLFW_PRESS;
    }

    bool WindowsInput::isMousePressedImplementation(MouseCode button) const
    {
        return glfwGetMouseButton(m_window, static_cast<uint16_t>(button)) == GLFW_PRESS;
    }

    float WindowsInput::getMouseXImplementation() const
    {
        return getMousePositionImplementation().x;
    }

    float WindowsInput::getMouseYImplementation() const
    {
        return getMousePositionImplementation().y;
    }

    glm::vec2 WindowsInput::getMousePositionImplementation() const
    {
        double x_pos, y_pos;
        glfwGetCursorPos(m_window, &x_pos, &y_pos);
        return {x_pos, y_pos};
    }

}
