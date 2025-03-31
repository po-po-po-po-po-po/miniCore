#pragma once

#include <Udon.hpp>
#include <Adafruit_NeoPixel.h>
#include "Color.hpp"

template <size_t length>
struct LedController
{
    LedController(uint8_t&& Pin_, unsigned long long&& InvokeCycle_)
        : pixel(length, Pin_)
        , integralVar(0.0)
        , interval(InvokeCycle_)
    {
        auto ar = makeRainbow(length);
        std::copy(ar.begin(), ar.end(), rainbow.begin());
    }

    void begin(const uint8_t& brightness);

    void setColor(const Color& color);

    void setBrightness(const uint8_t& brightness);

    void setRainbow(const double& speed);

    void setShootingStar(const double& speed, const int& range, const int& time, const Color& color);

    void flush();

    auto getColor() const { return flushColor; }
    auto getRainbow() const { return rainbow; }
    auto getIntegralVar() const { return integralVar; }

private:
    Adafruit_NeoPixel          pixel;
    std::array<Color, length>  rainbow;
    std::array<Color, length>  flushColor;
    std::array<double, length> phantom;
    long double                integralVar;
    const double               interval;
};


template <size_t length>
inline void LedController<length>::begin(const uint8_t& brightness)
{
    pixel.begin();
    pixel.setBrightness(brightness);
}

template <size_t length>
inline void LedController<length>::setColor(const Color& color)
{
    for (auto& x : flushColor)
    {
        x = color;
    }
}

template <size_t length>
inline void LedController<length>::setBrightness(const uint8_t& brightness)
{
    pixel.setBrightness(brightness);
}

template <size_t length>
inline void LedController<length>::setRainbow(const double& speed)
{
    constexpr int len = length;

    integralVar += speed * interval * length / 1'000'000;
    int phase = static_cast<int>(integralVar) % len;

    if (phase < 0)
    {
        phase += len;
    }

    for (int i = 0; i < len; ++i)
    {
        int index = (i + phase + len) % len;

        flushColor.at(i) = rainbow.at(index);
    }
}

template <size_t length>
inline void LedController<length>::setShootingStar(const double& speed, const int& range, const int& time, const Color& color)
{
    constexpr int len = length;
    if (range >= len)
    {
        Serial.println("Wrong argument for setShootingStar");
        return;
    }

    integralVar += speed * interval * length / 1'000'000;
    int phase = static_cast<int>(integralVar) % length;
    if (phase < 0)
    {
        phase += length;
    }

    const double decrement = interval / 1000.0 / time;
    for (int i = 0; i < len; ++i)
    {
        int index        = (i + phase) % len;
        phantom.at(i)    = std::min(phantom.at(i) + (index < range ? 1.0 : 0.0), 1.0);
        flushColor.at(i) = color * phantom.at(i);
        phantom.at(i)    = std::clamp(phantom.at(i) - decrement, 0.0, 1.0);
    }
}

template <size_t length>
inline void LedController<length>::flush()
{
    for (unsigned int i = 0; i < length; ++i)
    {
        auto [r, g, b] = flushColor.at(i);
        pixel.setPixelColor(i, r, g, b);
    }
    pixel.show();
}
