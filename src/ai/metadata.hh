//
// Created by adrien on 20/11/19.
//

#pragma once

#include <move.hh>

namespace ai
{
    class Metadata
    {
    public:
        int hash = 0;
        bool multithread = false;
        bool running = false;

        chessBoard::Move best_move = chessBoard::Move();


        /* Time options */
        int wtime = 300000;
        int btime = 300000;
        int winc = 0;
        int binc = 0;

    };

    extern Metadata meta;
}
