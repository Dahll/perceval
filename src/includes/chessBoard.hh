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

    using INDEX_T = uint16 ;
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

    constexpr uint64_t FILE_A = 0x0101010101010101;
    constexpr uint64_t FILE_B = 0x0202020202020202;
    constexpr uint64_t FILE_C = 0x0404040404040404;
    constexpr uint64_t FILE_D = 0x0808080808080808;
    constexpr uint64_t FILE_E = 0x1010101010101010;
    constexpr uint64_t FILE_F = 0x2020202020202020;
    constexpr uint64_t FILE_G = 0x4040404040404040;
    constexpr uint64_t FILE_H = 0x8080808080808080;
    constexpr uint64_t FILES[8] = {
            0x0101010101010101, 0x0202020202020202, 0x0404040404040404,
            0x0808080808080808, 0x1010101010101010, 0x2020202020202020,
            0x4040404040404040, 0x8080808080808080
    };
    constexpr uint64_t QSIDE = 0x0F0F0F0F0F0F0F0F;
    constexpr uint64_t KSIDE = 0xF0F0F0F0F0F0F0F0;
    constexpr uint64_t CENTER_FILES = 0x3C3C3C3C3C3C3C3C;

    constexpr uint64_t RANK_1 = 0x00000000000000FF;
    constexpr uint64_t RANK_2 = 0x000000000000FF00;
    constexpr uint64_t RANK_3 = 0x0000000000FF0000;
    constexpr uint64_t RANK_4 = 0x00000000FF000000;
    constexpr uint64_t RANK_5 = 0x000000FF00000000;
    constexpr uint64_t RANK_6 = 0x0000FF0000000000;
    constexpr uint64_t RANK_7 = 0x00FF000000000000;
    constexpr uint64_t RANK_8 = 0xFF00000000000000;
    constexpr uint64_t RANKS[8] = {
            0x00000000000000FF, 0x000000000000FF00, 0x0000000000FF0000,
            0x00000000FF000000, 0x000000FF00000000, 0x0000FF0000000000,
            0x00FF000000000000, 0xFF00000000000000
    };

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
