//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "Nebula.h"

#include <thread>

using namespace nebula;
using namespace std::chrono_literals;

auto application_specification = ApplicationSpecification("Sandbox", "APP", "", 144, 120);
auto window_properties = WindowProperties("Sandbox", 1600, 900, false, renderer::API::cOpenGL);

class ExampleLayer : public Layer
{
public:
    ExampleLayer() : Layer("Example Layer") {}

    void onUpdate(Timestep delta_time) override
    {
        if (Input::isKeyPressed(Keycode::Space))
            NB_INFO("Space is being pressed!");
    }

    void onFixedUpdate(Timestep delta_time) override
    {

    }

    void onRender() override
    {

    }

    void onAttach() override
    {
        NB_INFO("{} created!", getName());
    }

    void onDetach() override
    {
        NB_INFO("{} destroyed.", getName());
    }

    void onEvent(Event &event) override
    {
        EventDelegate delegate(event);

        delegate.delegate<KeyPressedEvent>(NB_BIND_EVENT_FUNCTION(ExampleLayer::onKeyPressed));
    }

    bool onKeyPressed(KeyPressedEvent& event)
    {
        if (event.getKeycode() == Keycode::Escape)
            Application::get().close();
        else if (event.getKeycode() == Keycode::R)
        {
            NB_TRACE("Reloading render context");
            Application::get().setRenderingAPI(renderer::API::cOpenGL);
        }

        return true;
    }
};

class Sandbox : public Application
{
public:
    Sandbox() : Application(application_specification, window_properties)
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
