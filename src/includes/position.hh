#pragma once

#include <utility>

#include "utype.hh"

namespace yaka
{
    /* The file enum represent the columns
     * on the board */
    enum class File
    {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
    };

    /* The rank enum represent the lines
     * on the board */
    enum class Rank
    {
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
    };

#ifndef STUDENT
    constexpr char file_to_char[] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    };

    constexpr char rank_to_char[] = {
        '1', '2', '3', '4', '5', '6', '7', '8',
    };
#endif

    /* Position represent a coordinate on the board */
    class Position final
    {
    public:
        Position(File file, Rank rank);

        File file_get() const;
        Rank rank_get() const;

        bool operator==(const Position& pos) const;
        bool operator!=(const Position& pos) const;

    private:
        File file_;
        Rank rank_;
    };

} // namespace board

#include "position.hxx"
