//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "Nebula.h"

#include <vector>

using namespace nebula;
using namespace nebula::literals;
using namespace std::chrono_literals;

auto application_specification = ApplicationSpecification("Sandbox", "APP", "", 144, 120);
auto window_properties = WindowProperties("Sandbox", 1600, 900, false, rendering::API::cOpenGL);

struct Temp
{
    int number;
    char string[100];

    explicit Temp(int number) : number(number), string{"Hello world!"} {}
};

struct Temp2 : public Temp
{
    explicit Temp2(int number) : Temp(number) {}
    ~Temp2() { NB_TRACE("Temp2 {} deleted!", number); }
};

class ExampleLayer : public Layer
{
public:
    ExampleLayer() :
        Layer("Example Layer"),
        m_allocator(memory::MemoryManager::requestMemory(3224), 3224),
        m_vector(m_allocator)
    {}

    void onUpdate(Timestep delta_time) override
    {
        // if (Input::isKeyPressed(Keycode::Space))
        //     NB_INFO("Space is being pressed!");
    }

    void onFixedUpdate(Timestep delta_time) override
    {
        // memory::ScopedAllocator<memory::StackAllocator, 10_Kb> allocator{};
        //
        // auto* temp = allocator.create<Temp>(5);
        // NB_INFO("Succesfully created Temp object: {}, {}", temp->number, temp->string);
        //
        // for (int i = 0; i < 10; ++i)
        //     m_vector.push_back(Temp(i));
        //
        // for (const auto& t : m_vector)
        //     NB_INFO("Temp object: {}, {}", t.number, t.string);
        //
        // NB_INFO("Used memory: {}", m_allocator.getUsedMemory());
        //
        // m_vector.clear();
        // m_vector.shrink_to_fit();
        // m_allocator.clear();
        //
        // auto temp2 = createReference<Temp2>(5);
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

    bool onKeyPressed(const KeyPressedEvent& event)
    {
        if (event.getKeycode() == Keycode::Escape)
            Application::get().close();
        else if (event.getKeycode() == Keycode::R)
        {
            NB_TRACE("Reloading render API");
            Application::get().setRenderingAPI(rendering::API::cOpenGL);
        }

        return true;
    }

private:
    memory::LinearAllocator m_allocator;
    std::vector<Temp, memory::STLAdapter<Temp, memory::LinearAllocator>> m_vector;
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
