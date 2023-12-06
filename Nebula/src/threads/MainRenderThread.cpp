//
// Created by michal-swiatek on 06.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "threads/MainRenderThread.h"

#include "core/Timer.h"
#include "core/Logging.h"
#include "core/Application.h"

#include "rendering/renderer/RendererAPI.h"

using namespace nebula::rendering;

namespace nebula::threads {

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
        NB_CORE_INFO("Initializing MainRenderThread...");

        const Application& application = Application::get();
        const Window& window = application.getWindow();

        m_render_context = RenderContext::create(window.getWindowHandle());
        m_renderer = Renderer::create<Renderer, ForwardRendererBackend>();

        RendererApi::create(application.getRenderingAPI());
    }

    void MainRenderThread::shutdown()
    {
        NB_CORE_INFO("Shutting down MainRenderThread...");

        RendererApi::destroy();
        m_render_context.reset();
    }

}
