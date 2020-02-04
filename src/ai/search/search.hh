//
// Created by fouche_r on 5/22/19.
//

#pragma once


#include <chrono>
#include <vector>
#include <algorithm>

#include "../transposition_table.hh"
#include "../evaluation/evaluation.hh"
#include "../../perft/perft.hh"
#include "stock.hh"
#include "../helpers/helper.hh"
#include "../uci.hh"
#include "../../board/board.hh"
#include "../refutation_table.hh"
#include "../metadata.hh"

using namespace std::chrono;

namespace ai::search
{

    int alphabeta(const chessBoard::Board& b, const chessBoard::enumPiece& colo_act, int depth, int alpha,
                int beta, const chessBoard::Move& prev_move,
                chessBoard::MOVES_T& prev_vect_move,
                uint64 hash);

    chessBoard::Move caller_alphabeta(const chessBoard::Board& b, int depth, std::vector<chessBoard::Move>& output_vect, uint64 hash, bool& winning_move);

    int quiesce(const chessBoard::Board& b, const chessBoard::enumPiece& color_act, int alpha, int beta,
                const chessBoard::Move& prev_move, uint64 hash);
    void iterative_deepening();

}
