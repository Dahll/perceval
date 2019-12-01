//
// Created by adrien on 20/11/19.
//

#pragma once

#include <move.hh>
#include "board.hh"

#define BASE_TIME 5 /* 5 */


namespace ai
{
    class Metadata
    {
    public:


        /* Description option*/
        const std::string my_name = "Perceval";

        /* Core option*/

        //Move repetition
        std::vector<std::pair<uint64, int>> vectBoard = std::vector<std::pair<uint64, int>>{};

        int hash = 0;
        bool multithread = false;
        bool running = false;

        int time_to_play;
        chessBoard::Move best_move = chessBoard::Move();


        /* Time options */
        int wtime = 300000;
        int btime = 300000;
        int winc = 0;
        int binc = 0;
        void set_time_to_play();
        void test_time_to_play();
    };

    extern Metadata meta;
}
