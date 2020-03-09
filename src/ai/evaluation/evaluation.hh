//
// Created by adrien on 01/11/19.
//



#pragma once

#include <algorithm>
#include <map>
#include <strings.h>
#include "stock.hh"
#include "../transposition_table.hh"
#include "../refutation_table.hh"


namespace ai
{
#define VAL_QUEEN 900
#define VAL_ROOKS 500
#define VAL_BISHOP 330
#define VAL_KNIGHT 320
#define VAL_PAWN 100
#define VAL_KING 20000
    namespace evaluation
    {

        int test_mini_max(const chessBoard::Board& b);

        int eval_queen(const chessBoard::Board& b, const chessBoard::enumPiece &color,
                       const chessBoard::enumPiece &not_color_);

        int eval_rook(const chessBoard::Board& b, const chessBoard::enumPiece &color,
                      const chessBoard::enumPiece &not_color_);

        int eval_bishop(const chessBoard::Board& b, const chessBoard::enumPiece &color,
                        const chessBoard::enumPiece &not_color_);

        int eval_knight(const chessBoard::Board& b, const chessBoard::enumPiece &color,
                        const chessBoard::enumPiece &not_color_);

        int eval_pawn(const chessBoard::Board& b, const chessBoard::enumPiece &color,
                      const chessBoard::enumPiece &not_color_);

        int eval_king(const chessBoard::Board& b, const chessBoard::enumPiece &color,
                      const chessBoard::enumPiece &not_color_);

        int evaluate(const chessBoard::Board& b, const chessBoard::enumPiece &color_);


        static const std::map<std::string, int> map_test_white{
                {"6k1/6p1/6Pp/7P/p7/Pp6/1P6/1K6 w - - 0 0\n", 0},
                {"6k1/6p1/6Pp/7P/p7/Pp6/1P6/3K4 w - - 0 0\n", 1},
                {"6k1/6p1/6Pp/7P/p7/Pp6/1P1K4/8 w - - 0 0\n", 2},

                {"4k3/6p1/6Pp/7P/p7/Pp6/1P6/1K6 w - - 0 0\n", -1},
                {"4k3/6p1/6Pp/7P/p7/Pp6/1P6/3K4 w - - 0 0\n", 0},
                {"4k3/6p1/6Pp/7P/p7/Pp6/1P1K4/8 w - - 0 0\n", 1},

                {"8/4k1p1/6Pp/7P/p7/Pp6/1P6/1K6 w - - 0 0\n", -2},
                {"8/4k1p1/6Pp/7P/p7/Pp6/1P6/3K4 w - - 0 0\n", -1},
                {"8/4k1p1/6Pp/7P/p7/Pp6/1P1K4/8 w - - 0 0\n", 0},
        };

        static const std::map<std::string, int> map_test_black{
                {"6k1/6p1/6Pp/7P/p7/Pp6/1P6/1K6 b - - 0 0\n", 0},
                {"6k1/6p1/6Pp/7P/p7/Pp6/1P6/3K4 b - - 0 0\n", -1},
                {"6k1/6p1/6Pp/7P/p7/Pp6/1P1K4/8 b - - 0 0\n", -2},

                {"4k3/6p1/6Pp/7P/p7/Pp6/1P6/1K6 b - - 0 0\n", 1},
                {"4k3/6p1/6Pp/7P/p7/Pp6/1P6/3K4 b - - 0 0\n", 0},
                {"4k3/6p1/6Pp/7P/p7/Pp6/1P1K4/8 b - - 0 0\n", -1},

                {"8/4k1p1/6Pp/7P/p7/Pp6/1P6/1K6 b - - 0 0\n", 2},
                {"8/4k1p1/6Pp/7P/p7/Pp6/1P6/3K4 b - - 0 0\n", 1},
                {"8/4k1p1/6Pp/7P/p7/Pp6/1P1K4/8 b - - 0 0\n", 0},
        };
    }

    using INT_MOVE_PAIR = std::pair<int, chessBoard::Move>;
    using VECTOR_PAIR = std::vector<INT_MOVE_PAIR>;


    namespace ordering
    {
        int quiescence_calc_move(const chessBoard::Board& b, const chessBoard::Move &move,
                                 const std::optional<chessBoard::Move> &prev_move);

        int calc_move(const chessBoard::Board& b, const chessBoard::Move &move, size_t act_depth,
                      const std::optional<chessBoard::Move> &prev_move, uint64 hash);

        VECTOR_PAIR moves_set_values(const chessBoard::Board& b,const std::vector<chessBoard::Move> &vect,
                                     const std::optional<chessBoard::Move> &prev_move, int depth, uint64 hash);

        VECTOR_PAIR moves_set_values_quiescence(const chessBoard::Board& b,const std::vector<chessBoard::Move> &vect,
                                                const std::optional<chessBoard::Move> &prev_move);
    }
}