//
// Created by adrien on 03/11/19.
//

#pragma once

#include "board.hh"

namespace ai::refutation_table
{
    void merge_vect(std::vector<chessBoard::Move>& vect1, std::vector<chessBoard::Move>& vect2);

    extern std::vector<chessBoard::Move> input_vect;
}