//
// Created by michal-swiatek on 13.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <atomic>

#include "core/Types.h"
#include "core/Timer.h"
#include "threads/BlockingQueue.h"
#include "events/ApplicationEvents.h"

#include "renderer/Frame.h"
#include "renderer/RendererAPI.h"
#include "renderer/RenderContext.h"

namespace nebula::threads {

    class RenderThread
    {
    public:
        void run(rendering::API api);

        void submitFrame(Scope<rendering::impl::Frame>&& frame);

        void close();
        void minimize(bool minimize);

        void changeAPI(rendering::API api);

        void onWindowResize(WindowResizeEvent& event) const;
        static void blockUntilInitialized();

    private:
        Timer m_render_timer;
        Scope<rendering::impl::Frame> m_current_frame = nullptr;
        Scope<rendering::impl::RenderContext> m_render_context = nullptr;

        std::atomic<rendering::API> m_api_change;
        std::atomic_flag m_running = true;
        std::atomic_flag m_minimized = false;

        BlockingQueue<Scope<rendering::impl::Frame>, 1> m_frame_queue;

        void init(rendering::API api);
        void shutdown();
        void setRenderingAPI(rendering::API api);
    };

}

#endif //RENDERTHREAD_H
