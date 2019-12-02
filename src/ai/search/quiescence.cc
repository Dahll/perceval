//
// Created by adrien on 25/10/19.
//
#include "search.hh"

namespace ai::search
{
    int quiesce(const chessBoard::Board& b, const chessBoard::enumPiece& color_act,
                    int alpha, int beta, const chessBoard::Move& prev_move, uint64 hash)
    {
        // Out of time
        if (!meta.running)
            return 0;
        // Evaluate and break with evaluate result
        const int& stand_pat = evaluation::evaluate(b, color_act);
        if (stand_pat >= beta)
        {
            return beta;
        }
        if (alpha < stand_pat)
            alpha = stand_pat;
        const auto& inv_color = b.other_color(color_act);
        // Generate moves and order them
        const auto& moves = b.generate_capture_moves(color_act);
        const auto& sorted_moves = ordering::moves_set_values_quiescence(moves, prev_move);//give hash
        if (moves.empty())
        {
            return alpha;
        }

        int bestscore = stand_pat;

        for (const auto& move : sorted_moves)
        {
            auto cpy = helpers::copy_board(b);
            const uint64& next_hash = cpy.apply_move(move.second, color_act, hash);
            const int& score = -quiesce(cpy, inv_color, -beta, -alpha, move.second, next_hash);
            //b.revert_move(move.second, color_act);
            if (score >= beta)
            {
                return beta;
            }
            if (score > bestscore)
            {
                bestscore = score;
                if (score > alpha)
                {
                    alpha = score;
                }
            }
        }
        return bestscore;
    }
}