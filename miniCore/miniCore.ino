#include <Udon.hpp>

#include "LedController.hpp"
#include "Omni3.hpp"
#include "Trigger.hpp"

Udon::LoopCycleController loopCtrl{ 1000 };

Udon::PicoWDT wdt;

Udon::Led picoLed(LED_BUILTIN);

Udon::PadPS5<Udon::E220Reader> pad{
    { .serial = Serial1,
      .m0     = 4,
      .m1     = 3,
      .aux    = 2 }
};

LedController<15> led{ 0, loopCtrl.cycleUs() };

Omni3 omni{
    std::array<Udon::Motor2, 3>{
        Udon::Motor2{ 0, 0 },
        Udon::Motor2{ 0, 0 },
        Udon::Motor2{ 0, 0 } }
};

Trigger trigger{ 0, Udon::ServoChoose::_20kg, loopCtrl.cycleUs() };

Udon::Motor3 motor{ 0, 0, 0 };


void setup()
{
    delay(100);

    Serial.begin(115200);
    picoLed.begin();
    pad.begin();

    delay(100);

    led.begin(15);
    omni.begin();
    trigger.begin();
    motor.begin();

    delay(100);
}


void loop()
{
    wdt.update();
    pad.update();
    picoLed.flush();

    if (pad.isOperable())
    {
        auto moveInfo = pad.getMoveInfo();
        moveInfo /= pad.getL2().press ? 0.5 : 1;
        omni.move(moveInfo);

        if (pad.getR2().press)
        {
            trigger.fullAuto(true);
        }
        else
        {
            trigger.semiAuto(pad.getR1().click);
        }

        motor.move(trigger.getStatus() ? 200 : 0);

        led.setRainbow(1.5);
    }
    else
    {
        omni.stop();
        trigger.stop();
        motor.stop();
        led.setColor(ColorPreset::RED);
    }

    led.flush();
    loopCtrl.update();
}
