#pragma once

#include <stdexcept>

namespace yaka
{
    /* The Color enum represent a side. */
    enum class Color : bool
    {
        WHITE = false,
        BLACK = true
    };

    constexpr Color other_color(Color color) noexcept
    {
        return Color::WHITE == color ? Color::BLACK : Color::WHITE;
    }

    inline Color char_to_color(const char c)
    {
        switch (c)
        {
            case 'w':
                return Color::WHITE;
            case 'b':
                return Color ::BLACK;
            default:
                throw std::invalid_argument("Unknown color");
        }
    }


} // namespace board
