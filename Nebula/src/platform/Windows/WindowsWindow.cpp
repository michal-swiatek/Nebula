//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Windows/WindowsWindow.h"

#include "core/Logging.h"
#include "events/MouseEvents.h"
#include "events/KeyboardEvents.h"
#include "events/ApplicationEvents.h"

namespace nebula {

    static uint8_t window_count = 0;

    static void GLFWErrorCallback(int error, const char* description)
    {
        NB_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    WindowsWindow::WindowsWindow(const nebula::WindowProperties& properties)
    {
        m_window_data.title = properties.title;
        m_window_data.width = properties.width;
        m_window_data.height = properties.height;
        m_window_data.vsync = properties.vsync;

        NB_CORE_INFO("Creating window: {} ({}, {})", properties.title, properties.width, properties.height);
        if (window_count == 0)
        {
            NB_CORE_INFO("Initializing GLFW");
            auto success = glfwInit();
            NB_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        m_window = glfwCreateWindow(properties.width, properties.height, properties.title.c_str(), nullptr, nullptr);

        if (m_window_data.vsync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        glfwSetWindowUserPointer(m_window, &m_window_data);
        setGLFWCallbacks();
    }

    WindowsWindow::~WindowsWindow()
    {
        glfwDestroyWindow(m_window);
        --window_count;

        if (window_count == 0)
        {
            NB_CORE_INFO("Terminating GLFW");
            glfwTerminate();
        }
    }

    void WindowsWindow::setGLFWCallbacks()
    {
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.width = width;
            data.height = height;

            data.event_manager->queueEvent<WindowResizeEvent>(width, height);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
        {
            WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.event_manager->queueEvent<WindowCloseEvent>();
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            auto keycode = static_cast<Keycode>(key);

            switch (action)
            {
                case GLFW_PRESS:    data.event_manager->queueEvent<KeyPressedEvent>(keycode, 0);        break;
                case GLFW_RELEASE:  data.event_manager->queueEvent<KeyReleasedEvent>(keycode);          break;
                case GLFW_REPEAT:   data.event_manager->queueEvent<KeyPressedEvent>(keycode, true);     break;
                default: return;
            }
        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.event_manager->queueEvent<KeyTypedEvent>(static_cast<Keycode>(keycode));
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            auto mouse_button = static_cast<MouseCode>(button);

            switch (action)
            {
                case GLFW_PRESS:    data.event_manager->queueEvent<MouseButtonPressedEvent>(mouse_button); break;
                case GLFW_RELEASE:  data.event_manager->queueEvent<MouseButtonReleasedEvent>(mouse_button); break;
                default: return;
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double x_offset, double y_offset)
        {
            WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.event_manager->queueEvent<MouseScrolledEvent>(static_cast<float>(x_offset), static_cast<float>(y_offset));
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x_pos, double y_pos)
        {
            WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.event_manager->queueEvent<MouseMovedEvent>(static_cast<float>(x_pos), static_cast<float>(y_pos));
        });
    }

    void WindowsWindow::onUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }

    void WindowsWindow::setVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_window_data.vsync = enabled;
    }

}
