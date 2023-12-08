//
// Created by michal-swiatek on 06.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef MAINUPDATETHREAD_H
#define MAINUPDATETHREAD_H

#include "SecondaryThread.h"

#include "core/Timer.h"
#include "core/Application.h"
#include "core/UpdateContext.h"

namespace nebula::threads {

    class MainUpdateThread final : public SecondaryThread
    {
    public:
        MainUpdateThread();

    private:
        Application& m_application;
        Scope<UpdateContext> m_update_context;

        Timer m_update_timer;
        double m_update_accumulator = 0.0;

        void init() override;
        void shutdown() override;

        void mainLoopBody() override;
    };

}

#endif //MAINUPDATETHREAD_H
