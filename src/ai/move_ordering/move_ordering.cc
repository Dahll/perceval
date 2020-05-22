//
// Created by adrien on 06/06/19.
//

#include "move_ordering.hh"
#include "../search/search.hh"

namespace ai::ordering
{

    int quiescence_calc_move(const chessBoard::Board& b, const chessBoard::Move& move,
            const std::optional<chessBoard::Move>& prev_move)
    {

        int ret = 0;



        //const chessBoard::enumPiece& type_piece = move.piece_get();
        //const chessBoard::enumPiece& type_capture = move.captured_piece_type_get();
        if (b.pieceBB[2] & move.getFromPosition())
        {
            ret -= 10;
        }
        else if (b.pieceBB[3] & move.getFromPosition())
        {
            ret -= VAL_KNIGHT;
        }
        else if (b.pieceBB[4] & move.getFromPosition())
        {
            ret -= VAL_BISHOP;
        }
        else if (b.pieceBB[5] & move.getFromPosition())
        {
            ret -= VAL_ROOKS;
        }
        else if (b.pieceBB[6] & move.getFromPosition())
        {
            ret -= VAL_QUEEN;
        }
        else if (b.pieceBB[7] & move.getFromPosition())
        {
            ret -= 1200;
        }
        if (b.pieceBB[2] & move.getToPosition())
        {
            ret += 100;
        }
        else if (b.pieceBB[3] & move.getToPosition())
        {
            ret += VAL_KNIGHT;
        }
        else if (b.pieceBB[4] & move.getToPosition())
        {
            ret += VAL_BISHOP;
        }
        else if (b.pieceBB[5] & move.getToPosition())
        {
            ret += VAL_ROOKS;
        }
        else if (b.pieceBB[6] & move.getToPosition())
        {
            ret += VAL_QUEEN;
        }
        else if (b.pieceBB[7] & move.getToPosition())
        {
            ret += 1200;
        }
        if ((prev_move.has_value()) /*&& (type_capture == prev_move.value().piece_get())*/ && (prev_move.value().getToIndex() == move.getToIndex()))
        {
            ret += 1001;
        }
        return ret;
    }



    int calc_move(const chessBoard::Board& b, const chessBoard::Move& move, int ply,
            const std::optional<chessBoard::Move>& prev_move, uint64 hash)
    {
        int ret = 0;
        if (move.isCapture())
        {
            //const chessBoard::enumPiece& type_piece = move.piece_get();
            //const chessBoard::enumPiece& type_capture = move.captured_piece_type_get();
            if (b.pieceBB[2] & move.getFromPosition())
            {
                ret -= 10;
            }
            else if (b.pieceBB[3] & move.getFromPosition())
            {
                ret -= VAL_KNIGHT;
            }
            else if (b.pieceBB[4] & move.getFromPosition())
            {
                ret -= VAL_BISHOP;
            }
            else if (b.pieceBB[5] & move.getFromPosition())
            {
                ret -= VAL_ROOKS;
            }
            else if (b.pieceBB[6] & move.getFromPosition())
            {
                ret -= VAL_QUEEN;
            }
            else if (b.pieceBB[7] & move.getFromPosition())
            {
                ret -= 1200;
            }
            if (b.pieceBB[2] & move.getToPosition())
            {
                ret += 100;
            }
            else if (b.pieceBB[3] & move.getToPosition())
            {
                ret += VAL_KNIGHT;
            }
            else if (b.pieceBB[4] & move.getToPosition())
            {
                ret += VAL_BISHOP;
            }
            else if (b.pieceBB[5] & move.getToPosition())
            {
                ret += VAL_ROOKS;
            }
            else if (b.pieceBB[6] & move.getToPosition())
            {
                ret += VAL_QUEEN;
            }
            else if (b.pieceBB[7] & move.getToPosition())
            {
                ret += 1200;
            }

            if ((prev_move.has_value()) /*&& (type_capture == prev_move.value().piece_get())*/ && (prev_move.value().getToIndex() == move.getToIndex()))
            {
                ret += 1001;
            }

        }
        if (ply < search::G_PV.length)
        {
            const chessBoard::Move &opti_move = search::G_PV.pv[ply];
            if (opti_move == move)
            {
                ret += 30000;
            }
        }

        const auto transpo = ai::transposition_table::tt_search.find(hash);

        if (transpo.depth_ != -1 && transpo.hash_ == hash)
        {
            if (transpo.move_ == move)
            {
                ret += 1000;
            }
        }
        return ret;
    }

    bool cmp_pair(const INT_MOVE_PAIR& pair1, const INT_MOVE_PAIR& pair2)
    {
        return pair1.first > pair2.first;
    }

    VECTOR_PAIR moves_set_values(const chessBoard::Board& b,const chessBoard::MOVES_T& vect,
            const std::optional<chessBoard::Move>& prev_move, int ply, uint64 hash)
    {
        VECTOR_PAIR ret;
        for (const auto& move : vect)
        {
            ret.emplace_back(calc_move(b, move, ply, prev_move, hash), move);
        }
        std::sort(ret.begin(), ret.end(), cmp_pair);
        return ret;
    }

    VECTOR_PAIR moves_set_values_quiescence(const chessBoard::Board& b, const chessBoard::MOVES_T& vect,
            const std::optional<chessBoard::Move>& prev_move)
    {
        VECTOR_PAIR ret;
        //auto ret = std::vector<std::pair<int, const chessBoard::Move>>();
        for (const auto& move : vect)
        {
            ret.emplace_back(quiescence_calc_move(b, move, prev_move), move);
        }
        std::sort(ret.begin(), ret.end(), cmp_pair);
        return ret;
    }
}