//
// Created by michal-swiatek on 10.06.2022.
// Github: https://github.com/michal-swiatek/
//

#include "Nebula.h"

#include <iostream>

class App : public nebula::Application
{
public:
    App() : nebula::Application()
    {
        std::cout << "Creating custom application.\n";
    }

    ~App() override
    {
        std::cout << "Destroying custom application.\n";
    }
};

nebula::Application *nebula::CreateApplication()
{
    return new App();
}
