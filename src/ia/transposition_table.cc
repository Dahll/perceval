//
// Created by adrien on 07/06/19.
//
#include "transposition_table.hh"
#include "ia_env.hh"

namespace transposition_table
{

    chessBoard::Move Data::move_get()
    {
        return move_.value();
    }

    void Data::move_set(std::optional<chessBoard::Move> move)
    {
        move_ = move;
    }

    bool Data::move_has_value()
    {
        return move_.has_value();
    }

    int Data::score_get()
    {
        return score_;
    }

    void Data::score_set(int score)
    {
        score_ = score;
    }

    int Data::depth_get()
    {
        return depth_;
    }

    void Data::depth_set(int depth)
    {
        depth_ = depth;
    }

    int Data::is_cut_off_get()
    {
        return is_cut_off_;
    }

    void Data::is_cut_off_set(int is_cut_off)
    {
        is_cut_off_ = is_cut_off;
    }



    void update_transposition_table(const std::optional<chessBoard::Move>& move, int score, int depth, uint64 hash, int is_cut_off)
    {
        auto a = ai::env::transposition_table->find(hash);
        if (a != ai::env::transposition_table->end())
        {
            // need to change and overload =
            a->second.move_set(move);
            a->second.score_set(score);
            a->second.depth_set(depth);
            a->second.is_cut_off_set(is_cut_off);
        }
        else
        {
            auto data = Data(move, score, depth, is_cut_off);
            ai::env::transposition_table->insert({hash, data});
        }

    }

    void update_transposition_table_quiescence(const std::optional<chessBoard::Move>& move, int score, uint64 hash, int is_cut_off)
    {
        auto a = ai::env::transposition_table_quiescence->find(hash);
        if (a != ai::env::transposition_table_quiescence->end())
        {
            // need to change and overload =
            //a->second.move_set(move);
            a->second.score_set(score);
            //a->second.depth_set(0);
            a->second.is_cut_off_set(is_cut_off);
        }
        else
        {
            auto data = Data(move, score, 0, is_cut_off);
            ai::env::transposition_table_quiescence->insert({hash, data});
        }
    }

}