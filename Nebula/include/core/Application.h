//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_APPLICATION_H
#define NEBULAENGINE_APPLICATION_H

#include <string>
#include <optional>

#include "Core.h"
#include "Timer.h"
#include "Input.h"
#include "Window.h"
#include "LayerStack.h"

#include "events/Event.h"
#include "events/EventManager.h"
#include "events/ApplicationEvents.h"

#include "utility/Filesystem.h"
#include "rendering/RenderContext.h"

int main(int argc, char** argv);

namespace nebula {

    struct ApplicationVersion
    {
        uint16_t major;
        uint16_t minor;
        uint16_t patch;
    };

    struct ApplicationSpecification
    {
        std::string name = "Nebula Application";
        std::string logger_name = "APP";
        std::string working_directory;

        int render_fps = 0;
        double update_timestep = 0.02;

        rendering::API api = rendering::API::cOpenGL;
        ApplicationVersion version = ApplicationVersion{1, 0, 0};
    };

    class NEBULA_API Application
    {
    public:
        explicit Application(ApplicationSpecification specification, const std::optional<WindowProperties>& window_properties = {});
        virtual ~Application();

        void close();
        void minimize(bool minimized);

        template <typename T, typename... Args>
        LayerStack::LayerID pushLayer(Args&&... args)
        {
            auto layer = new T(std::forward<Args>(args)...); //  TODO: Replace with memory allocator
            layer->onAttach();
            return m_layer_stack.pushLayer(layer);
        }

        template <typename T, typename... Args>
        LayerStack::LayerID pushOverlay(Args&&... args)
        {
            auto layer = new T(std::forward<Args>(args)...); //  TODO: Replace with memory allocator
            layer->onAttach();
            return m_layer_stack.pushOverlay(layer);
        }

        Scope<Layer> popLayer(LayerStack::LayerID layer_id) { return m_layer_stack.popLayer(layer_id); }
        Scope<Layer> popOverlay(LayerStack::LayerID layer_id) { return m_layer_stack.popOverlay(layer_id); }

        [[nodiscard]] int getRenderFps() const { return m_specification.render_fps; }
        [[nodiscard]] double getUpdateTimestep() const { return m_specification.update_timestep; }
        [[nodiscard]] double getTime() { return m_application_timer.elapsedSeconds(); }
        [[nodiscard]] std::string getName() const { return m_specification.name; }

        void setRenderFps(const int fps) { m_specification.render_fps = fps; }
        void setUpdateTimestep(const double timestep) { m_specification.update_timestep = timestep; }

        [[nodiscard]] rendering::API getRenderingAPI() const;

        [[nodiscard]] ApplicationVersion getVersion() const { return m_specification.version; }
        [[nodiscard]] Window& getWindow() const { return *m_window; }
        static Application& get() { return *s_instance; }

        static void reloadEngineConfig(const std::string& path = "");
        static filesystem::Path getResourcesPath(bool absolute = false);

    private:
        void run();

        void onEvent(Event& event);
        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);

        bool m_running = true;
        bool m_minimized = false;
        Timer m_application_timer;

        Scope<Input> m_input;
        Scope<Window> m_window;
        ApplicationSpecification m_specification;

        LayerStack m_layer_stack;
        EventManager m_event_manager;

        //  Update
        Timer m_update_timer;
        double m_update_accumulator = 0.0;

        //  Rendering
        Timer m_render_timer;
        Scope<rendering::RenderContext> m_render_context = nullptr;

        static Application* s_instance;
        friend int ::main(int argc, char** argv);
    };

    Application* createApplication(int argc, char** argv);

}

#endif //NEBULAENGINE_APPLICATION_H
