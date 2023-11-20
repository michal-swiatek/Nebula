//
// Created by michal-swiatek on 13.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "threads/RenderThread.h"

#include <glad/glad.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "core/Application.h"
#include "renderer/Renderer.h"
#include "renderer/utils/RendererCommands.h"
#include "debug/ImGuiLayer.h"

namespace nebula::threads {

    using namespace rendering;

    std::atomic_flag initialized = false;

    void RenderThread::run(API api)
    {
        init(api);
        auto& application = Application::get();
        auto& window = application.getWindow();

        window.setVSync(window.checkVSync());

        while (m_running.test(std::memory_order_relaxed))
        {
            int render_fps = application.getRenderFps();
            double render_timestep = render_fps > 0 ? 1.0 / render_fps : 0.0;

            auto next_frame_time = application.getTime() + render_timestep;
            auto frame = m_frame_queue.pop(0);

            if (!m_minimized.test(std::memory_order_relaxed))
            {
                if (frame)
                    m_current_frame = std::move(*frame);

                if (m_current_frame)
                {
                    m_current_frame->getRenderPasses().dispatchRenderPasses();  //  TODO: Cache render result

                    for (const auto& layer : application.m_layer_stack)
                        layer->onRender();
                }
                else
                    ClearColorCommand(0.2f, 0.2f, 0.2f, 1.0f).execute();

                ImGuiLayer::begin();

                for (const auto& layer : application.m_layer_stack)
                    layer->onImGuiRender();

                ImGuiLayer::end();

                m_render_context->swapBuffers();
            }

            Timer::sleepUntilPrecise(next_frame_time);
        }

        shutdown();
    }

    void RenderThread::submitFrame(Scope<rendering::Frame>&& frame)
    {
        m_frame_queue.push(std::move(frame));
    }

    void RenderThread::init(const API api)
    {
        auto& application = Application::get();

        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
        NB_CORE_INFO("GLFW support for Vulkan: {}", glfwVulkanSupported());
        NB_CORE_INFO("Number of supported Vulkan extensions: {}", extension_count);

        m_render_context = RenderContext::create(application.getWindow().getWindowHandle());
        Renderer::init(api, this);

        application.pushOverlay<ImGuiLayer>();

        initialized.test_and_set();
        initialized.notify_one();
    }

    void RenderThread::shutdown()
    {
        Renderer::shutdown();
        m_render_context.reset();
    }

    void RenderThread::close()
    {
        m_running.clear(std::memory_order_release);
    }

    void RenderThread::minimize(const bool minimize)
    {
        if (minimize)
            m_minimized.test_and_set(std::memory_order_acquire);
        else
            m_minimized.clear(std::memory_order_release);
    }

    void RenderThread::onWindowResize(WindowResizeEvent& event) const
    {
        glViewport(0, 0, static_cast<int>(event.getWidth()), static_cast<int>(event.getHeight()));
    }

    void RenderThread::blockUntilInitialized()
    {
        initialized.wait(false);
    }

}
