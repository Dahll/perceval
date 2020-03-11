//
// Created by fouche_r on 5/19/19.
//

#include "adapter.hh"
#include "convertion.hh"
#include <functional>
#include <strings.h>

namespace adapter
{

    chessBoard::Board copy_board(const Board& b)
    {
        Board n;
        n.pieceBB[0] = b.pieceBB[0];
        n.pieceBB[1] = b.pieceBB[1];
        n.pieceBB[2] = b.pieceBB[2];
        n.pieceBB[3] = b.pieceBB[3];
        n.pieceBB[4] = b.pieceBB[4];
        n.pieceBB[5] = b.pieceBB[5];
        n.pieceBB[6] = b.pieceBB[6];
        n.pieceBB[7] = b.pieceBB[7];
        n.castlings = b.castlings;
        n.special_moves = b.special_moves;
        n.color = b.color;
        n.mat = n.mat;
        n.check = b.check;
        n.pat = b.pat;
        return n;
    }

    bool cmp_boards(const Board& b, const Board& n)
    {
        if (n.pieceBB[0] != b.pieceBB[0])
            return false;
        if (n.pieceBB[1] != b.pieceBB[1])
            return false;
        if (n.pieceBB[2] != b.pieceBB[2])
            return false;
        if (n.pieceBB[3] != b.pieceBB[3])
            return false;
        if (n.pieceBB[4] != b.pieceBB[4])
            return false;
        if (n.pieceBB[5] != b.pieceBB[5])
            return false;
        if (n.pieceBB[6] != b.pieceBB[6])
            return false;
        if (n.pieceBB[7] != b.pieceBB[7])
            return false;
        if (n.castlings != b.castlings)
            return false;
        if (n.special_moves != b.special_moves)
            return false;
        if (!(n.color == b.color))
            return false;
        if (n.mat != b.mat)
            return false;
        if (n.check != b.check)
            return false;
        return n.pat == b.pat;
    }

    bool Adapter::disqualify_if_false(bool cond)
    {
        if (!cond)
            for (auto &l : listeners)
                l->on_player_disqualified(
                        enumPiece_to_color(boardM.color));
        return !cond;
    }

    void Adapter::game_finished_sends()
    {
        for (auto& l : listeners)
            l->on_game_finished();
    }


    bool Adapter::player_is_mat(chessBoard::Move &move,
                                const yaka::PgnMove& ref)
    {
        if (disqualify_if_false(
                boardM.mat ==
                (ref.report_get() == ReportType::CHECKMATE)))
            return true;
        if (boardM.mat)
        {
            for (auto& l : listeners)
                l->on_player_mat(
                        other_color(enumPiece_to_color(boardM.color)));
            return true;
        }

        if (disqualify_if_false(
                boardM.check ==
                (ref.report_get() == ReportType::CHECK)))
            return true;
        if (boardM.check)
        {
            for (auto& l : listeners)
                l->on_player_check(
                        other_color(enumPiece_to_color(boardM.color)));
        }

        if (boardM.pat)
        {
            for (auto& l : listeners)
                l->on_player_pat(
                        other_color(enumPiece_to_color(boardM.color)));
     //       for (auto& l : listeners)
   //             l->on_draw();
    //        return true;
        }
        else if (!boardM.game_is_draw(move))
            return false;

        for (auto& l : listeners)
            l->on_draw();
        return true;
    }


    bool threefold(Bvect& vect, Board b)
    {
        bool t = false;
        for (auto& p : vect)
        {
            if (cmp_boards(p.first, b))
            {
                t = true;
                p.second++;
                if (p.second == 3)
                    return true;
            }
        }
        if (!t)
            vect.emplace_back(b, 1);
        return false;
    }


    /* Returns true if game is finished */
    bool Adapter::listener_calls(
            chessBoard::Move &move,
            const yaka::PgnMove& ref,
            Bvect& vect)
    {
        std::optional<chessBoard::Move> m = boardM.check_apply_move(move);
        Board b = copy_board(boardM);
        if (m.has_value())
        {
            chessBoard::Move move1{m.value()};
            if (disqualify_if_false((move1.isQueenCastle() || move1.isKingCastle()) == ref.is_castling()))
                return true;
            if ((move1.isQueenCastle() || move1.isKingCastle()))
            {
                // Castling done
                for (auto& l : listeners)
                    l->on_piece_moved(yaka::PieceType::KING,
                                      ref.start_get(),
                                      ref.end_get());
                if (move1.isQueenCastle())
                    for (auto& l : listeners)
                        l->on_queenside_castling(
                                enumPiece_to_color(boardM.color));
                else
                    for (auto& l : listeners)
                        l->on_kingside_castling(
                                enumPiece_to_color(boardM.color));
                return player_is_mat(move1, ref);
            }
            // No castling
            for (auto& l : listeners)
                l->on_piece_moved(ref.piece_get(),
                                  ref.start_get(),
                                  ref.end_get());
            if (disqualify_if_false(move1.isCapture() == ref.capture_get()))
                return true;
            if (move1.isCapture())
                for (auto& l : listeners)
                {
                    int capture_piece_type = 0;
                    if (boardM.pieceBB[2] & move1.getToPosition())
                        capture_piece_type = 2;
                    else if (boardM.pieceBB[3] & move1.getToPosition())
                        capture_piece_type = 3;
                    else if (boardM.pieceBB[4] & move1.getToPosition())
                        capture_piece_type = 4;
                    else if (boardM.pieceBB[5] & move1.getToPosition())
                        capture_piece_type = 5;
                    else if (boardM.pieceBB[6] & move1.getToPosition())
                        capture_piece_type = 6;
                    else if (boardM.pieceBB[7] & move1.getToPosition())
                        capture_piece_type = 7;
                    l->on_piece_taken(
                            enumPiece_to_PieceType(
                                    static_cast<const enumPiece>(capture_piece_type)),
                            index_to_position(
                                    ffsll(boardM.captured_piece_position(move1))));
                }
            if (disqualify_if_false(
                    (move1.getFlags() > 7) == ref.promotion_get().has_value()))
                return true;
            if (move1.getFlags() > 7) {
                for (auto &l : listeners)
                    l->on_piece_promoted(
                            *ref.promotion_get(), ref.end_get());
            }
            /// check for threefold repetition
            if (threefold(vect, b) || boardM.half_move_count_ == 100)
            {
                player_is_mat(move1, ref);
                for (auto &l : listeners)
                    l->on_draw();
                return true;
            }
            return player_is_mat(move1, ref);
        }

        for (auto& l : listeners)
            l->on_player_disqualified(enumPiece_to_color(boardM.color));
        return true;
    }

    void Adapter::init_listeners()
    {
        for (auto &l : listeners)
            l->register_board(*this);
    }
}