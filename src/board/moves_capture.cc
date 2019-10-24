//
// Created by romain on 6/4/19.
//

#include "stock.hh"
#include "board.hh"
#include "generate.hh"
#include <strings.h>

namespace chessBoard {

    void Board::get_knight_capture_moves(const enumPiece &color_,
                                         MOVES_T &vec) const
    {
        uint64 mask = pieceBB[nKnight] & pieceBB[color_];
        int i = 0;
        while ((i = split_index(mask)) != 0)
        {
            const uint64& move = knight_move[i];
            const uint64 temp = ((move ^ pieceBB[color_]) & move);
            uint64 temp_cap = temp & pieceBB[other_color(color_)];

            //move capture
            generate_captures(nKnight, color_, temp_cap, i, vec);
        }
    }

    void Board::get_tower_capture_moves(const enumPiece &color_,
                                        MOVES_T &vec) const
    {
        uint64 mask = pieceBB[nRook] & pieceBB[color_];
        int i = 0;
        while ((i = split_index(mask)) != 0) {
            const uint64& relevant_mask = tower_move[i] & (pieceBB[0] | pieceBB[1]);
            const uint64& index = (magic_number_tower[i] * relevant_mask) >> 52ull;
            uint64 temp = tower_output[i][index];
            temp = (temp & pieceBB[color_]) ^ temp;
            uint64 temp_cap = temp & pieceBB[other_color(color_)];

            //move capture
            generate_captures_casts(nRook, color_, temp_cap, i, vec);
        }
    }

    void Board::get_bishop_capture_moves(const enumPiece &color_,
                                         MOVES_T &vec) const
    {
        uint64 mask = pieceBB[nBishop] & pieceBB[color_];
        int i = 0;
        while ((i = split_index(mask)) != 0)
        {
            const uint64& relevant_mask = bishop_move[i] & (pieceBB[0] | pieceBB[1]);
            const uint64& index = (magic_number_bishop[i] * relevant_mask) >> 52ull;
            uint64 temp = bishop_output[i][index];
            temp = (temp & pieceBB[color_]) ^ temp;
            uint64 temp_cap = temp & pieceBB[other_color(color_)];

            //move capture
            generate_captures(nBishop, color_, temp_cap, i, vec);
        }

    }

    void Board::get_king_capture_moves(const enumPiece &color_,
                                       MOVES_T &vec) const
    {
        uint64 king = pieceBB[nKing] & pieceBB[color_];
        int i = split_index(king);
        const uint64& move = king_move[i];
        const uint64 temp = ((move ^ pieceBB[color_]) & move);
        uint64 temp_cap = temp & pieceBB[other_color(color_)];

        //move capture
        generate_captures_casts(nKing, color_, temp_cap, i, vec);
    }

    void Board::get_queen_capture_moves(const enumPiece &color_,
                                        MOVES_T &vec) const
    {
        uint64 queen = pieceBB[6] & pieceBB[color_];
        int i = 0;
        while ((i = split_index(queen)) != 0)
        {
            const uint64& relevant_diagonals = bishop_move[i] & (pieceBB[0] | pieceBB[1]);
            const uint64& index_diagonals = (magic_number_bishop[i] * relevant_diagonals) >> 52ull;
            const uint64& relevant_lignes = tower_move[i] & (pieceBB[0] | pieceBB[1]);
            const uint64& index_lignes = (magic_number_tower[i] * relevant_lignes) >> 52ull;

            uint64 temp = tower_output[i][index_lignes] | bishop_output[i][index_diagonals];
            temp = (temp & pieceBB[color_]) ^ temp;
            uint64 temp_cap = temp & pieceBB[other_color(color_)];

            //move capture
            generate_captures(nQueen, color_, temp_cap, i, vec);
        }
    }

    void Board::get_pawn_capture_moves(const chessBoard::enumPiece& color_,
            MOVES_T &vec) const
    {
        uint64 mask = pieceBB[nPawn] & pieceBB[color_];
        if (color_ == nWhite)
        {
            int i = 0;
            while ((i = split_index(mask)) != 0)
                generate_attack_move_pawn(color_, vec, i, white_pawn_attack);
        }
        else
        {
            int i = 0;
            while ((i = split_index(mask)) != 0)
                generate_attack_move_pawn(color_, vec, i, black_pawn_attack);
        }
    }

    const std::vector <Move> Board::generate_capture_moves(const enumPiece &color_) const
    {
        auto vec = MOVES_T();
        get_pawn_capture_moves(color_, vec);
        get_knight_capture_moves(color_, vec);
        get_bishop_capture_moves(color_, vec);
        get_tower_capture_moves(color_, vec);
        get_queen_capture_moves(color_, vec);
        get_king_capture_moves(color_, vec);
        return vec;
    }

}