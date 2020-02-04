//
// Created by adrien on 01/11/19.
//

#pragma once

#include "board.hh"
#include <unordered_map>

namespace ai::transposition_table
{

    struct Data
    {
        Data();
        void update(const chessBoard::Move& move, int score, int depth, uint64 hash, int is_cut_off, int age) noexcept;
        uint64 hash_;
        chessBoard::Move move_;
        int score_;
        int depth_;
        int8 is_cut_off_;
        int age_;
    };

    class TT
    {
    public:

        explicit TT(int size);
        void update(const chessBoard::Move& move, int score, int depth, uint64 hash, int is_cut_off) noexcept ;
        Data& find(uint64 hash) noexcept;
        void increment_age() noexcept;
        void reset();

    private:
        std::vector<Data> TT_;
        int size_;
        int age_;
    };

    extern TT tt_search;
}

