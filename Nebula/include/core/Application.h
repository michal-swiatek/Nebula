//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_APPLICATION_H
#define NEBULAENGINE_APPLICATION_H

#include <mutex>
#include <string>
#include <vector>
#include <optional>

#include "Core.h"
#include "Timer.h"
#include "Input.h"
#include "Window.h"
#include "LayerStack.h"

#include "events/Event.h"
#include "events/EventManager.h"
#include "events/ApplicationEvents.h"

#include "threads/SecondaryThread.h"
#include "utility/Filesystem.h"

int main(int argc, char** argv);

namespace nebula {

    namespace threads {

        class MainUpdateThread;
        class MainRenderThread;

    }

    namespace rendering { class VulkanRecordCommandsVisitor; }

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

        //  TODO: Move to config file
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
        bool minimized() { std::lock_guard lock{m_mutex}; return m_minimized; }
        void minimize(const bool minimized) { std::lock_guard lock{m_mutex}; m_minimized = minimized; }

        ///////////////////////////////////////////////////////////////////////////////////
        /////  Properties  ////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////

        [[nodiscard]] rendering::API getRenderingAPI() const { return m_specification.api; }
        [[nodiscard]] ApplicationVersion getVersion() const { return m_specification.version; }
        [[nodiscard]] std::string getName() const { return m_specification.name; }

        static Window& getWindow() { return *s_instance->m_window; }
        static Application& get() { return *s_instance; }

        static void reloadEngineConfig(const std::string& path = "");
        static filesystem::Path getResourcesPath(bool absolute = false);

    protected:
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

        Scope<Layer> popLayer(const LayerStack::LayerID layer_id) { return m_layer_stack.popLayer(layer_id); }
        Scope<Layer> popOverlay(const LayerStack::LayerID layer_id) { return m_layer_stack.popOverlay(layer_id); }

    private:
        void run() const;

        void onEvent(Event& event);
        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);

        bool m_running = true;
        bool m_minimized = false;

        Scope<Input> m_input;
        Scope<Window> m_window;
        ApplicationSpecification m_specification;

        LayerStack m_layer_stack;
        EventManager m_event_manager;

        //  Threads
        std::mutex m_mutex;
        std::vector<Scope<threads::SecondaryThread>> m_threads;

        void createThreads();
        void cleanupThreads();
        void closeThreads() const;

        friend class nebula::threads::MainUpdateThread;
        friend class nebula::threads::MainRenderThread;
        friend class nebula::rendering::VulkanRecordCommandsVisitor;

        static Application* s_instance;
        friend int ::main(int argc, char** argv);
    };

    Application* createApplication(int argc, char** argv);

}

#endif //NEBULAENGINE_APPLICATION_H
