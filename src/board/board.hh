//
// Created by fouche_r on 5/16/19.
//

#pragma once

#include "color.hh"
#include "move.hh"
#include "position.hh"
#include "chessBoard.hh"
#include "parser.hh"
//#include "../magic/generate.hh"
#include "../magic/magic.hh"
#include <iostream>
#include <vector>
#include <optional>
/*
 * -----------------------------------------
 * | 63 | 62 | 61 | 60 | 59 | 58 | 57 | 56 |
 * |---------------------------------------|
 * | 55 | 54 | 53 | 52 | 51 | 50 | 49 | 48 |
 * |---------------------------------------|
 * | 47 | 46 | 45 | 44 | 43 | 42 | 41 | 40 |
 * |---------------------------------------|
 * | 39 | 38 | 37 | 36 | 35 | 34 | 33 | 32 |
 * |---------------------------------------|
 * | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 |
 * |---------------------------------------|
 * | 23 | 22 | 21 | 20 | 19 | 18 | 17 | 16 |
 * |---------------------------------------|
 * | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |
 * |---------------------------------------|
 * |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
 * -----------------------------------------
 */

namespace chessBoard
{

    //using MOVES_T = std::vector<Move>;
    class Board
    {
    private:
        /* Private attributes here */

    public:
        /* Public attributes */
        bool mat = false;
        bool check = false;
        bool pat = false;
        uint64 special_moves = 0;
        uint64 pieceBB[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        enumPiece color = nWhite;
        bool game_finished = false;
        uint64 castlings = 0;
        int half_move_count_ = 0;
        int turn_count_ = 0;

        /* Functions */
        explicit Board(bool empty = false);

        Board copy_board() const;

        uint64 apply_castling(const Move& m, const enumPiece& color, uint64 hash_board);
        uint64 apply_move(const Move &m, const enumPiece& color, uint64 hash_board);
        void apply_null_move(uint64& hash_board);
        void revert_null_move(uint64& hash_board, uint64 tmp_special_move);




        void add_piece(const piece_color_t& piece, const POSITION_T& index);
        //opt_piece_t operator[](const INDEX_T& index) const;
        int moves_count();

        enumPiece other_color(const enumPiece& color) const;
        void switch_color();

        /* Checks */
        bool check_promotion(const Move& move, const Move& m) const;
        //std::optional<Move> check_apply_move(Move& move);
        bool game_is_draw(const Move& move);

        void set_check_mat_pat(enumPiece color_, const VMove& valid_moves);
        bool player_is_check(const enumPiece& color_) const;
        bool square_is_check(const enumPiece& color_, const POSITION_T& pos) const;

        bool player_is_mat(enumPiece color_, const VMove& moves) const;
        bool player_is_pat(enumPiece color_, const VMove& moves) const;


        POSITION_T captured_piece_position(const Move& move);


        void move_attack_square(VMove& moves, const enumPiece& color_,
                uint64 pos) const;

        chessBoard::Move string_to_move(std::string& s, const enumPiece& color_);
        //Move ia_apply_move(const Move& m, const enumPiece& color_, uint64& hash_board);
        void print() const;
        //void add_move(const enumPiece& color_, std::vector<Move>& vec, const uint64& from, const uint64& to, const enumPiece& type, const std::optional<enumPiece>& capture, const std::optional<enumPiece>& promotion, const bool& is_castling, const bool& is_en_passant, const uint64& report) const;
        void add_move(const enumPiece& color_, VMove& moves, const Move& m) const;

        bool is_board_valide(Move& mov, enumPiece& color_);
        //void test_board(std::vector<Move>& vec);
        std::string to_perft() const;

        //Board apply_many_moves(const MOVES_T& moves);
        //int get_current_pieces_count() const;
        bool is_only_pawn(chessBoard::enumPiece color) const;



        /* Move generation (const) */
        void generate_moves(const enumPiece& color_, VMove& vec) const;

        void generate_captures(const enumPiece& color_,
                uint64& captures_pos, const INDEX_T& index,
                VMove& vec) const;
        void generate_non_captures(const enumPiece& color_, uint64& captures_pos,
                const INDEX_T& index, VMove& vec) const;
        void get_knight_move(const enumPiece& color_, VMove& vec) const;
        void get_tower_move(const enumPiece& color_, VMove& vec) const;
        void get_bishop_move(const enumPiece& color_, VMove& vec) const;
        void get_king_move(const enumPiece& color_, VMove& vec) const;
        void get_queen_move(const enumPiece& color_, VMove& vec) const;
        void get_pawn_move(const enumPiece& color_, VMove& vec) const;
        void generate_castlings(const enumPiece& color_, VMove& vec) const;
        void generate_pawn_move(const enumPiece &color_, VMove& vec,
                uint64& mask, const uint64 (&attack)[64],
                const uint64 (&normal)[64], const uint64 (&jump)[64]) const;
        void generate_attack_move_pawn(const enumPiece &color_,
                VMove& vec, uint16 index,
                const uint64 (&attack)[64]) const;
        void generate_classic_move_pawn(const enumPiece& color_,
                VMove& vec, const uint16 & index,
                const uint64 (&normal)[64], const uint64 (&jump)[64]) const;
        void get_all_move_square_check(VMove& moves,
                const enumPiece& color_, Board& board) const;


        /* Captures only */
        void generate_capture_moves(const enumPiece& color_ ,VMove& vec) const;
        void get_knight_capture_moves(const enumPiece& color_, VMove& vec) const;
        void get_tower_capture_moves(const enumPiece& color_, VMove& vec) const;
        void get_bishop_capture_moves(const enumPiece& color_,VMove& vec) const;
        void get_king_capture_moves(const enumPiece& color_, VMove& vec) const;
        void get_queen_capture_moves(const enumPiece& color_, VMove& vec) const;
        void get_pawn_capture_moves(const enumPiece& color_, VMove& vec) const;

        /* Static */
    };
}
