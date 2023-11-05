//
// Created by michal-swiatek on 05.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_TIMER_H
#define NEBULAENGINE_TIMER_H

#include <chrono>

#include "Core.h"

namespace nebula {

    class NEBULA_API Timer
    {
    public:
        Timer()
        {
            reset();
        }

        inline void reset() { m_start = std::chrono::high_resolution_clock::now(); }

        inline double elapsedSeconds(bool reset_timer = false)      { return elapsed(0.000'000'001, reset_timer); }
        inline double elapsedMilliSeconds(bool reset_timer = false) { return elapsed(0.000'001, reset_timer); }
        inline double elapsedMicroSeconds(bool reset_timer = false) { return elapsed(0.001, reset_timer); }
        inline double elapsedNanoSeconds(bool reset_timer = false)  { return elapsed(1, reset_timer); }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

        double elapsed(double multiplier, bool reset_timer)
        {
            auto delta_time = static_cast<double>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_start).count()
            ) * multiplier;

            if (reset_timer)
                reset();

            return delta_time;
        }
    };

}

#endif //NEBULAENGINE_TIMER_H
