//
// Created by adrien on 01/11/19.
//



#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <strings.h>
#include <optional>
#include "stock.hh"
#include "../transposition_table.hh"
#include "mid_game.hh"
#include "late_game.hh"


namespace ai::evaluation
{
    extern uint32 values[2][8][64];

    void set_positional_values(const chessBoard::Board& b, const chessBoard::enumPiece& color,
                               const chessBoard::enumPiece& not_color, MG& mg, EG& eg);
    void set_eval_values(const chessBoard::Board& b, const chessBoard::enumPiece& color,
                         const chessBoard::enumPiece& not_color, MG& mg, EG& eg);

    void init_values();
    int calc_phase(const chessBoard::Board& b);
    int evaluate(const chessBoard::Board& b, const chessBoard::enumPiece &color_);
}