//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#include "Nebula.h"

#include <vector>

using namespace nebula;
using namespace nebula::rendering;
using namespace nebula::literals;
using namespace std::chrono_literals;

auto application_specification = ApplicationSpecification("Sandbox", "APP", "", 60, 0.02, API::cVulkan);
auto window_properties = WindowProperties("Sandbox", 1600, 900);

class TestFramebuffer final : public FramebufferTemplate
{
public:
    TestFramebuffer() : FramebufferTemplate(100, 100)
    {
        AttachmentDescription attachment;
        attachment.format = TextureFormat::cFormat_R8G8B8A8_SRGB;

        addTextureAttachment(attachment);
    }
};

class TestRenderPass final : public RenderPassTemplate
{
public:
    TestRenderPass() : RenderPassTemplate(ClearColor(0, 0, 0, 0), createReference<TestFramebuffer>())
    {
        AttachmentReference attachment_reference = {0};
        auto shader = Shader::create("triangle", VertexShader("vulkan/triangle_shader.vert.spv", "vulkan/triangle_shader.frag.spv"));

        addStage(GraphicsPipelineState(shader), {attachment_reference});
        addStage(GraphicsPipelineState(shader), {attachment_reference});
    }
};

class ExampleLayer : public Layer
{
public:
    ExampleLayer() : Layer("Example Layer")
    {
        setup();
    }

    void setup()
    {
        m_renderer->setRenderPass(createReference<TestRenderPass>());
    }

    void onUpdate(Timestep delta_time) override
    {
        if (Input::isKeyPressed(Keycode::Space))
            NB_INFO("Space is being pressed!");

        m_renderer->beginRenderPass();
        m_renderer->nextRenderStage();
        m_renderer->endRenderPass();

        const auto render_commands = m_renderer->viewCommandBuffer()->viewCommands();
        // NB_TRACE("Number of rendering commands in RenderCommandBuffer: {}", render_commands.size());
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

    bool onKeyPressed(const KeyPressedEvent& event)
    {
        if (event.getKeycode() == Keycode::Escape)
            Application::get().close();
        if (event.getKeycode() == Keycode::R)
            Application::get().reloadEngineConfig();

        return true;
    }

private:
    Scope<Renderer> m_renderer = Renderer::create<Renderer>(false);
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
