//
// Created by adrien on 25/10/19.
//
#include "search.hh"

namespace ai::search
{
    int quiesce(const chessBoard::enumPiece& color_act,
                    int alpha, int beta, const chessBoard::Move& prev_move, uint64 hash)
    {
        // Evaluate and break with evaluate result
        const int& stand_pat = evaluation::evaluate(color_act);
        if (stand_pat >= beta)
        {
            return beta;
        }
        if (alpha < stand_pat)
            alpha = stand_pat;
        const auto& inv_color = chessBoard::boardM.other_color(color_act);
        // Generate moves and order them
        const auto& moves = chessBoard::boardM.generate_capture_moves(color_act);
        const auto& sorted_moves = ordering::moves_set_values_quiescence(moves, prev_move);//give hash
        if (moves.empty())
        {
            return alpha;
        }
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
        }
        return alpha;
    }
}