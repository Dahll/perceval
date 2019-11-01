//
// Created by adrien on 31/10/19.
//

#pragma once

#include <tiff.h>
#include <vector>
#include <unordered_map>
#include <chrono>
#include "chessBoard.hh"
#include "board.hh"
#include "transposition_table.hh"
#include "evaluation.hh"
//#include "ia.hh"

//#include "ia.hh" // TODO

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;


namespace ai::env
{

    extern chessBoard::Board boardM;

    extern std::vector<chessBoard::Move> input_vect;
    extern std::vector<chessBoard::Move> input_vect_quiescence;

    extern std::unordered_map<uint64, transposition_table::Data>* transposition_table;
    extern std::unordered_map<uint64, transposition_table::Data>* transposition_table_quiescence;

    extern TimePoint act_start;
    extern int start_depth;

    extern const std::string my_name;
    extern bool is_opening;

    extern std::vector<std::pair<uint64, int>> vectBoard;
}