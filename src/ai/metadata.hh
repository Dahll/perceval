//
// Created by adrien on 20/11/19.
//

#pragma once

#include "../board/move.hh"
#include "../board/board.hh"

namespace ai
{
    class TreefoldStack
    {
    public:
        bool is_treefold(uint64 hash);
        void push(uint64 hash);
        void pop();
        void reset();
        void set_root_index();

    private:
        int root_index = 0;
        int stack_index = 0;
        std::vector<uint64> stack = std::vector<uint64>(256);
    };

    class Metadata
    {
    public:


        /* Description option*/
        const std::string my_name = "Perceval";

        /* Core option*/

        /* Treefold and 50 moves repetition */
        TreefoldStack treefold = TreefoldStack();
        std::vector<std::pair<uint64, int>> vectBoard = std::vector<std::pair<uint64, int>>{};



        // Multithreading info
        bool multithread = false;
        bool running = false;

        // Board info
        chessBoard::Board boardM = chessBoard::Board();
        uint64 hash = 0;
        chessBoard::Move best_move = chessBoard::Move();


        /* Time options */
        int time_to_play;
        int wtime = 300000;
        int btime = 300000;
        int winc = 0;
        int binc = 0;
        void set_time_to_play();
    };

    extern Metadata meta;
}
