//
// Created by michal-swiatek on 10.06.2022.
// Github: https://github.com/michal-swiatek/
//

#include "Application.h"

namespace nebula {

    Application::Application() = default;

    Application::~Application() = default;

    void Application::Run()
    {
        CORE_LOGGER_INFO("Welcome to Nebula engine!");
    }

}
