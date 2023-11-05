//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include <filesystem>

#include "core/Application.h"

#include "core/Logging.h"

namespace nebula {

    Application::Application(ApplicationSpecification specification)
        : m_specification(std::move(specification)),
          m_event_manager(m_layer_stack, [this](Event& event) { onEvent(event); })
    {
        if (!m_specification.working_directory.empty())
            std::filesystem::current_path(m_specification.working_directory);

        logging::initClient(m_specification.logger_name);

        m_window = Window::create(WindowProperties(m_specification.name));
        m_window->setEventManager(m_event_manager);
    }

    void Application::run()
    {
        while (m_running)
        {
            auto delta_time = Timestep(m_timer.elapsedSeconds(true));

            if (!m_minimized)
            {
                for (auto& layer : m_layer_stack)
                    layer->onUpdate(delta_time);

                for (auto& layer : m_layer_stack)
                    layer->onImGuiRender();
            }

            m_window->onUpdate();
            m_event_manager.dispatchEvents();
        }
    }

    void Application::onEvent(Event& event)
    {
        EventDelegate delegate(event);
        delegate.delegate<WindowCloseEvent>(NB_BIND_EVENT_FUNCTION(Application::onWindowClose));
        delegate.delegate<WindowResizeEvent>(NB_BIND_EVENT_FUNCTION(Application::onWindowResize));
    }

    bool Application::onWindowClose(WindowCloseEvent& event)
    {
        m_running = false;
        return true;
    }

    bool Application::onWindowResize(WindowResizeEvent& event)
    {
        if (event.getWidth() == 0 || event.getHeight() == 0)
        {
            m_minimized = true;
            return false;
        }

        m_minimized = false;
        return false;
    }

}
