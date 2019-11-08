//
// Created by adrien on 07/06/19.
//
#include "transposition_table.hh"




namespace ai::transposition_table
{

    TT tt_search = TT();

    chessBoard::Move Data::move_get() const
    {
        return move_.value();
    }

    void Data::move_set(std::optional<chessBoard::Move> move)
    {
        move_ = move;
    }

    bool Data::move_has_value() const
    {
        return move_.has_value();
    }

    int Data::score_get() const
    {
        return score_;
    }

    void Data::score_set(int score)
    {
        score_ = score;
    }

    int Data::depth_get() const
    {
        return depth_;
    }

    void Data::depth_set(int depth)
    {
        depth_ = depth;
    }

    int Data::is_cut_off_get() const
    {
        return is_cut_off_;
    }

    void Data::is_cut_off_set(int is_cut_off)
    {
        is_cut_off_ = is_cut_off;
    }


    void TT::init() noexcept
    {
        TT_ = new std::unordered_map<uint64 , transposition_table::Data>();
    }

    void TT::clean() noexcept
    {
        delete(TT_);
        TT_ = nullptr;
    }

    void TT::update(const std::optional<chessBoard::Move>& move, int score, int depth, uint64 hash, int is_cut_off
            , std::unordered_map<uint64 , ai::transposition_table::Data>::iterator& transpo) noexcept
    {
        //auto a = TT_->find(hash);

        if (transpo == TT_->end())
        {
            auto data = Data(move, score, depth, is_cut_off);
            TT_->insert({hash, data});
        }
        else if (transpo->second.depth_get() < depth)
        {
            transpo->second.move_set(move);
            transpo->second.score_set(score);
            transpo->second.depth_set(depth);
            transpo->second.is_cut_off_set(is_cut_off);
        }

    }

    std::unordered_map<uint64 , ai::transposition_table::Data>::iterator TT::get(uint64 hash) noexcept
    {
        return TT_->find(hash);
    }


    const std::unordered_map<uint64 , ai::transposition_table::Data>::iterator TT::end() const noexcept
    {
        return TT_->end();
    }

    TT::TT(int size)
    {
        size_ = size;
    }
}