#pragma once

#include <Udon.hpp>
#include <array>
#include <utility>
#include <cmath>

//       3      1
//       ↗＝＝＝↘
//          ↑
//          ← 2
//

struct Omni3
{
    Omni3(std::array<Udon::Motor2, 3>&& Motors_)
        : motors(std::move(Motors_))
    {
    }

    void begin();
    void move(const Udon::Stick& moveInfo);
    void stop();

private:
    std::array<Udon::Motor2, 3> motors;
};


inline void Omni3::begin()
{
    for (auto& motor : motors)
    {
        motor.begin();
    }
}


inline void Omni3::move(const Udon::Stick& moveInfo)
{
    const auto& vec  = moveInfo.vector;
    const auto& turn = moveInfo.turn;

    std::array<double, 3> powers;
    powers[0] = (vec.x / 2.0) - (vec.y / 2.0 * std::sqrt(3)) + turn;
    powers[1] = -vec.x + turn;
    powers[2] = (vec.x / 2.0) + (vec.y / 2.0 * std::sqrt(3)) + turn;

    constexpr double limit = 220;

    const auto it = std::find_if(powers.begin(), powers.end(), [](const double& power)
                                 { return std::fabs(power) < limit; });

    if (it != powers.end())
    {
        const double late = limit / std::fabs(*it);

        for (auto& power : powers)
        {
            power *= late;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        motors[i].move(powers[i]);
    }
}


inline void Omni3::stop()
{
    for (auto& motor : motors)
    {
        motor.stop();
    }
}
