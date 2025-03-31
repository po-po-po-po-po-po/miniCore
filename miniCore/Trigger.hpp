#pragma once

#include "SpeedServo.hpp"
#include "Timer.hpp"

struct Trigger
{
    Trigger(uint8_t&& ServoPin_, Udon::ServoChoose&& Choose_, uint&& LoopCycle_)
        : servo(ServoPin_, Udon::ServoSpeed{ LoopCycle_, Choose_, (uint)beginAngle })
        , timer()
        , isReloaded(true)
        , isFiring(false)
        , lastStatus(true)
    {
    }

    void begin() { servo.attach(); }
    void semiAuto(const bool& click);
    void fullAuto(const bool& press);
    void stop();

    bool getStatus() const { return isFiring; }

private:
    SpeedServo servo;
    Timer      timer;
    bool       isReloaded;
    bool       isFiring;
    bool       lastStatus;

    void move(const bool& isFire);

    static constexpr double beginAngle = 0;
    static constexpr double endAngle   = 50;
};


inline void Trigger::semiAuto(const bool& click)
{
    isFiring = click || isFiring;
    if (isFiring)
    {
        isFiring = isReloaded;
    }
    this->move(isFiring);
}


inline void Trigger::fullAuto(const bool& press)
{
    isFiring = press && isReloaded;
    this->move(isFiring);
}


inline void Trigger::move(const bool& isFire)
{
    servo.move((isFire ? beginAngle : endAngle), 20);

    if (isFire)
    {
        if (lastStatus != servo.getStatus())
        {
            timer.reset();
        }
        if ((timer.getTime() > 500) && servo.getStatus())
        {
            isReloaded = true;
        }
    }
    else
    {
        isReloaded = !(servo.getStatus());
    }

    lastStatus = servo.getStatus();
}


inline void Trigger::stop()
{
    servo.stop();
}