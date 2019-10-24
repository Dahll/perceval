//
// Created by fouche_r on 5/19/19.
//

#pragma once

#include <tiff.h>
#include <stdexcept>
#include <optional>

namespace chessBoard {
    /* Using, enum and constexpr */
    enum enumPiece {
        nWhite = 0,
        nBlack = 1,
        nPawn = 2,
        nKnight = 3,
        nBishop = 4,
        nRook = 5,
        nQueen = 6,
        nKing = 7,
    };
    using piece_color_t = std::pair<enumPiece, enumPiece>;
    using opt_piece_t = std::optional<piece_color_t>;

    using INDEX_T = int;
    using POSITION_T = uint64;

    constexpr enumPiece int_to_enumPiece[] = {nWhite, nBlack, nPawn, nKnight, nBishop, nRook, nQueen, nKing};
    constexpr char epiece_to_char[] =
            {'w', 'b', 'P', 'N', 'B', 'R', 'Q', 'K'};

    constexpr enumPiece char_to_enum(const char c) {
        switch (c) {
            case 'P':
                return enumPiece::nPawn;
            case 'N':
                return enumPiece::nKnight;
            case 'B':
                return enumPiece::nBishop;
            case 'R':
                return enumPiece::nRook;
            case 'Q':
                return enumPiece::nQueen;
            case 'K':
                return enumPiece::nKing;
            default:
                throw std::invalid_argument("Unknown piece");
        }
    }

    /* Board independent / general purpose */
    int number_piece(uint64 num);
    uint64 index_to_uint64(int index);
    INDEX_T uint64_to_index(uint64 pos);
    bool pawn_will_promote(INDEX_T index, enumPiece color);
    INDEX_T split_index(uint64& pBB);
    POSITION_T split_pos(uint64& pBB);
    POSITION_T pos_from_index(const INDEX_T& index);
}

#include "chessboard.hxx"
