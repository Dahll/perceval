//
// Created by adrien on 15/05/20.
//

#pragma once


#include <algorithm>
#include <map>
#include <strings.h>
#include "stock.hh"
#include "../transposition_table.hh"


using INT_MOVE_PAIR = std::pair<int, chessBoard::Move>;
using VECTOR_PAIR = std::vector<INT_MOVE_PAIR>;
#define VAL_QUEEN 900
#define VAL_ROOKS 500
#define VAL_BISHOP 330
#define VAL_KNIGHT 320
#define VAL_PAWN 100

namespace ai::ordering
{

    int quiescence_calc_move(const chessBoard::Board& b, const chessBoard::Move &move,
                             const std::optional<chessBoard::Move> &prev_move);

    int calc_move(const chessBoard::Board& b, const chessBoard::Move &move, int ply,
                  const std::optional<chessBoard::Move> &prev_move, uint64 hash);

    VECTOR_PAIR moves_set_values(const chessBoard::Board& b,const std::vector<chessBoard::Move> &vect,
                                 const std::optional<chessBoard::Move> &prev_move, int ply, uint64 hash);

    VECTOR_PAIR moves_set_values_quiescence(const chessBoard::Board& b,const std::vector<chessBoard::Move> &vect,
                                            const std::optional<chessBoard::Move> &prev_move);
}