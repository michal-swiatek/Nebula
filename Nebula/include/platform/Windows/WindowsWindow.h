//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_WINDOWSWINDOW_H
#define NEBULAENGINE_WINDOWSWINDOW_H

#include <GLFW/glfw3.h>

#include "core/Window.h"

namespace nebula {

    class WindowsWindow : public Window
    {
    public:
        explicit WindowsWindow(const WindowProperties& properties);
        ~WindowsWindow() override;

        void onUpdate() override;

        [[nodiscard]] inline bool checkVSync() const override { return m_window_data.vsync; }
        [[nodiscard]] inline uint32_t getWidth() const override { return m_window_data.width; }
        [[nodiscard]] inline uint32_t getHeight() const override { return m_window_data.height; }

        void setVSync(bool enabled) override;
        void setEventManager(EventManager& event_manager) override { m_window_data.event_manager = &event_manager; }

        [[nodiscard]] inline void* getNativeWindow() const override { return m_window; }

    private:
        GLFWwindow* m_window = nullptr;

        void setGLFWCallbacks();

        struct WindowData
        {
            std::string title;
            int32_t width;
            int32_t height;
            bool vsync;

            View<EventManager> event_manager;
        };

        WindowData m_window_data;
    };

}

#endif //NEBULAENGINE_WINDOWSWINDOW_H
