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
            int update_fps = application.getUpdateFps();
            update_fps = static_cast<int>(update_fps * 1.035);   //  TODO: Remove hack and resolve timer resolution problem
            double timestep = update_fps > 0 ? 1.0 / update_fps : 0.0;

            auto frame_time = m_frame_timer.elapsedSeconds(true);
            m_update_accumulator += frame_time;

            if (!m_minimized.test(std::memory_order_relaxed))
            {
                for (const auto& layer : application.m_layer_stack)
                    layer->onUpdate(Timestep(frame_time));

                while (m_update_accumulator > timestep)
                {
                    for (const auto& layer : application.m_layer_stack)
                        layer->onFixedUpdate(Timestep(timestep));

                    m_update_accumulator -= timestep;
                }
            }

            double wait_time = std::max(timestep - m_frame_timer.elapsedSeconds(), 0.0);
            Timer::sleep(wait_time);
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

