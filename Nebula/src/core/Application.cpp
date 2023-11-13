//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Application.h"

#include <filesystem>
#include <glad/glad.h>

#include "core/Config.h"
#include "core/Logging.h"
#include "debug/ImGuiLayer.h"

#include "renderer/Renderer.h"
#include "renderer/utils/RendererCommands.h"

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

        auto window_settings = window_properties ? *window_properties : WindowProperties(m_specification.name);
        m_window = Window::create(window_settings);

        m_window->setEventManager(m_event_manager);
        m_input = Input::create(m_window.get());

        m_render_manager = createScope<rendering::impl::RenderManager>();
        rendering::Renderer::init(window_settings.api, m_render_manager.get());

        m_imgui_layer = pushOverlay<ImGuiLayer>();
    }

    Application::~Application()
    {
        rendering::Renderer::shutdown();
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

                for (const auto& layer : m_layer_stack)
                    layer->onRender();

                auto render_context = m_window->getRenderContext();
                render_context->unbind();
                m_render_manager->render(*render_context);

                ImGuiLayer::begin();

                for (const auto& layer : m_layer_stack)
                    layer->onImGuiRender();

                ImGuiLayer::end();
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

    void Application::setRenderingAPI(rendering::API api)
    {
        m_window->setRenderContext(api);
        rendering::Renderer::setRenderingApi(api);
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

        glViewport(0, 0, int(event.getWidth()), int(event.getHeight()));
        return false;
    }

}
