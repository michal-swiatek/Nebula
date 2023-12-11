//
// Created by michal-swiatek on 06.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "threads/MainRenderThread.h"

#include "core/Timer.h"
#include "core/Config.h"
#include "core/Application.h"
#include "core/UpdateContext.h"

#include "rendering/renderer/RendererAPI.h"

using namespace nebula::rendering;

namespace nebula {

    namespace rendering {

        RenderContext* RenderContext::s_instance = nullptr;

        RenderContext::RenderContext()
        {
            NB_CORE_ASSERT(!s_instance);
            s_instance = this;

            auto& engine_config = Config::getEngineConfig();
            m_frames_in_flight_number = engine_config["rendering"]["frames_in_flight"].as<uint32_t>();
        }

        RenderContext::~RenderContext()
        {
            NB_CORE_ASSERT(s_instance);
            s_instance = nullptr;
        }

    }

    namespace threads {

        class FinalRenderPass final : public RenderPassTemplate
        {
        public:
            explicit FinalRenderPass(const Reference<FramebufferTemplate>& final_framebuffer_template) :
                RenderPassTemplate(ClearColor(0.0f, 0.0f, 0.1f, 1.0f), final_framebuffer_template)
            {
                AttachmentReference attachment_reference = {0};
                const auto shader = Shader::create("triangle", VertexShader("vulkan/triangle_shader.vert.spv", "vulkan/triangle_shader.frag.spv"));

                addStage(GraphicsPipelineState(shader), {attachment_reference});
            }
        };

        MainRenderThread::MainRenderThread() : SecondaryThread("RenderThread"), m_application(Application::get()) {}

        void MainRenderThread::mainLoopBody()
        {
            const uint32_t render_fps = m_render_context->getRenderFps();
            const double render_timestep = render_fps > 0 ? 1.0 / render_fps : 0.0;
            const double next_frame_time = UpdateContext::get().getTime() + render_timestep;

            const uint32_t frame_in_flight = m_render_context->getCurrentRenderFrame();

            if (!m_application.minimized())
            {
                m_render_context->waitForFrameResources(frame_in_flight);
                const auto framebuffer = m_render_context->getNextImage();

                if (!framebuffer)
                {
                    m_render_context->reload();
                    initFinalRenderpass();
                }
                else
                {
                    m_renderpass_executor->resetResources(frame_in_flight);
                    m_renderpass_executor->setFramebuffer(framebuffer);

                    auto final_commands = m_renderpass_executor->execute(m_renderpass_objects, frame_in_flight);
                    auto commands_executor = m_render_context->getCommandExecutor();

                    updateApplicationStack();

                    //  Submit commands
                    commands_executor->executeCommands(std::move(final_commands));
                    commands_executor->submitCommands();

                    m_render_context->presentImage();
                }
            }

            Timer::sleepUntilPrecise(next_frame_time);
        }

        void MainRenderThread::updateApplicationStack() const
        {
            for (const auto& layer : m_application.m_layer_stack)
                layer->onRender();

            for (const auto& layer : m_application.m_layer_stack)
                layer->onImGuiRender();
        }

        void MainRenderThread::init()
        {
            const Window& window = Application::getWindow();

            //  Init engine
            m_render_context = RenderContext::create(window.getWindowHandle());
            m_render_context->bind();

            RendererApi::create(m_application.getRenderingAPI());

            initFinalRenderpass();
            m_renderpass_objects.setStages(1);
        }

        void MainRenderThread::shutdown()
        {
            for (uint32_t frame_in_flight = 0; frame_in_flight < m_render_context->getFramesInFlightNumber(); ++frame_in_flight)
                m_render_context->waitForFrameResources(frame_in_flight);

            m_renderpass_executor.reset();

            RendererApi::destroy();

            m_render_context->unbind();
            m_render_context.reset();
        }

        void MainRenderThread::initFinalRenderpass()
        {
            const auto& swapchain_framebuffer_template = m_render_context->viewFramebufferTemplate();
            const auto renderpass_template = createReference<FinalRenderPass>(swapchain_framebuffer_template);
            auto renderer = Renderer::create<Renderer, ForwardRendererBackend>();

            renderer->setRenderPass(renderpass_template);
            m_renderpass_executor = RenderPassExecutor::create(std::move(renderer));
        }

    }

}
