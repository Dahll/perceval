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
            return stand_pat;
        }
        if (alpha < stand_pat)
            alpha = stand_pat;
        const auto& inv_color = b.other_color(color_act);
        // Generate moves and order them
        auto moves = chessBoard::VMove();
        b.generate_capture_moves(color_act, moves);
        const auto& sorted_moves = ordering::moves_set_values_quiescence(b, moves, prev_move);//give hash
        if (moves.len == 0)
        {
            return stand_pat;
        }

        int bestscore = stand_pat;

        for (const auto& move : sorted_moves)
        {
            //auto cpy = helpers::copy_board(b);
            auto cpy = b;
            const uint64& next_hash = cpy.apply_move(move.second, color_act, hash);
            ai::meta.treefold.push(hash);
            const int& score = -quiesce(cpy, inv_color, -beta, -alpha, move.second, next_hash);
            ai::meta.treefold.pop();
            //b.revert_move(move.second, color_act);
            if (score >= beta)
            {
                return score;
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