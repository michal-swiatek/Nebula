//
// Created by michal-swiatek on 10.06.2022.
// Github: https://github.com/michal-swiatek/
//

#include "Application.h"

#include <iostream>

namespace nebula {

    Application::Application() = default;

    Application::~Application() = default;

    void Application::Run()
    {
        std::cout << "Welcome to Nebula!\n";
    }

}
