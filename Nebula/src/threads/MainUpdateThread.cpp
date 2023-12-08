//
// Created by michal-swiatek on 06.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "threads/MainUpdateThread.h"

#include "rendering/RenderContext.h"

namespace nebula {

    UpdateContext* UpdateContext::s_instance = nullptr;

    UpdateContext::UpdateContext()
    {
        NB_CORE_ASSERT(!s_instance);
        s_instance = this;
    }

    UpdateContext::~UpdateContext()
    {
        NB_CORE_ASSERT(s_instance);
        s_instance = nullptr;
    }

    Scope<UpdateContext> UpdateContext::create()
    {
        return createScopeFromPointer(new UpdateContext());
    }

    namespace threads {

        MainUpdateThread::MainUpdateThread() : SecondaryThread("UpdateThread"), m_application(Application::get()) {}

        void MainUpdateThread::mainLoopBody()
        {
            const uint32_t render_fps = rendering::RenderContext::get().getRenderFps();
            const double render_timestep = render_fps > 0 ? 1.0 / render_fps : 0.0;
            const double next_frame_time = m_update_context->getTime() + render_timestep;

            const double update_timestep = m_update_context->getUpdateTimestep();
            const double frame_time = m_update_timer.elapsedSeconds(true);

            m_application.m_event_manager.dispatchEvents();

            if (!m_application.minimized())
            {
                m_update_accumulator += frame_time;

                for (const auto& layer : m_application.m_layer_stack)
                    layer->onUpdate(Timestep(frame_time));

                while (m_update_accumulator > update_timestep)
                {
                    for (const auto& layer : m_application.m_layer_stack)
                        layer->onFixedUpdate(Timestep(update_timestep));

                    m_update_accumulator -= update_timestep;
                }
            }

            Timer::sleepUntilPrecise(next_frame_time);
        }

        void MainUpdateThread::init()
        {
            m_update_context = UpdateContext::create();
        }

        void MainUpdateThread::shutdown()
        {

        }

    }

}
