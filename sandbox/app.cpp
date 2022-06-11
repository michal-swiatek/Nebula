//
// Created by michal-swiatek on 10.06.2022.
// Github: https://github.com/michal-swiatek/
//

#include "Nebula.h"

class App : public nebula::Application
{
public:
    App() : nebula::Application()
    {
    }

    ~App() override
    {
    }
};

nebula::Application *nebula::CreateApplication()
{
    return new App();
}
