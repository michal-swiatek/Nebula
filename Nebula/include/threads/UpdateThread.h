//
// Created by michal-swiatek on 14.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <atomic>

#include "core/Types.h"
#include "core/Timer.h"
#include "threads/BlockingQueue.h"
#include "events/ApplicationEvents.h"

#include "renderer/Frame.h"
#include "renderer/RendererAPI.h"
#include "renderer/RenderContext.h"

namespace nebula::threads {

    class UpdateThread
    {
    public:
        void run();

        void close();
        void minimize(bool minimize);

    private:
        Timer m_update_timer;
        double m_update_accumulator = 0.0;

        std::atomic_flag m_running = true;
        std::atomic_flag m_minimized = false;

        void init();
        void shutdown();
    };

}

#endif //UPDATETHREAD_H
