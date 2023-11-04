//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Application.h"

#include "core/Logging.h"
#include "OpenGLConfiguration.h"

namespace nebula {

    Application::Application(std::string name, const std::string& logger_name) : m_name(std::move(name))
    {
        NB_CORE_INFO("OpenGL version: {}.{}", OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
        logging::initClient(logger_name);
    }

    void Application::run()
    {

    }

}
