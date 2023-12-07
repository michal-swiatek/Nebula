//
// Created by michal-swiatek on 06.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "threads/MainRenderThread.h"

#include "core/Timer.h"
#include "core/Application.h"

#include "rendering/renderer/RendererAPI.h"

using namespace nebula::rendering;

namespace nebula::threads {

    class FinalRenderPass final : public RenderPassTemplate
    {
    public:
        explicit FinalRenderPass(const Reference<FramebufferTemplate>& final_framebuffer_template) :
            RenderPassTemplate(ClearColor(0.0f, 0.0f, 0.2f, 1.0f), final_framebuffer_template)
        {
            AttachmentReference attachment_reference = {0};
            const auto shader = Shader::create("triangle", VertexShader("vulkan/triangle_shader.vert.spv", "vulkan/triangle_shader.frag.spv"));

            addStage(GraphicsPipelineState(shader), {attachment_reference});
        }
    };

    MainRenderThread::MainRenderThread() : SecondaryThread("RenderThread"), m_application(Application::get()) {}

    void MainRenderThread::mainLoopBody()
    {
        const int render_fps = m_application.getRenderFps();
        const double render_timestep = render_fps > 0 ? 1.0 / render_fps : 0.0;
        const double next_frame_time = m_application.getTime() + render_timestep;

        if (!m_application.minimized())
        {
            for (const auto& layer : m_application.m_layer_stack)
                layer->onRender();

            for (const auto& layer : m_application.m_layer_stack)
                layer->onImGuiRender();

            m_render_context->swapBuffers();
        }

        Timer::sleepUntilPrecise(next_frame_time);
    }

    void MainRenderThread::init()
    {
        const Window& window = Application::getWindow();

        //  Init engine
        m_render_context = RenderContext::create(window.getWindowHandle());
        RendererApi::create(m_application.getRenderingAPI());

        //  Init members
        const auto renderpass_template = createReference<FinalRenderPass>(m_render_context->viewFramebufferTemplate());
        auto renderer = Renderer::create<Renderer, ForwardRendererBackend>();

        renderer->setRenderPass(renderpass_template);
        m_renderpass_objects.setStages(renderpass_template->viewRenderStages().size());

        m_renderpass_executor = createScope<RenderPassExecutor>(std::move(renderer));
    }

    void MainRenderThread::shutdown()
    {
        m_renderpass_executor.reset();

        RendererApi::destroy();
        m_render_context.reset();
    }

}
