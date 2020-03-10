//
// Created by adrien on 07/06/19.
//
#include "transposition_table.hh"




namespace ai::transposition_table
{

    TT tt_search = TT(64000);

    void TT::update(const chessBoard::Move& move, int score, int depth, uint64 hash, int is_cut_off) noexcept
    {
        auto& act_data = TT_[hash%size_];
        if (act_data.age_ < age_)
        {
            act_data.update(move, score, depth, hash, is_cut_off, age_);
        }
        else if (act_data.depth_ <= depth)
        {
            act_data.update(move, score, depth, hash, is_cut_off, age_);
        }
    }

    Data& TT::find(uint64 hash) noexcept
    {
        return TT_[hash % size_];
    }

    TT::TT(int size) // in K
    {
        int tmp_size = (size * 1024) / sizeof(Data);
        size_ = 2;
        while (size_ < tmp_size)
        {
            size_ *= 2;
        }
        TT_ = std::vector<Data>(size_);
        age_ = 0;
    }

    void TT::increment_age() noexcept
    {
        age_++;
    }

    void TT::reset()
    {
        for (auto &a : TT_)
        {
            a = Data();
        }
    }

    Data::Data()
    {
        depth_ = -1;
        move_ = chessBoard::Move();
        score_ = 0;
        hash_ = 0;
        is_cut_off_ = 0;
        age_ = -1;
    }

    void Data::update(const chessBoard::Move &move, int score, int depth, uint64 hash, int is_cut_off, int age) noexcept
    {
        hash_ = hash;
        move_ = move;
        score_ = score;
        depth_ = depth;
        is_cut_off_ = is_cut_off;
        age_ = age;
    }
}