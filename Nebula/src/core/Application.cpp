//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Application.h"

#include <filesystem>

#include "core/Config.h"
#include "core/Logging.h"

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

        m_render_context = rendering::RenderContext::create(m_window->getWindowHandle());
    }

    void Application::run()
    {
        while (m_running)
        {
            int render_fps = getRenderFps();
            double render_timestep = render_fps > 0 ? 1.0 / render_fps : 0.0;
            double update_timestep = getUpdateTimestep();

            auto next_frame_time = getTime() + render_timestep;
            auto frame_time = m_update_timer.elapsedSeconds(true);
            m_update_accumulator += frame_time;

            if (!m_minimized)
            {
                for (const auto& layer : m_layer_stack)
                    layer->onUpdate(Timestep(frame_time));

                while (m_update_accumulator > update_timestep)
                {
                    for (const auto& layer : m_layer_stack)
                        layer->onFixedUpdate(Timestep(update_timestep));

                    m_update_accumulator -= update_timestep;
                }

                for (const auto& layer : m_layer_stack)
                    layer->onRender();

                for (const auto& layer : m_layer_stack)
                    layer->onImGuiRender();
            }

            //  Poll events
            m_event_manager.dispatchEvents();
            m_render_context->swapBuffers();
            m_window->onUpdate();

            Timer::sleepUntilPrecise(next_frame_time);
        }
    }

    void Application::close()
    {
        m_running = false;
    }

    void Application::minimize(bool minimized)
    {
        m_minimized = minimized;
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

        return false;
    }

}
