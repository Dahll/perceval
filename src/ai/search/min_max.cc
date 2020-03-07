//
// Created by adrien on 30/05/19.
//
#include "search.hh"

namespace ai::search
{
    chessBoard::Move caller_alphabeta(const chessBoard::Board& b, int depth,
                         chessBoard::MOVES_T &output_vect,
                         uint64 hash,
                         bool& winning_move)
    {
        int alpha = INT32_MIN + 1;
        int beta = INT32_MAX;

        auto save_alpha = alpha;
        bool foundpv = false;

        auto actual_vect = std::vector<chessBoard::Move>();
        int best = INT32_MIN + 1;
        const auto &inv_color = b.other_color(b.color);
        const auto &colo_act = b.color;

        //test if the board exist in the hash map and if depth left == depth stocked
        const auto &mov = b.generate_moves(colo_act);
        chessBoard::Move best_move = mov[0];
        //auto moves = helpers::remove_move_repetition(mov, b, ai::meta.vectBoard, hash);
        /*if (moves.empty())
            return INT32_MIN + 1;*/
        auto sorted_moves = ordering::moves_set_values(mov, std::nullopt, depth, hash);
        output_vect.push_back(sorted_moves[0].second);
        int score = 0;
        for (const auto &move : sorted_moves)
        {
            auto cpy = b;
            const uint64 &next_hash = cpy.apply_move(move.second, colo_act, hash);
            ai::meta.treefold.push(hash);
            if (foundpv)
            {
                score = -alphabeta(cpy, inv_color, depth - 1, -alpha - 1, -alpha, move.second, actual_vect,
                                   next_hash);
                if ((score > alpha) && (score < beta))
                {
                    score = -alphabeta(cpy, inv_color, depth - 1, -beta, -alpha, move.second, actual_vect,
                                       next_hash);
                }
            }
            else
            {
                score = -alphabeta(cpy, inv_color, depth - 1, -beta, -alpha, move.second, actual_vect,
                                   next_hash);
            }
            ai::meta.treefold.pop();
            if (score >= beta)
            {
                output_vect[0] = move.second;
                refutation_table::merge_vect(output_vect, actual_vect);
                transposition_table::tt_search.update(output_vect[0], score, depth, hash, -1);
                if (score == INT32_MAX)
                    winning_move = true;
                return move.second;
            }
            if (score > best)
            {
                best = score;
                best_move = move.second;
                refutation_table::merge_vect(output_vect, actual_vect);
                output_vect[0] = move.second;
                if (score > alpha)
                {
                    alpha = score;
                    foundpv = true;
                }
            }
            actual_vect.resize(0);
        }
        /*if (best <= save_alpha)
        {
            transposition_table::tt_search.update(output_vect[0], best, depth, hash, 1);
        }
        else if (best >= beta)
        {
            transposition_table::tt_search.update(output_vect[0], best, depth, hash, -1);
        }
        else
        {
            transposition_table::tt_search.update(output_vect[0], best, depth, hash, 0);
        }*/
        if (save_alpha < alpha && alpha < beta)
        {
            transposition_table::tt_search.update(output_vect[0], best, depth, hash, 0);
        }
        else if (alpha <= save_alpha)
        {
            transposition_table::tt_search.update(output_vect[0], best, depth, hash, 1);
        }
        if (score == INT32_MAX)
            winning_move = true;
        return best_move;
    }


    int alphabeta(const chessBoard::Board& b, const chessBoard::enumPiece &colo_act,
                  int depth, int alpha, int beta, const chessBoard::Move &prev_move,
                  chessBoard::MOVES_T &prev_vect_move,
                  uint64 hash)
    {

        if (b.half_move_count_ >= 100)
            return 0;
        if (b.half_move_count_ >= 2 && meta.treefold.is_treefold(hash))
        {
            return 0;
        }
        // Out of time
        if (!meta.running)
            return 0;
        //test if the board exist in the hash map and if depth left == depth stocked
        auto save_alpha = alpha;
        auto transpo = transposition_table::tt_search.find(hash);
        if (transpo.depth_ != -1 && transpo.hash_ == hash && transpo.depth_ >= depth)
        {
            if (transpo.is_cut_off_ == 0)
                return transpo.score_;
            else if (transpo.is_cut_off_ == -1)
            {
                alpha = std::max(alpha, transpo.score_);
            }
            else
            {
                beta = std::min(beta, transpo.score_);
            }
            if (alpha >= beta)
                return transpo.score_;
        }

        if (depth == 0)
        {
            return quiesce(b, colo_act, alpha, beta, prev_move, hash);
        }

        bool foundpv = false;

        auto actual_vect = chessBoard::MOVES_T();
        int best = INT32_MIN + 1;
        const auto &moves = b.generate_moves(colo_act);
        if (moves.empty())
        {
            if (!b.player_is_check(colo_act))
                return 0;
            return best;
        }
        const auto &sorted_moves = ordering::moves_set_values(moves, prev_move, depth, hash);//give hash
        const auto &inv_color = b.other_color(colo_act);
        prev_vect_move.push_back(sorted_moves[0].second);
        int score = 0;
        for (const auto &move : sorted_moves)
        {

            //auto cpy = helpers::copy_board(b);
            auto cpy = b;
            uint64 next_hash = cpy.apply_move(move.second, colo_act, hash);
            ai::meta.treefold.push(hash);
            if (foundpv)
            {
                score = -alphabeta(cpy, inv_color, depth - 1, -alpha - 1, -alpha, move.second, actual_vect,
                                   next_hash);
                if ((score > alpha) && (score < beta))
                {
                    score = -alphabeta(cpy, inv_color, depth - 1, -beta, -alpha, move.second, actual_vect,
                                       next_hash);
                }
            }
            else
            {
                score = -alphabeta(cpy, inv_color, depth - 1, -beta, -alpha, move.second, actual_vect,
                                   next_hash);
            }
            ai::meta.treefold.pop();
            //b.revert_move(move.second, colo_act);
            if (score >= beta)
            {
                refutation_table::merge_vect(prev_vect_move, actual_vect);
                prev_vect_move[0] = move.second;
                transposition_table::tt_search.update(move.second, score, depth, hash, -1);
                return score;
            }
            if (score > best)
            {
                refutation_table::merge_vect(prev_vect_move, actual_vect);
                prev_vect_move[0] = move.second;
                best = score;
                if (score > alpha)
                {
                    foundpv = true;
                    alpha = score;
                }
            }
            actual_vect.resize(0);
        }
        /*if (best <= save_alpha)
        {
            transposition_table::tt_search.update(prev_vect_move[0], best, depth, hash, 1);
        }
        else if (best >= beta)
        {
            transposition_table::tt_search.update(prev_vect_move[0], best, depth, hash, -1);
        }
        else
        {
            transposition_table::tt_search.update(prev_vect_move[0], best, depth, hash, 0);
        }*/
        if (save_alpha < alpha && alpha < beta)
        {
            transposition_table::tt_search.update(prev_vect_move[0], best, depth, hash, 0);
        }
        else if (alpha <= save_alpha)
        {
            transposition_table::tt_search.update(prev_vect_move[0], best, depth, hash, 1);
        }
        return best;
    }


}
