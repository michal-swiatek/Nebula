//
// Created by michal-swiatek on 06.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "threads/MainRenderThread.h"

#include "core/Timer.h"
#include "core/Config.h"
#include "core/Application.h"
#include "core/UpdateContext.h"
#include "debug/ImGuiLayer.h"
#include "debug/ImGuiBackend.h"

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
            const uint32_t render_fps = m_render_context->getRenderFps();
            const double render_timestep = render_fps > 0 ? 1.0 / render_fps : 0.0;
            const double next_frame_time = UpdateContext::get().getTime() + render_timestep;

            const uint32_t frame_in_flight = m_render_context->getCurrentRenderFrame();

            if (!m_application.minimized())
            {
                m_render_context->waitForFrameResources(frame_in_flight);
                if (m_vsync != m_render_context->checkVSync())
                    reloadSwapchain();

                const auto framebuffer = m_render_context->getNextImage();

                if (!framebuffer)
                    reloadSwapchain();
                else
                {
                    m_renderpass_executor->resetResources(frame_in_flight);
                    m_renderpass_executor->setFramebuffer(framebuffer);

                    auto final_commands = m_renderpass_executor->execute(m_renderpass_objects, frame_in_flight);
                    auto commands_executor = m_render_context->getCommandExecutor();

                    //  Submit commands
                    commands_executor->executeCommands(std::move(final_commands));
                    commands_executor->submitCommands();

                    m_render_context->presentImage();
                }
            }

            if (render_fps > 0)
                Timer::sleepUntilPrecise(next_frame_time);
        }

        void MainRenderThread::init()
        {
            const Window& window = Application::getWindow();

            //  Init engine
            m_render_context = RenderContext::create(window.getWindowHandle());
            m_render_context->bind();

            RendererApi::create(m_application.getRenderingAPI());
            ImGuiBackend::init();

            initFinalRenderpass(true);

            m_imgui_object = createScope<ImGuiRenderObject>();
            m_renderpass_objects.setStages(1);
            m_renderpass_objects.addObject(0, m_imgui_object.get());
        }

        void MainRenderThread::shutdown()
        {
            for (uint32_t frame_in_flight = 0; frame_in_flight < m_render_context->getFramesInFlightNumber(); ++frame_in_flight)
                m_render_context->waitForFrameResources(frame_in_flight);

            m_application.popOverlay(m_im_gui_layer->getID());
            m_renderpass_executor.reset();

            ImGuiBackend::shutdown();
            RendererApi::destroy();

            m_render_context->unbind();
            m_render_context.reset();
        }

        void MainRenderThread::reloadSwapchain()
        {
            m_vsync = m_render_context->checkVSync();
            m_render_context->reload();
            initFinalRenderpass();
        }

        void MainRenderThread::initFinalRenderpass(const bool setup_imgui_layer)
        {
            m_render_context->setVSync(m_vsync);

            const auto& swapchain_framebuffer_template = m_render_context->viewFramebufferTemplate();
            const auto renderpass_template = createReference<FinalRenderPass>(swapchain_framebuffer_template);
            auto renderer = Renderer::create<Renderer, ForwardRendererBackend>();
            auto renderpass = RenderPass::create(renderpass_template);

            if (setup_imgui_layer)
            {
                const auto id = m_application.pushOverlay<ImGuiLayer>(*renderpass);
                m_im_gui_layer = dynamic_cast<ImGuiLayer*>(m_application.m_layer_stack.getOverlay(id));
            }
            else
                m_im_gui_layer->reloadBackend(*renderpass);

            renderer->setRenderPass(std::move(renderpass));
            m_renderpass_executor = RenderPassExecutor::create(std::move(renderer));
        }

    }

}
