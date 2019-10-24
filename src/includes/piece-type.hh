#pragma once

#include <array>
#include <optional>

namespace yaka
{
    /* PieceType is an enum representing every possible
     * piece type present on the board. The char associated
     * with each value is the ascii char representing the
     * piece on the board */
    enum class PieceType
    {
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT,
        PAWN,
        KING, // Last position allows better optimization (order-move)
    };

    using opt_piecetype_t = std::optional<PieceType>;

    /* Can be useful to iterate over PieceTypes */
    constexpr std::array<PieceType, 6> piecetype_array{
        PieceType::QUEEN,  PieceType::ROOK, PieceType::BISHOP,
        PieceType::KNIGHT, PieceType::PAWN, PieceType::KING};

    constexpr char piece_to_char[] = {'K', 'Q', 'R', 'B', 'N', 'P'};

    constexpr char piece_to_low_char[] = {'k', 'q', 'r', 'b', 'n', 'p'};

    inline PieceType char_to_piece(const char c)
    {
        switch (c)
        {
        case 'P':
            return PieceType::PAWN;
        case 'N':
            return PieceType::KNIGHT;
        case 'B':
            return PieceType::BISHOP;
        case 'R':
            return PieceType::ROOK;
        case 'Q':
            return PieceType::QUEEN;
        case 'K':
            return PieceType::KING;
        default:
            throw std::invalid_argument("Unknown piece");
        }
    }


    inline int piece_to_enumpiece_index(const PieceType& c)
    {
        switch (c)
        {
            case PieceType::PAWN:
                return 2;
            case PieceType::KING:
                return 7;
            case PieceType::KNIGHT:
                return 3;
            case PieceType::BISHOP:
                return 4;
            case PieceType::QUEEN:
                return 6;
            case PieceType::ROOK:
                return 5;
            default:
                throw std::invalid_argument("Unknown piece");
        }
    }
} // namespace board
