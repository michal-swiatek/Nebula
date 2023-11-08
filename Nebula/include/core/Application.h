//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_APPLICATION_H
#define NEBULAENGINE_APPLICATION_H

#include <string>

#include "Core.h"
#include "Timer.h"
#include "Window.h"
#include "LayerStack.h"

#include "events/Event.h"
#include "events/EventManager.h"
#include "events/ApplicationEvents.h"

int main(int argc, char** argv);

namespace nebula {

    struct ApplicationSpecification
    {
        std::string name = "Nebula Application";
        std::string logger_name = "APP";
        std::string working_directory;

        int render_fps = 0;
        int update_fps = 50;
    };

    class NEBULA_API Application
    {
    public:
        explicit Application(ApplicationSpecification specification);
        virtual ~Application() = default;

        template <typename T, typename... Args>
        LayerStack::LayerID pushLayer(Args&&... args)
        {
            auto layer = new T(std::forward<Args>(args)...);    //  TODO: Replace with memory allocator
            layer->onAttach();
            return m_layer_stack.pushLayer(layer);
        }

        template <typename T, typename... Args>
        LayerStack::LayerID pushOverlay(Args&&... args)
        {
            auto layer = new T(std::forward<Args>(args)...);    //  TODO: Replace with memory allocator
            layer->onAttach();
            return m_layer_stack.pushOverlay(layer);
        }

        Scope<Layer> popLayer(LayerStack::LayerID layer_id) { return m_layer_stack.popLayer(layer_id); }
        Scope<Layer> popOverlay(LayerStack::LayerID layer_id) { return m_layer_stack.popOverlay(layer_id); }

        [[nodiscard]] inline int getRenderFps() const { return m_specification.render_fps; }
        [[nodiscard]] inline int getUpdateFps() const { return m_specification.update_fps; }

        inline void setRenderFps(int render_fps) { m_specification.render_fps = render_fps; }
        inline void setUpdateFps(int update_fps) { m_specification.update_fps = update_fps; }

        [[nodiscard]] inline WindowProperties getWindowProperties() const { return m_window->getProperties(); }
        inline void setWindowProperties(const WindowProperties& window_properties) { m_window->setProperties(window_properties); }

    private:
        void run();

        void onEvent(Event& event);
        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);

        bool m_running = true;
        bool m_minimized = false;

        Timer m_timer;
        double m_update_accumulator = 0.0;

        LayerStack m_layer_stack;
        EventManager m_event_manager;

        Scope<Window> m_window;
        ApplicationSpecification m_specification;

        friend int ::main(int argc, char** argv);
    };

    Application* createApplication(int argc, char** argv);

}

#endif //NEBULAENGINE_APPLICATION_H
