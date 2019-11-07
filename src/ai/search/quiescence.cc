//
// Created by adrien on 25/10/19.
//
#include "search.hh"

namespace ai::search
{
    int quiesce(const chessBoard::enumPiece& color_act,
                    int alpha, int beta, const chessBoard::Move& prev_move, uint64 hash)
    {
        const int& stand_pat = evaluation::evaluate(color_act);
        auto actual_vect = std::vector<chessBoard::Move>();
        if (stand_pat >= beta)
        {
            return beta;
        }
        if (alpha < stand_pat)
            alpha = stand_pat;
        const auto& inv_color = chessBoard::boardM.other_color(color_act);
        const auto& moves = chessBoard::boardM.generate_capture_moves(color_act);
        const auto& sorted_moves = ordering::moves_set_values_quiescence(moves, prev_move);//give hash
        if (moves.empty())
        {
            return alpha;
        }
        actual_vect.emplace_back(sorted_moves[0].second);
        for (const auto& move : sorted_moves)
        {
            const uint64& next_hash = chessBoard::boardM.apply_move(move.second, color_act, hash);
            const int& score = -quiesce(inv_color, -beta, -alpha, move.second, next_hash);
            chessBoard::boardM.revert_move(move.second, color_act);
            if (score >= beta)
            {
                return beta;
            }
            if (score > alpha)
            {
                alpha = score;
            }
            actual_vect.resize(0);
        }
        return alpha;
    }
}
