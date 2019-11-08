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
        int score_get() const;
        void score_set(int score);

        chessBoard::Move move_get() const;
        void move_set(std::optional<chessBoard::Move> move); 

        int depth_get() const;
        void depth_set(int depth);

        int is_cut_off_get() const;
        void is_cut_off_set(int is_cut_off);

        bool move_has_value() const;

    private:
        std::optional<chessBoard::Move> move_;
        int score_;
        int depth_;
        int is_cut_off_;
    };

    class TT
    {
    public:

        TT(int size = 2048);
        void init() noexcept;
        void clean() noexcept;
        void update(const std::optional<chessBoard::Move>& move, int score, int depth, uint64 hash, int is_cut_off, std::unordered_map<uint64 , ai::transposition_table::Data>::iterator& transpo) noexcept ;
        std::unordered_map<uint64 , ai::transposition_table::Data>::iterator get(uint64 hash) noexcept ;
        const std::unordered_map<uint64 , ai::transposition_table::Data>::iterator end() const noexcept ;

    private:
        std::unordered_map<uint64 , Data>* TT_ = nullptr;
        int size_;
    };

    extern TT tt_search;
}

