//
// Created by michal-swiatek on 06.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "threads/MainUpdateThread.h"

namespace nebula::threads {

    MainUpdateThread::MainUpdateThread() : SecondaryThread("UpdateThread"), m_application(Application::get()) {}

    void MainUpdateThread::mainLoopBody()
    {
        const int render_fps = m_application.getRenderFps();
        const double render_timestep = render_fps > 0 ? 1.0 / render_fps : 0.0;
        const double next_frame_time = m_application.getTime() + render_timestep;

        const double update_timestep = m_application.getUpdateTimestep();
        const double frame_time = m_update_timer.elapsedSeconds(true);

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

        m_application.m_event_manager.dispatchEvents();
        Timer::sleepUntilPrecise(next_frame_time);
    }

    void MainUpdateThread::init()
    {

    }

    void MainUpdateThread::shutdown()
    {

    }

}
