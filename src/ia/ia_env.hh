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
#include "evaluation/evaluation.hh"


namespace ai::env
{
    extern std::vector<chessBoard::Move> input_vect;
    extern std::vector<chessBoard::Move> input_vect_quiescence;


}