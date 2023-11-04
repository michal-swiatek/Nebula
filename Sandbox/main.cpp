//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "Nebula.h"

class Sandbox : public nebula::Application
{
public:
    Sandbox() : nebula::Application("Sandbox", "SANDBOX")
    {
        NB_TRACE("Hello sandbox!");
    }
};

nebula::Application* nebula::createApplication(int argc, char** argv)
{
    return new Sandbox();
}
