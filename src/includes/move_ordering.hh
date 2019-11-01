//
// Created by adrien on 02/11/19.
//

#pragma once

#include "board.hh"

using INT_MOVE_PAIR = std::pair<int, chessBoard::Move>;
using VECTOR_PAIR = std::vector<INT_MOVE_PAIR >;


namespace ai::ordering
{
    int quiescence_calc_move(const chessBoard::Move& move,
                             const std::optional<chessBoard::Move>& prev_move, size_t depth, uint64 hash);
    int calc_move(const chessBoard::Move& move, size_t act_depth,
                  const std::optional<chessBoard::Move>& prev_move, uint64 hash);
    VECTOR_PAIR moves_set_values(const std::vector<chessBoard::Move>& vect,
                                 const std::optional<chessBoard::Move>& prev_move, int depth, uint64 hash);
    VECTOR_PAIR moves_set_values_quiescence(const std::vector<chessBoard::Move>& vect,
                                            const std::optional<chessBoard::Move>& prev_move, int depth, uint64 hash);
}