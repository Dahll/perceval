//
// Created by adrien on 06/06/19.
//

#include <strings.h>
#include <chessBoard.hh>
#include "ia.hh"

namespace ai
{

    int IA::quiescence_calc_move(const chessBoard::Move& move,
            const std::optional<chessBoard::Move>& prev_move, size_t depth, uint64 hash)
    {
        int ret = 0;
        const chessBoard::enumPiece& type_piece = move.piece_get();
        const chessBoard::enumPiece& type_capture = move.captured_piece_type_get();
        if (type_piece == 2)
        {
            ret -= 10;
        }
        else if (type_piece == 3)
        {
            ret -= KNIGHT;
        }
        else if (type_piece == 4)
        {
            ret -= BISHOP;
        }
        else if (type_piece == 5)
        {
            ret -= ROOKS;
        }
        else if (type_piece == 6)
        {
            ret -= QUEEN;
        }
        else if (type_piece == 7)
        {
            ret -= 1200;
        }
        if (type_capture == 2)
        {
            ret += 100;
        }
        else if (type_capture == 3)
        {
            ret += KNIGHT;
        }
        else if (type_capture == 4)
        {
            ret += BISHOP;
        }
        else if (type_capture == 5)
        {
            ret += ROOKS;
        }
        else if (type_capture == 6)
        {
            ret += QUEEN;
        }
        else if (type_capture == 7)
        {
            ret += 1200;
        }
        if ((prev_move.has_value()) && (type_capture == prev_move.value().piece_get()) && (prev_move.value().to_get() == move.to_get()))
        {
            ret += 1001;
        }
        if (input_vect_quiescence.size() > depth)
        {
            const chessBoard::Move& opti_move = input_vect_quiescence.at(depth);
            if ((move.piece_get() == opti_move.piece_get()) && (move.to_get() == opti_move.to_get()) &&
                (move.from_get() == opti_move.from_get()))
            {
                ret += 30000;
            }
        }
        const auto& transp = transposition_table_quiescence->find(hash);

        if (transp != transposition_table_quiescence->end() && transp->second.move_has_value())
        {
            if (transp->second.move_get().from_get() == move.from_get() && transp->second.move_get().to_get() == move.to_get() &&
            transp->second.move_get().piece_get() == move.piece_get())
            {
                ret += 1000;
            }
        }
        return ret;
    }



    int IA::calc_move(const chessBoard::Move& move, size_t act_depth,
            const std::optional<chessBoard::Move>& prev_move, uint64 hash)
    {
        int ret = 0;
        if (move.is_capture())
        {
            const chessBoard::enumPiece& type_piece = move.piece_get();
            const chessBoard::enumPiece& type_capture = move.captured_piece_type_get();
            if (type_piece == 2)
            {
                ret -= 10;
            }
            else if (type_piece == 3)
            {
                ret -= KNIGHT;
            }
            else if (type_piece == 4)
            {
                ret -= BISHOP;
            }
            else if (type_piece == 5)
            {
                ret -= ROOKS;
            }
            else if (type_piece == 6)
            {
                ret -= QUEEN;
            }
            else if (type_piece == 7)
            {
                ret -= 1200;
            }
            if (type_capture == 2)
            {
                ret += 100;
            }
            else if (type_capture == 3)
            {
                ret += KNIGHT;
            }
            else if (type_capture == 4)
            {
                ret += BISHOP;
            }
            else if (type_capture == 5)
            {
                ret += ROOKS;
            }
            else if (type_capture == 6)
            {
                ret += QUEEN;
            }
            else if (type_capture == 7)
            {
                ret += 1200;
            }

            if ((prev_move.has_value()) && (type_capture == prev_move.value().piece_get()) && (prev_move.value().to_get() == move.to_get()))
            {
                ret += 1001;
            }

        }
        if (input_vect.size() > start_depth - act_depth)
        {
            const chessBoard::Move& opti_move = input_vect.at(start_depth - act_depth);
            if ((move.piece_get() == opti_move.piece_get()) && (move.to_get() == opti_move.to_get()) &&
                (move.from_get() == opti_move.from_get()))
            {
                ret += 30000;
            }
        }

        const auto transp = transposition_table->find(hash);

        if (transp != transposition_table->end() && transp->second.move_has_value())
        {
            if (transp->second.move_get().from_get() == move.from_get() && transp->second.move_get().to_get() == move.to_get() &&
                transp->second.move_get().piece_get() == move.piece_get())
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

    VECTOR_PAIR IA::moves_set_values(const chessBoard::MOVES_T& vect,
            const std::optional<chessBoard::Move>& prev_move, int depth, uint64 hash)
    {
        VECTOR_PAIR ret;
        for (const auto& move : vect)
        {
            ret.emplace_back(calc_move(move, depth, prev_move, hash), move);
        }
        std::sort(ret.begin(), ret.end(), cmp_pair);
        return ret;
    }

    VECTOR_PAIR IA::moves_set_values_quiescence(const chessBoard::MOVES_T& vect,
            const std::optional<chessBoard::Move>& prev_move, int depth, uint64 hash)
    {
        VECTOR_PAIR ret;
        //auto ret = std::vector<std::pair<int, const chessBoard::Move>>();
        for (const auto& move : vect)
        {
            ret.emplace_back(quiescence_calc_move(move, prev_move, depth, hash), move);
        }
        std::sort(ret.begin(), ret.end(), cmp_pair);
        return ret;
    }

    void merge_vect(chessBoard::MOVES_T& vect1, chessBoard::MOVES_T& vect2)
    {
        vect1.resize(1);
        for (const auto move : vect2)
        {
            vect1.push_back(move);
        }
    }


    /*void IA::add_transposition_table(const chessBoard::Board& board)
    {

    }

    void IA::add_transposition_table_quiescence(, const chessBoard::Move& move, int value)
    {

    }*/
}