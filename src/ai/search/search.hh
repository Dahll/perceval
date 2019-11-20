//
// Created by fouche_r on 5/22/19.
//

#pragma once


#include <chrono>
#include <vector>
#include <algorithm>

#include "../time_management.hh"
#include "../transposition_table.hh"
#include "../evaluation/evaluation.hh"
#include "perft.hh"
#include "stock.hh"
#include "../helpers/helper.hh"
#include "../uci.hh"
#include "board.hh"
#include "../refutation_table.hh"
#include "../metadata.hh"

using namespace std::chrono;

namespace ai::search
{

    int alphabeta(const chessBoard::enumPiece& colo_act, int depth, int alpha,
                int beta, const chessBoard::Move& prev_move,
                chessBoard::MOVES_T& prev_vect_move,
                uint64 hash);

    chessBoard::Move caller_alphabeta(int depth, std::vector<chessBoard::Move>& output_vect, uint64 hash, bool& winning_move);

    int quiesce(const chessBoard::enumPiece& color_act, int alpha, int beta,
                const chessBoard::Move& prev_move, uint64 hash);
    std::optional<chessBoard::Move> iterative_deepening(int max_size);

}
