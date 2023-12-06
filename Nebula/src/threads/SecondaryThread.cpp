//
// Created by michal-swiatek on 06.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "threads/SecondaryThread.h"

#include "core/Logging.h"

namespace nebula::threads {

    SecondaryThread::SecondaryThread(std::string name) : m_name(std::move(name)) {}

    SecondaryThread::~SecondaryThread()
    {
        m_thread.join();
    }

    void SecondaryThread::spawn()
    {
        m_thread = std::thread(&SecondaryThread::mainLoop, this);
    }

    void SecondaryThread::waitReady() const
    {
        m_ready.wait(false);
    }

    void SecondaryThread::run()
    {
        m_running.test_and_set(std::memory_order_acquire);
        m_running.notify_all();
    }

    void SecondaryThread::close()
    {
        m_running.clear(std::memory_order_release);
    }

    void SecondaryThread::mainLoop()
    {
        logging::ThreadFormatterFlag::addThreadName(std::this_thread::get_id(), m_name);

        init();

        m_ready.test_and_set(std::memory_order_acquire);
        m_ready.notify_all();

        m_running.wait(false);
        while (m_running.test(std::memory_order_relaxed))
            mainLoopBody();
    }

}
