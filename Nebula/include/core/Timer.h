//
// Created by michal-swiatek on 05.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_TIMER_H
#define NEBULAENGINE_TIMER_H

#include "core/Core.h"

namespace nebula {

    void system_sleep(double seconds); //  Implemented in platform/DetectPlatform.cpp

    class NEBULA_API Timer
    {
    public:
        explicit Timer(double start_time = 0.0)
        {
            set(start_time);
        }

        void reset() { set(); }
        void set(double time_seconds = 0.0)
        {
            m_start = std::chrono::high_resolution_clock::now();
            m_start -= std::chrono::nanoseconds(static_cast<std::size_t>(time_seconds * 1'000'000'000));
        }

        template <typename Duration>
        [[nodiscard]] std::size_t elapsed() const
        {
            return std::chrono::duration_cast<Duration>(std::chrono::high_resolution_clock::now() - m_start).count();
        }

        double elapsedSeconds(bool reset_timer = false)      { return deltaTime(0.000'000'001, reset_timer); }
        double elapsedMilliSeconds(bool reset_timer = false) { return deltaTime(0.000'001, reset_timer); }
        double elapsedMicroSeconds(bool reset_timer = false) { return deltaTime(0.001, reset_timer); }
        double elapsedNanoSeconds(bool reset_timer = false)  { return deltaTime(1, reset_timer); }

        //  Implemented in core/Timer.cpp
        static void sleep(double seconds);
        static void sleepUntil(double application_time);

        static void sleepPrecise(double seconds, double busy_offset = 0.001);
        static void sleepUntilPrecise(double application_time, double busy_offset = 0.001);

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start{};

        double deltaTime(double multiplier, bool reset_timer)
        {
            auto delta_time = static_cast<double>(elapsed<std::chrono::nanoseconds>()) * multiplier;

            if (reset_timer)
                reset();

            return delta_time;
        }
    };

}

#endif //NEBULAENGINE_TIMER_H
