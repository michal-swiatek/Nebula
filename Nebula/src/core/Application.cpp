//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Application.h"

#include <thread>
#include <filesystem>

#include "core/Config.h"
#include "core/Logging.h"

#include "renderer/Renderer.h"

namespace nebula {

    Application* Application::s_instance = nullptr;

    Application::Application(
        ApplicationSpecification specification,
        const std::optional<WindowProperties>& window_properties
    ) :
            m_specification(std::move(specification)),
            m_event_manager(
              m_layer_stack,
              [this](Event& event) { onEvent(event); },
              Config::getEngineConfig()["memory"]["event_queue_size"].as<int>()
            )
    {
        NB_CORE_ASSERT(!s_instance, "Can't create another instance of Application!");
        s_instance = this;

        if (!m_specification.working_directory.empty())
            std::filesystem::current_path(m_specification.working_directory);

        logging::initClient(m_specification.logger_name);

        const auto window_settings = window_properties ? *window_properties : WindowProperties(m_specification.name);
        m_window = Window::create(window_settings, m_specification.api);

        m_window->setEventManager(m_event_manager);
        m_input = Input::create(m_window.get());
    }

    void Application::run()
    {
        std::thread render_thread(&threads::RenderThread::run, &m_render_thread, getRenderingAPI());
        threads::RenderThread::blockUntilInitialized();
        std::thread update_thread(&threads::UpdateThread::run, &m_update_thread);

        while (m_running)
        {
            //  Poll events
            m_window->onUpdate();
        }

        update_thread.join();
        render_thread.join();
    }

    void Application::close()
    {
        m_running = false;
        m_update_thread.close();
        m_render_thread.close();
    }

    void Application::minimize(bool minimized)
    {
        m_minimized = minimized;
        m_update_thread.minimize(minimized);
        m_render_thread.minimize(minimized);
    }

    rendering::API Application::getRenderingAPI() const
    {
        return m_specification.api;
    }

    void Application::onEvent(Event& event)
    {
        EventDelegate delegate(event);
        delegate.delegate<WindowCloseEvent>(NB_BIND_EVENT_FUNCTION(Application::onWindowClose));
        delegate.delegate<WindowResizeEvent>(NB_BIND_EVENT_FUNCTION(Application::onWindowResize));
    }

    bool Application::onWindowClose(WindowCloseEvent& event)
    {
        close();
        return true;
    }

    bool Application::onWindowResize(WindowResizeEvent& event)
    {
        if (event.getWidth() == 0 || event.getHeight() == 0)
        {
            minimize(true);
            return false;
        }

        minimize(false);

        m_render_thread.onWindowResize(event);
        return false;
    }

}
