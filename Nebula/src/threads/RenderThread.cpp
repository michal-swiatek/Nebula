//
// Created by michal-swiatek on 13.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "threads/RenderThread.h"

#include <condition_variable>

#include <glad/glad.h>

#include "core/Application.h"
#include "renderer/Renderer.h"
#include "renderer/utils/RendererCommands.h"
#include "debug/ImGuiLayer.h"

namespace nebula::threads {

    using namespace rendering;
    using namespace rendering::impl;

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
            render_fps = static_cast<int>(render_fps * 1.035);   //  TODO: Remove hack and resolve timer resolution problem
            double timestep = render_fps > 0 ? 1.0 / render_fps : 0.0;

            auto frame_time = m_frame_timer.elapsedSeconds(true);
            auto frame = m_frame_queue.pop(0);

            NB_CORE_INFO("Number of frames in queue: {}", m_frame_queue.size());

            if (api = m_api_change.load(std::memory_order_relaxed); api != API::cUndefined)
                setRenderingAPI(api);

            if (!m_minimized.test(std::memory_order_relaxed))
            {
                if (frame)
                {
                    NB_CORE_ASSERT(*frame, "Invalid frame!");
                    NB_CORE_INFO("OK");

                    (*frame)->getRenderPasses().dispatchRenderPasses();

                    NB_CORE_INFO("OK2");

                    for (const auto& layer : application.m_layer_stack)
                        layer->onRender();
                }
                else
                    ClearColorCommand(0.2f, 0.2f, 0.2f, 1.0f).execute();

                ImGuiLayer::begin();
                NB_CORE_INFO("OK3");

                for (const auto& layer : application.m_layer_stack)
                    layer->onImGuiRender();

                ImGuiLayer::end();

                m_render_context->swapBuffers();
            }

            double wait_time = std::max(timestep - m_frame_timer.elapsedSeconds(), 0.0);
            Timer::sleep(wait_time);
        }

        shutdown();
    }

    void RenderThread::submitFrame(Scope<rendering::impl::Frame>&& frame)
    {
        m_frame_queue.push(std::move(frame));
    }

    void RenderThread::init(const API api)
    {
        auto& application = Application::get();
        void* window_handle = application.getWindow().getWindowHandle();

        m_render_context = RenderContext::create(api, window_handle);
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

    void RenderThread::setRenderingAPI(const API api)
    {
        void* window_handle = Application::get().getWindow().getWindowHandle();
        m_render_context = RenderContext::create(api, window_handle);
        Renderer::setRenderingApi(api);

        m_api_change.store(API::cUndefined);
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

    void RenderThread::changeAPI(const API api)
    {
        NB_CORE_ASSERT(api != API::cUndefined, "Trying to change render API to Undefined API!");
        m_api_change.store(api);
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