//
// Created by fouche_r on 5/22/19.
//

#pragma once

#include "move_ordering.hh"
#include "transposition_table.hh"
#include "evaluation.hh"
#include "perft.hh"
#include "stock.hh"
#include <helper.hh>
#include <uci.hh>
#include <board.hh>
#include <map>
#include <chrono>
#include <vector>
#include <algorithm>
#include <unordered_map>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
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

    /* Time management */
    int get_boost_factor(int nb_turns);
    int approx_next_time(const chessBoard::Board& cur_b, int depth);
    bool approx_enough_time(const TimePoint& act_start,
            const TimePoint& start, int max_time, size_t depth);
    int val_max_depth();

    int give_time(int time_left);

    void merge_vect(std::vector<chessBoard::Move>& vect1, std::vector<chessBoard::Move>& vect2);

}
