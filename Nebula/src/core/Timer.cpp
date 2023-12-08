//
// Created by michal-swiatek on 14.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/Timer.h"

#include "core/Core.h"
#include "core/UpdateContext.h"

namespace nebula {

    void Timer::sleep(double seconds)
    {
        system_sleep(seconds);
    }

    void Timer::sleepUntil(double application_time)
    {
        double sleep_time = std::max(application_time - UpdateContext::get().getTime(), 0.0);
        system_sleep(sleep_time);
    }

    void Timer::sleepPrecise(double seconds, double busy_offset)
    {
        Timer busy_timer;
        double sleep_time = std::max(seconds - busy_offset, 0.0);

        system_sleep(sleep_time);                       //  Thread sleeping
        while (busy_timer.elapsedSeconds() < seconds);  //  Spin-lock remaining duration
    }

    void Timer::sleepUntilPrecise(double application_time, double busy_offset)
    {
        static constexpr double epsilon = 0.0;
        Timer busy_timer(UpdateContext::get().getTime());

        sleepUntil(application_time - busy_offset);                       //  Thread sleeping
        while (application_time - busy_timer.elapsedSeconds() > epsilon); //  Spin-lock remaining duration
    }

}
