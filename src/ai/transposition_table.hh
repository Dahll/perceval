//
// Created by adrien on 01/11/19.
//

#pragma once

#include "board.hh"
#include <unordered_map>

namespace ai::transposition_table
{

    class Data
    {
    public:
        Data(const std::optional<chessBoard::Move> move, int score, int depth, int is_cut_off) : move_(move), score_(score), depth_(depth), is_cut_off_(is_cut_off)
        {}
        int score_get();
        void score_set(int score);

        chessBoard::Move move_get();
        void move_set(std::optional<chessBoard::Move> move); 

        int depth_get();
        void depth_set(int depth);

        int is_cut_off_get();
        void is_cut_off_set(int is_cut_off);

        bool move_has_value();

    private:
        std::optional<chessBoard::Move> move_;
        int score_;
        int depth_;
        int is_cut_off_;
    };

    class TT
    {
    public:

        void init() noexcept;
        void clean() noexcept;
        void update(const std::optional<chessBoard::Move>& move, int score, int depth, uint64 hash, int is_cut_off) noexcept ;
        std::unordered_map<uint64 , ai::transposition_table::Data>::iterator get(uint64 hash) noexcept ;
        const std::unordered_map<uint64 , ai::transposition_table::Data>::iterator end() const noexcept ;

    private:
        std::unordered_map<uint64 , Data>* TT = nullptr;
    };

    extern TT tt_search;
    extern TT tt_quiesc;
}

