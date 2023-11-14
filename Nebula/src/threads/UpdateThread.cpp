//
// Created by michal-swiatek on 14.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "threads/UpdateThread.h"

#include "core/Timestep.h"
#include "core/Application.h"

namespace nebula::threads {

    void UpdateThread::run()
    {
        init();
        auto& application = Application::get();

        while (m_running.test(std::memory_order_relaxed))
        {
            int fps = application.getRenderFps();
            double render_timestep = fps > 0 ? 1.0 / fps : 0.0;
            double update_timestep = application.getUpdateTimestep();

            auto next_frame_time = application.getTime() + render_timestep;
            auto frame_time = m_update_timer.elapsedSeconds(true);
            m_update_accumulator += frame_time;

            if (!m_minimized.test(std::memory_order_relaxed))
            {
                for (const auto& layer : application.m_layer_stack)
                    layer->onUpdate(Timestep(frame_time));

                while (m_update_accumulator > update_timestep)
                {
                    for (const auto& layer : application.m_layer_stack)
                        layer->onFixedUpdate(Timestep(update_timestep));

                    m_update_accumulator -= update_timestep;
                }
            }

            application.m_event_manager.dispatchEvents();

            Timer::sleepUntilPrecise(next_frame_time);
        }

        shutdown();
    }

    void UpdateThread::init()
    {

    }

    void UpdateThread::shutdown()
    {

    }

    void UpdateThread::close()
    {
        m_running.clear(std::memory_order_release);
    }

    void UpdateThread::minimize(const bool minimize)
    {
        if (minimize)
            m_minimized.test_and_set(std::memory_order_acquire);
        else
            m_minimized.clear(std::memory_order_release);
    }

}

