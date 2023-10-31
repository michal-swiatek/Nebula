//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "Application.h"

#include <iostream>
#include "OpenGLConfiguration.h"

namespace nebula {

    Application::Application(std::string name) : m_name(std::move(name))
    {

    }

    void Application::run()
    {
        std::cout << "Using OpenGL version: " << OPENGL_MAJOR_VERSION << '.' << OPENGL_MINOR_VERSION << '\n';
    }

}
