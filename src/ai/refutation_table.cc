//
// Created by adrien on 03/11/19.
//

#include "refutation_table.hh"

namespace ai::refutation_table
{

    std::vector<chessBoard::Move> input_vect = std::vector<chessBoard::Move>();


    void merge_vect(chessBoard::MOVES_T& vect1, chessBoard::MOVES_T& vect2)
    {
        vect1.resize(1);
        for (const auto move : vect2)
        {
            vect1.push_back(move);
        }
    }
}