//
// Created by adrien on 30/05/19.
//
#include "search.hh"

namespace ai::search
{
    chessBoard::Move caller_alphabeta(int depth,
                         chessBoard::MOVES_T &output_vect,
                         uint64 hash,
                         bool& winning_move)
    {
        int alpha = INT32_MIN + 1;
        int beta = INT32_MAX;
        chessBoard::Move best_move = chessBoard::Move();
        auto save_alpha = alpha;
        auto transpo = transposition_table::tt_search.find(hash);
        if (transpo.depth_ != -1 && transpo.hash_ == hash && transpo.depth_ >= depth)
        {
            if (transpo.is_cut_off_ == 0)
            {
                if (transpo.score_ == INT32_MAX)
                    winning_move = true;
                return transpo.move_;
            }
            else if (transpo.is_cut_off_ == -1)
            {
                alpha = std::max(alpha, transpo.score_);
            }
            else
            {
                beta = std::min(beta, transpo.score_);
            }
            if (alpha >= beta)
            {
                if (transpo.score_ == INT32_MAX)
                    winning_move = true;
                return transpo.move_;
            }
        }

        auto actual_vect = std::vector<chessBoard::Move>();
        int best = INT32_MIN + 1;
        const auto &inv_color = chessBoard::boardM.other_color(chessBoard::boardM.color);
        const auto &colo_act = chessBoard::boardM.color;

        //test if the board exist in the hash map and if depth left == depth stocked
        const auto &mov = chessBoard::boardM.generate_moves(colo_act);
        auto moves = helpers::remove_move_repetition(mov, chessBoard::boardM, uci::vectBoard, hash);
        /*if (moves.empty())
            return INT32_MIN + 1;*/
        auto sorted_moves = ordering::moves_set_values(moves, std::nullopt, depth, hash);
        output_vect.push_back(sorted_moves[0].second);
        int score = 0;
        for (const auto &move : sorted_moves)
        {
            const uint64 &next_hash = chessBoard::boardM.apply_move(move.second, colo_act, hash);
            score = -alphabeta(inv_color, depth - 1, -beta, -alpha, move.second, actual_vect,
                               next_hash);
            chessBoard::boardM.revert_move(move.second, colo_act);
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
                }
            }
            actual_vect.resize(0);
        }
        if (best <= save_alpha)
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
        }
        if (score == INT32_MAX)
            winning_move = true;
        return best_move;
    }


    int alphabeta(const chessBoard::enumPiece &colo_act,
                  int depth, int alpha, int beta, const chessBoard::Move &prev_move,
                  chessBoard::MOVES_T &prev_vect_move,
                  uint64 hash)
    {

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
            return quiesce(colo_act, alpha, beta, prev_move, hash);
        }

        auto actual_vect = chessBoard::MOVES_T();
        int best = INT32_MIN + 1;
        const auto &moves = chessBoard::boardM.generate_moves(colo_act);
        if (moves.empty())
        {
            if (!chessBoard::boardM.player_is_check(colo_act))
                return 0;
            return best;
        }
        const auto &sorted_moves = ordering::moves_set_values(moves, prev_move, depth, hash);//give hash
        const auto &inv_color = chessBoard::boardM.other_color(colo_act);
        prev_vect_move.push_back(sorted_moves[0].second);
        for (const auto &move : sorted_moves)
        {
            uint64 next_hash = chessBoard::boardM.apply_move(move.second, colo_act, hash);
            int score = -alphabeta(inv_color, depth - 1, -beta, -alpha, move.second, actual_vect,
                                   next_hash);
            chessBoard::boardM.revert_move(move.second, colo_act);
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
                    alpha = score;
            }
            actual_vect.resize(0);
        }
        if (best <= save_alpha)
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
        }
        return best;
    }


}
