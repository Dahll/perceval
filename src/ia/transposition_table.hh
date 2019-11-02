//
// Created by adrien on 01/11/19.
//

#pragma once

#include "board.hh"

namespace transposition_table
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

    void update_transposition_table(const std::optional<chessBoard::Move>& move, int score, int depth, uint64 hash, int is_cut_off);
    void update_transposition_table_quiescence(const std::optional<chessBoard::Move>& move, int score, uint64 hash, int is_cut_off);

}