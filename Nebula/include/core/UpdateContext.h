//
// Created by michal-swiatek on 08.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef UPDATECONTEXT_H
#define UPDATECONTEXT_H

#include <atomic>

#include "core/Core.h"
#include "core/Timer.h"
#include "core/Types.h"

namespace nebula {

    namespace threads { class MainUpdateThread; }

    class NEBULA_API UpdateContext
    {
    public:
        ~UpdateContext();

        [[nodiscard]] double getTime() { return m_application_timer.elapsedSeconds(); }

        void setUpdateTimestep(const double seconds) { m_current_update_timestep.store(seconds); }
        [[nodiscard]] Timestep getUpdateTimestep() const { return Timestep(m_current_update_timestep.load()); }
        [[nodiscard]] std::atomic_uint32_t getCurrentUpdateFrame() const { return m_current_update_frame.load(); }

        static UpdateContext& get() { return *s_instance; }

    private:
        UpdateContext();    //  Defined in MainUpdateThread

        std::atomic_uint32_t m_current_update_frame;
        std::atomic<double> m_current_update_timestep = 0.02;

        Timer m_application_timer;

        static UpdateContext* s_instance;

        friend class nebula::threads::MainUpdateThread;
        static Scope<UpdateContext> create();
    };

}

#endif //UPDATECONTEXT_H
