//
// Created by adrien on 30/05/19.
//
#include "ia.hh"

namespace ai
{
    int IA::caller_alphabeta(int depth,
            chessBoard::MOVES_T& output_vect,
            chessBoard::MOVES_T& output_vect_quiescence, uint64 hash)
    {
        int alpha = INT32_MIN + 1;
        int beta = INT32_MAX;
        auto save_alpha = alpha;
        auto transpo = transposition_table->find(hash);
        if (transpo != transposition_table->end() && transpo->second.depth_get() >= depth)
        {
            if (transpo->second.is_cut_off_get() == 0)
                return transpo->second.score_get();
            else if (transpo->second.is_cut_off_get() == -1)
            {
                alpha = std::max(alpha, transpo->second.score_get());
            }
            else
            {
                beta = std::min(beta, transpo->second.score_get());
            }
            if (alpha >= beta)
                return transpo->second.score_get();
        }

        auto actual_vect = std::vector<chessBoard::Move>();
        int best = INT32_MIN + 1;
        const auto& inv_color = boardM.other_color(boardM.color);
        const auto& colo_act = boardM.color;

        //test if the board exist in the hash map and if depth left == depth stocked
        const auto& mov = boardM.generate_moves(colo_act);
        auto moves = helpers::remove_move_repetition(mov, boardM,  vectBoard, hash);
        if (moves.empty())
            return INT32_MIN + 1;
        auto sorted_moves = moves_set_values(moves, std::nullopt, depth, hash);
        output_vect.push_back(sorted_moves[0].second);
        int score = 0;
        for (const auto& move : sorted_moves)
        {
            const uint64& next_hash = boardM.apply_move(move.second, colo_act, hash);
            score = -alphabeta(inv_color, depth - 1, -beta, -alpha, move.second, actual_vect, output_vect_quiescence, next_hash);
            boardM.revert_move(move.second, colo_act);
            if (score >= beta)
            {
                output_vect[0] = move.second;
                merge_vect(output_vect, actual_vect);
                if (transpo == transposition_table->end() || transpo->second.depth_get() < depth)
                    update_transposition_table(output_vect[0], score, depth, hash, -1);
                return score;
            }
            if (score > best)
            {
                best = score;
                merge_vect(output_vect, actual_vect);
                output_vect[0] = move.second;
                if (score > alpha)
                {
                    alpha = score;
                }
            }
            actual_vect.resize(0);
        }
        if (transpo == transposition_table->end() || transpo->second.depth_get() < depth)
        {
            if (best <= save_alpha)
            {
                update_transposition_table(output_vect[0], best, depth, hash, 1);
            }
            else if (best >= beta)
            {
                    update_transposition_table(output_vect[0], best, depth, hash, -1);
            }
            else
            {
                //if (transpo == transposition_table->end() || transpo->second.depth_get() < depth)
                    update_transposition_table(output_vect[0], best, depth, hash, 0);
            }
        }
        return best;
    }


    int IA::alphabeta(const chessBoard::enumPiece& colo_act,
            int depth ,int alpha, int beta, const chessBoard::Move& prev_move,
            chessBoard::MOVES_T& prev_vect_move,
            chessBoard::MOVES_T& prev_vect_move_quiescence, uint64 hash)
    {

        //test if the board exist in the hash map and if depth left == depth stocked
        auto transpo = transposition_table->find(hash);
        auto save_alpha = alpha;
        if (transpo != transposition_table->end() && transpo->second.depth_get() >= depth)
        {
            if (transpo->second.is_cut_off_get() == 0)
                return transpo->second.score_get();
            else if (transpo->second.is_cut_off_get() == -1)
            {
                alpha = std::max(alpha, transpo->second.score_get());
            } else
            {
                beta = std::min(beta, transpo->second.score_get());
            }
            if (alpha >= beta)
                return transpo->second.score_get();
        }

        if (depth == 0)
        {
            return quiesce(colo_act, alpha, beta, prev_move, 0, prev_vect_move_quiescence, hash);
        }

        auto actual_vect = chessBoard::MOVES_T();
        int best = INT32_MIN + 1;
        const auto &moves = boardM.generate_moves(colo_act);
        if (moves.empty())
        {
            if (!boardM.player_is_check(colo_act))
                return 0;
            return best;
        }
        const auto &sorted_moves = moves_set_values(moves, prev_move, depth, hash);//give hash
        const auto &inv_color = boardM.other_color(colo_act);
        prev_vect_move.push_back(sorted_moves[0].second);
        for (const auto &move : sorted_moves)
        {
            uint64 next_hash = boardM.apply_move(move.second, colo_act, hash);
            int score = -alphabeta(inv_color, depth - 1, -beta, -alpha, move.second, actual_vect,
                                   prev_vect_move_quiescence, next_hash);
            boardM.revert_move(move.second, colo_act);
            if (score >= beta)
            {
                merge_vect(prev_vect_move, actual_vect);
                prev_vect_move[0] = move.second;
                if (transpo == transposition_table->end() || transpo->second.depth_get() < depth)
                    update_transposition_table(move.second, score, depth, hash, -1);
                return score;
            }
            if (score > best)
            {
                merge_vect(prev_vect_move, actual_vect);
                prev_vect_move[0] = move.second;
                best = score;
                if (score > alpha)
                    alpha = score;
            }
            actual_vect.resize(0);
        }

        if (transpo == transposition_table->end() || transpo->second.depth_get() < depth)
        {
            if (best <= save_alpha)
            {
                update_transposition_table(prev_vect_move[0], best, depth, hash, 1);
            }
            else if (best >= beta)
            {
                update_transposition_table(prev_vect_move[0], best, depth, hash, -1);
            }
            else
            {
                //if (transpo == transposition_table->end() || transpo->second.depth_get() < depth)
                    update_transposition_table(prev_vect_move[0], best, depth, hash, 0);
            }
        }
        return best;
    }


}
