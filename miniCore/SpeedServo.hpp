#pragma once

#include <Udon.hpp>

struct SpeedServo
{
    SpeedServo(uint8_t ServoPin_, Udon::ServoSpeed Speed_)
        : pin(std::move(ServoPin_))
        , speed(std::move(Speed_))
    {
    }

    void attach() { servo.attach(pin); }
    void move(const uint& angle, const double& speed);
    void stop();
    bool getStatus() const { return speed.getStatus(); }

private:
    const uint8_t    pin;
    Udon::ServoSpeed speed;
    Udon::Servo      servo;
};


inline void SpeedServo::move(const uint& angle, const double& speed)
{
    this->speed.setAngle(angle, speed);
    this->speed.update();

    servo.write(this->speed.getAngle());
}


inline void SpeedServo::stop()
{
    speed.stop();
    servo.write(speed.getAngle());
}
