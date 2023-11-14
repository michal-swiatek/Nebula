//
// Created by michal-swiatek on 05.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_TIMESTEP_H
#define NEBULAENGINE_TIMESTEP_H

#include <chrono>

#include "core/Core.h"

namespace nebula {

    class NEBULA_API Timestep
    {
    public:
        explicit Timestep(double seconds = 0.0) : m_time(seconds) {}

        [[nodiscard]] float getSecondsFloat()      const { return static_cast<float>(m_time); }
        [[nodiscard]] float getMilliSecondsFloat() const { return static_cast<float>(m_time * 1'000); }
        [[nodiscard]] float getMicroSecondsFloat() const { return static_cast<float>(m_time * 1'000'000); }
        [[nodiscard]] float getNanoSecondsFloat()  const { return static_cast<float>(m_time * 1'000'000'000); }

        [[nodiscard]] double getSeconds()      const { return m_time; }
        [[nodiscard]] double getMilliSeconds() const { return m_time * 1'000; }
        [[nodiscard]] double getMicroSeconds() const { return m_time * 1'000'000; }
        [[nodiscard]] double getNanoSeconds()  const { return m_time * 1'000'000'000; }

        [[nodiscard]] std::chrono::seconds      getSecondsDuration()      const { return std::chrono::seconds(static_cast<int>(m_time)); }
        [[nodiscard]] std::chrono::milliseconds getMilliSecondsDuration() const { return std::chrono::milliseconds(static_cast<int>(m_time * 1'000)); }
        [[nodiscard]] std::chrono::microseconds getMicroSecondsDuration() const { return std::chrono::microseconds(static_cast<int>(m_time * 1'000'000)); }
        [[nodiscard]] std::chrono::nanoseconds  getNanoSecondsDuration()  const { return std::chrono::nanoseconds(static_cast<int>(m_time * 1'000'000'000)); }

        explicit operator float() const { return static_cast<float>(m_time); }
        operator double() const { return m_time; }

    private:
        double m_time;
    };

}

#endif //NEBULAENGINE_TIMESTEP_H
