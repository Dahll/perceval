//
// Created by adrien on 15/05/20.
//

#pragma once
#include <algorithm>
#include <map>
#include <strings.h>
#include "stock.hh"
#include "../../board/board.hh"

namespace ai::evaluation
{

    struct EG
    {
        int positional_value = 0;
    };

    extern int pieces_values_eg[6];
    extern int psqt_values_eg[6][64];


    int end_game_evaluation(const EG& eg);
}
