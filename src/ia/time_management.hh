//
// Created by adrien on 02/11/19.
//

#pragma once

#include <chrono>
#include <board.hh>
#include "ia_env.hh"
#include <x86intrin.h>


using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;


namespace time_management
{
    int get_boost_factor(int nb_turns);
    int approx_next_time(const chessBoard::Board& cur_b, int depth);
    bool approx_enough_time(const TimePoint& act_start,
                            const TimePoint& start, int max_time, size_t depth);
    int val_max_depth();

    int give_time(int time_left);


}
