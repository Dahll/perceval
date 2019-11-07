//
// Created by adrien on 07/06/19.
//
#include "transposition_table.hh"




namespace ai::transposition_table
{

    TT tt_search = TT();

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


    void TT::init() noexcept
    {
        TT = new std::unordered_map<uint64 , transposition_table::Data>();
    }

    void TT::clean() noexcept
    {
        delete(TT);
        TT = nullptr;
    }

    void TT::update(const std::optional<chessBoard::Move>& move, int score, int depth, uint64 hash, int is_cut_off) noexcept
    {
        auto a = TT->find(hash);
        if (a != TT->end())
        {
            a->second.move_set(move);
            a->second.score_set(score);
            a->second.depth_set(depth);
            a->second.is_cut_off_set(is_cut_off);
        }
        else
        {
            auto data = Data(move, score, depth, is_cut_off);
            TT->insert({hash, data});
        }
    }

    std::unordered_map<uint64 , ai::transposition_table::Data>::iterator TT::get(uint64 hash) noexcept
    {
        return TT->find(hash);
    }


    const std::unordered_map<uint64 , ai::transposition_table::Data>::iterator TT::end() const noexcept
    {
        return TT->end();
    }
}