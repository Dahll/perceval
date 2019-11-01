//
// Created by adrien on 01/11/19.
//



#pragma once

#include "board.hh"
#include <map>



namespace evaluation
{

    int test_mini_max();
    int eval_queen(const chessBoard::enumPiece& color,
                   const chessBoard::enumPiece& not_color_);
    int eval_rook(const chessBoard::enumPiece& color,
                  const chessBoard::enumPiece& not_color_);
    int eval_bishop(const chessBoard::enumPiece& color,
                    const chessBoard::enumPiece& not_color_);
    int eval_knight(const chessBoard::enumPiece& color,
                    const chessBoard::enumPiece& not_color_);
    int eval_pawn(const chessBoard::enumPiece& color,
                  const chessBoard::enumPiece& not_color_);
    int eval_king(const chessBoard::enumPiece& color,
                  const chessBoard::enumPiece& not_color_);
    int evaluate(const chessBoard::enumPiece& color_);


    static const std::map<std::string, int> map_test_white {
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

    static const std::map<std::string, int> map_test_black {
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