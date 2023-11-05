//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "Nebula.h"

#include <thread>
#include <chrono>

#include <unistd.h>

using namespace nebula;
using namespace std::chrono_literals;

auto application_specification = ApplicationSpecification("Sandbox", "APP");

class ExampleLayer : public Layer
{
public:
    ExampleLayer() : Layer("Example Layer") {}

    void onUpdate(nebula::Timestep delta_time) override
    {

    }

    void onFixedUpdate(nebula::Timestep delta_time) override
    {

    }

    void onRender() override
    {
        Timer busy_timer{};
        while (busy_timer.elapsedMilliSeconds() < 2);
    }

    void onAttach() override
    {
        NB_INFO("{} created!", getName());
    }

    void onDetach() override
    {
        NB_INFO("{} destroyed.", getName());
    }

    void onEvent(nebula::Event &event) override {}
};

class Sandbox : public Application
{
public:
    Sandbox() : Application(application_specification)
    {
        NB_TRACE("Hello sandbox!");
        m_example_layer_id = pushLayer<ExampleLayer>();
    }

    ~Sandbox() override
    {
        auto example_layer = popLayer(m_example_layer_id);
        if (example_layer)
            example_layer.reset();
        NB_TRACE("Sandbox destroyed.");
    }

private:
    LayerStack::LayerID m_example_layer_id;
};

Application* nebula::createApplication(int argc, char** argv)
{
    return new Sandbox();
}
