//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Config.h"
#include "core/Logging.h"

#include "threads/MainRenderThread.h"

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
            filesystem::setCurrentWorkingDirectory(m_specification.working_directory);

        logging::initClient(m_specification.logger_name);

        const auto window_settings = window_properties ? *window_properties : WindowProperties(m_specification.name);
        m_window = Window::create(window_settings, m_specification.api);

        m_window->setEventManager(m_event_manager);
        m_input = Input::create(m_window.get());

        createThreads();
    }

    Application::~Application()
    {
        cleanupThreads();
    }

    void Application::run()
    {
        for (const auto& thread : m_threads)
            thread->run();

        while (m_running)
        {
            double update_timestep = getUpdateTimestep();
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
            }

            //  Poll events
            m_event_manager.dispatchEvents();
            m_window->onUpdate();
        }
    }

    void Application::createThreads()
    {
        m_threads.emplace_back(createScope<threads::MainRenderThread>());

        spawnThreads();

        for (const auto& thread : m_threads)
            thread->waitReady();
    }

    void Application::cleanupThreads()
    {
        for (const auto& thread : m_threads)
            thread->shutdown();

        m_threads.clear();
    }

    void Application::spawnThreads() const
    {
        for (const auto& thread : m_threads)
            thread->spawn();
    }

    void Application::closeThreads() const
    {
        for (const auto& thread : m_threads)
            thread->close();
    }

    void Application::close()
    {
        m_running = false;
        closeThreads();
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

    void Application::reloadEngineConfig(const std::string& path)
    {
        Config::reloadEngineConfig(path);
        NB_CORE_INFO("Reloaded engine config.");
    }

    filesystem::Path Application::getResourcesPath(const bool absolute)
    {
        auto resources_dir = Config::getEngineConfig()["resources"]["resources_directory"].as<std::string>();
        if (absolute)
            return filesystem::getCurrentWorkingDirectory() / resources_dir;
        return resources_dir;
    }

}
