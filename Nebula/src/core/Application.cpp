//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Application.h"

#include "core/Logging.h"
#include "events/MouseEvents.h"
#include "events/KeyboardEvents.h"
#include "events/ApplicationEvents.h"

#include "OpenGLConfiguration.h"

namespace nebula {

    Application::Application(std::string name, const std::string& logger_name)
        : m_name(std::move(name)),
          m_event_manager(m_layer_stack, [this](Event& event) { onEvent(event); })
    {
        NB_CORE_INFO("OpenGL version: {}.{}", OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
        logging::initClient(logger_name);

        m_event_manager.queueEvent<MouseMovedEvent>(3.14, -42);
        m_event_manager.queueEvent<WindowResizeEvent>(100, 100);
    }

    void Application::onEvent(Event& event)
    {
        EventDelegate delegate(event);

        delegate.delegate<MouseMovedEvent>([this](MouseMovedEvent& event){ NB_CORE_INFO(event.toString()); return false; });
        delegate.delegate<WindowResizeEvent>([this](WindowResizeEvent& event){ NB_CORE_INFO(event.toString()); return true; });
    }

    void Application::run()
    {
        m_event_manager.dispatchEvents();
    }

}
