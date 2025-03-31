#pragma once

#include <Udon.hpp>

struct Color
{
    double r;
    double g;
    double b;

    Color operator*(const double& other) const
    {
        return {
            r * other,
            g * other,
            b * other
        };
    }

    Color operator/(const double& other) const
    {
        return {
            r / other,
            g / other,
            b / other
        };
    }
};


namespace ColorPreset
{
    constexpr Color RED     = Color{ 255, 000, 000 };
    constexpr Color YELLOW  = Color{ 255, 255, 000 };
    constexpr Color GREEN   = Color{ 000, 255, 000 };
    constexpr Color AQUA    = Color{ 000, 255, 255 };
    constexpr Color BLUE    = Color{ 000, 000, 255 };
    constexpr Color MAGENTA = Color{ 255, 000, 255 };
    constexpr Color WHITE   = Color{ 255, 255, 255 };
    constexpr Color BLACK   = Color{ 000, 000, 000 };
}    // namespace ColorPreset


inline constexpr Color hsvToRgb(double h, double s, double v)
{
    const double c      = v * s;
    const double h_dash = h / 60.0;
    const double x      = c * (1 - std::fabs(fmod(h_dash, 2) - 1));
    double       r = 0, g = 0, b = 0;

    if (0 <= h_dash && h_dash < 1)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (1 <= h_dash && h_dash < 2)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (2 <= h_dash && h_dash < 3)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (3 <= h_dash && h_dash < 4)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (4 <= h_dash && h_dash < 5)
    {
        r = x;
        g = 0;
        b = c;
    }
    else if (5 <= h_dash && h_dash < 6)
    {
        r = c;
        g = 0;
        b = x;
    }

    const double m = v - c;
    r              = (r + m) * 255;
    g              = (g + m) * 255;
    b              = (b + m) * 255;

    return Color{ r, g, b };
}


inline std::vector<Color> makeRainbow(int length)
{
    std::vector<Color> colors(length);

    for (int i = 0; i < length; ++i)
    {
        colors.at(i) = (hsvToRgb(Udon::Map(i, 0, length, 0, 360), 1, 1));
    }

    return colors;
}
