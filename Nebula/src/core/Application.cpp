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
            double update_timestep = 1.0 / m_specification.update_fps;
            double render_timestep = m_specification.render_fps > 0 ? 1.0 / m_specification.render_fps : 0.0;

            auto frame_time = m_timer.elapsedSeconds(true);
            m_update_accumulator += frame_time;

            if (!m_minimized)
            {
                for (auto& layer : m_layer_stack)
                    layer->onUpdate(Timestep(frame_time));

                while (m_update_accumulator > update_timestep)
                {
                    for (auto& layer : m_layer_stack)
                        layer->onFixedUpdate(Timestep(update_timestep));

                    m_update_accumulator -= update_timestep;
                }

                for (auto& layer : m_layer_stack)
                {
                    layer->onRender();
                    layer->onImGuiRender();
                }
            }

            //  Swap buffers and poll events
            m_window->onUpdate();
            m_event_manager.dispatchEvents();

            if (m_timer.elapsedSeconds() < render_timestep)
            {
                Timer busy_timer;
                double wait_time = render_timestep - m_timer.elapsedSeconds();
                while (busy_timer.elapsedSeconds() < wait_time);
            }
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
