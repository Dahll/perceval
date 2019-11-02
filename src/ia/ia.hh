//
// Created by fouche_r on 5/22/19.
//

#pragma once

#include "time_management.hh"
#include "move_ordering.hh"
#include "transposition_table.hh"
#include "evaluation/evaluation.hh"
#include "perft.hh"
#include "stock.hh"
#include "helpers/helper.hh"
#include "uci.hh"
#include "board.hh"
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std::chrono;

namespace ai
{

    #define QUEEN 900
    #define ROOKS 500
    #define BISHOP 330
    #define KNIGHT 320
    #define PAWN 100
    #define KING 20000

    void play_chess();

    int alphabeta(const chessBoard::enumPiece& colo_act, int depth, int alpha,
                int beta, const chessBoard::Move& prev_move,
                chessBoard::MOVES_T& prev_vect_move,
                chessBoard::MOVES_T& prev_vect_move_quiescence, uint64 hash);

    int caller_alphabeta(int depth, std::vector<chessBoard::Move>& output_vect, std::vector<chessBoard::Move>& output_vect_quiescence, uint64 hash);

    int quiesce(const chessBoard::enumPiece& color_act, int alpha, int beta,
                const chessBoard::Move& prev_move, int depth,
                chessBoard::MOVES_T& prev_vect_move_quiescence, uint64 hash);
    std::optional<chessBoard::Move> iterative_deepening(int max_size, uint64 hash);

}
