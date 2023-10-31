//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include <memory>

#include "Nebula.h"

class Sandbox : public nebula::Application
{
public:
    Sandbox() : nebula::Application("Sandbox") {}
};

nebula::Application* nebula::createApplication()
{
    return new Sandbox();
}
