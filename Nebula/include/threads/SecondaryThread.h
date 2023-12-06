//
// Created by michal-swiatek on 06.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef SECONDARYTHREAD_H
#define SECONDARYTHREAD_H

#include <atomic>
#include <thread>

namespace nebula::threads {

    class SecondaryThread
    {
    public:
        explicit SecondaryThread(std::string name);
        virtual ~SecondaryThread();

        void spawn();
        void waitReady() const;

        void run();
        void close();

        virtual void shutdown() = 0;

    protected:
        virtual void init() = 0;
        virtual void mainLoopBody() = 0;

    private:
        std::string m_name;
        std::thread m_thread;
        std::atomic_flag m_ready = ATOMIC_FLAG_INIT;
        std::atomic_flag m_running = ATOMIC_FLAG_INIT;

        void mainLoop();
    };

}

#endif //SECONDARYTHREAD_H
