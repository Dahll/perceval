//
// Created by adrien on 25/10/19.
//
#include "ia.hh"
#include "ia_env.hh"

namespace ai
{
    int IA::quiesce(const chessBoard::enumPiece& color_act,
                    int alpha, int beta, const chessBoard::Move& prev_move, int depth,
                    chessBoard::MOVES_T& prev_vect_move_quiescence, uint64 hash)
    {
        //test if the board exist in the hash map
        /*auto transpo = transposition_table_quiescence->find(hash);
        if (transpo != transposition_table_quiescence->end())
        {
            if (transpo->second.is_cut_off_get() == 0)
                return transpo->second.score_get();
            else if (transpo->second.is_cut_off_get() == -1)
            {
                if (transpo->second.score_get() <= alpha)
                    return alpha;
                if (transpo->second.score_get() < beta)
                    beta = transpo->second.score_get();
            }
            else
            {
                if (transpo->second.score_get() >= beta)
                    return beta;
                if (transpo->second.score_get() > alpha)
                    alpha = transpo->second.score_get();

            }
        }*/
        const int& stand_pat = evaluation::evaluate(color_act);
        auto actual_vect = std::vector<chessBoard::Move>();
        if (stand_pat >= beta)
        {

            transposition_table::update_transposition_table_quiescence(std::nullopt, beta, hash, 1);
            return beta;
        }
        if (alpha < stand_pat)
            alpha = stand_pat;
        const auto& inv_color = env::boardM.other_color(color_act);
        const auto& moves = env::boardM.generate_capture_moves(color_act);
        const auto& sorted_moves = moves_set_values_quiescence(moves, prev_move, depth, hash);//give hash
        if (moves.empty())
        {
            transposition_table::update_transposition_table_quiescence(std::nullopt, alpha, hash, -1);
            return alpha;
        }
        actual_vect.emplace_back(sorted_moves[0].second);
        for (const auto& move : sorted_moves)
        {
            const uint64& next_hash = env::boardM.apply_move(move.second, color_act, hash);
            const int& score = -quiesce(inv_color, -beta, -alpha, move.second, depth + 1, actual_vect, next_hash);
            env::boardM.revert_move(move.second, color_act);
            if (score >= beta)
            {
                merge_vect(prev_vect_move_quiescence, actual_vect);
                prev_vect_move_quiescence[0] = move.second;
                //if (transpo == transposition_table->end() || transpo->second.depth_get() < depth)
                transposition_table::update_transposition_table_quiescence(move.second, beta, hash, 1);
                return beta;
            }
            if (score > alpha)
            {
                merge_vect(prev_vect_move_quiescence, actual_vect);
                prev_vect_move_quiescence[0] = move.second;
                alpha = score;
            }
            actual_vect.resize(0);
        }
        //if (transpo == transposition_table->end() || transpo->second.depth_get() < depth)
        //{
        if (prev_vect_move_quiescence.empty())
            transposition_table::update_transposition_table_quiescence(std::nullopt, alpha, hash, 0);
        else
            transposition_table::update_transposition_table_quiescence(prev_vect_move_quiescence[0], alpha, hash, 0);
        //}
        return alpha;
    }
}
