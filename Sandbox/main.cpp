//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "Nebula.h"

using namespace nebula;

class ExampleLayer : public Layer
{
public:
    ExampleLayer() : Layer("Example Layer") {}

    void onAttach() override
    {
        NB_INFO("{} created!", getName());
    }

    void onDetach() override
    {
        NB_INFO("{} destroyed.", getName());
    }

    void onEvent(nebula::Event &event) override
    {
        EventDelegate delegate(event);

        delegate.delegate<MouseMovedEvent>([this](MouseMovedEvent& event){ NB_INFO(event.toString()); return true; });
        delegate.delegate<WindowResizeEvent>([this](WindowResizeEvent& event){ NB_INFO(event.toString()); return true; });
    }
};

class Sandbox : public Application
{
public:
    Sandbox() : Application("Sandbox", "SANDBOX")
    {
        NB_TRACE("Hello sandbox!");
        m_example_layer_id = pushLayer<ExampleLayer>();
        pushLayer<ExampleLayer>();
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
