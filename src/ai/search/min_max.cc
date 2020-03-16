//
// Created by adrien on 30/05/19.
//
#include "search.hh"

namespace ai::search
{
    chessBoard::Move caller_alphabeta(const chessBoard::Board& b, int depth,
                         PV& parent_PV,
                         uint64 hash, int alpha, int beta,
                         int& best1)
    {
        auto save_alpha = alpha;
        bool foundpv = false;

        PV child_PV = PV();
        const auto &inv_color = b.other_color(b.color);
        const auto &colo_act = b.color;

        int best = -INF;


        const auto &mov = b.generate_moves(colo_act);

        chessBoard::Move best_move = Move();

        auto sorted_moves = ordering::moves_set_values(b, mov, std::nullopt, 0, hash);

        updatePV(sorted_moves[0].second, parent_PV, child_PV);

        int score = 0;

        for (const auto &move : sorted_moves)
        {

            auto cpy = b;
            const uint64 &next_hash = cpy.apply_move(move.second, colo_act, hash);
            ai::meta.treefold.push(hash);
            if (foundpv)
            {
                score = -alphabeta(cpy, inv_color, depth - 1, 1, -alpha - 1, -alpha, move.second, child_PV,
                                   next_hash, true);
                if ((score > alpha) && (score < beta))
                {
                    score = -alphabeta(cpy, inv_color, depth - 1, 1, -beta, -alpha, move.second, child_PV,
                                       next_hash, true);
                }
            }
            else
            {
                score = -alphabeta(cpy, inv_color, depth - 1, 1, -beta, -alpha, move.second, child_PV,
                                   next_hash, true);
            }
            ai::meta.treefold.pop();
            if (score >= beta)
            {
                best1 = score;
                updatePV(move.second, parent_PV, child_PV);
                transposition_table::tt_search.update(move.second, score, depth, hash, -1);
                return move.second;
            }
            if (score > best)
            {
                best = score;
                best_move = move.second;
                updatePV(move.second, parent_PV, child_PV);
                if (score > alpha)
                {
                    alpha = score;
                    foundpv = true;
                }
            }
            child_PV.length = 0;

            /*if (best < alpha)
            {
                best1 = best;
                return best_move;
            }*/
        }
        if (save_alpha < alpha && alpha < beta)
        {
            transposition_table::tt_search.update(best_move, best, depth, hash, 0);
        }
        else if (alpha <= save_alpha)
        {
            transposition_table::tt_search.update(best_move, best, depth, hash, 1);
        }
        best1 = best;
        return best_move;
    }


    int alphabeta(Board& b, const enumPiece &colo_act, int depth, int ply,
            int alpha, int beta, const Move &prev_move,PV& parent_PV, uint64 hash, bool null_move)
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

        if (b.player_is_check(colo_act))
            depth++;

        if (depth <= 0)
        {
            return quiesce(b, colo_act, alpha, beta, prev_move, hash);
        }

        bool isPV = (beta - alpha) != 1;

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


        const auto &inv_color = b.other_color(colo_act);
        PV child_PV = PV();


        int static_eval = evaluation::evaluate(b, colo_act);


        if (!isPV && !b.player_is_check(colo_act) && depth >= 3 &&
        /*__builtin_popcountll(b.pieceBB[colo_act]) > 3 && !b.is_only_pawn(colo_act) &&*/
        null_move && static_eval >= beta)
        {
            //int R = 2 + (32 * depth + std::min(static_eval - beta, 384)) / 128;
            int R = 2;
            if (depth > 6) R = 3;
            //auto tmp_board = b;
            //auto tmp_hash = hash;

            // Do null move
            auto tmp_special_move = b.special_moves;
            hash ^= position_value[ffsll(b.special_moves)];
            b.special_moves = 0;
            hash ^= position_value[ffsll(b.special_moves)];
            hash ^= side_to_move;


            int val = -alphabeta(b, inv_color, depth - 1 - R, ply + 1, -beta, -beta + 1, prev_move, child_PV,
                                 hash, false);

            // Undo null move
            hash ^= position_value[ffsll(b.special_moves)];
            b.special_moves = tmp_special_move;
            hash ^= position_value[ffsll(b.special_moves)];
            hash ^= side_to_move;


            if (val >= beta)
            {
                if (depth >= 10)
                {
                    val = alphabeta(b, inv_color, depth - 1 - R, ply + 1, alpha, beta, prev_move, child_PV, hash, false);
                    if (val >= beta)
                        return val;
                }
                else
                    return val;
            }
            child_PV.length = 0;
        }

        bool foundpv = false;

        //auto actual_vect = chessBoard::MOVES_T();

        int best = -MAT + ply;
        const auto &moves = b.generate_moves(colo_act);
        if (moves.empty())
        {
            if (!b.player_is_check(colo_act))
                return 0;
            return best;
        }



        //Null move prunning


        const auto &sorted_moves = ordering::moves_set_values(b, moves, prev_move, ply, hash);//give hash

        //updatePV(sorted_moves[0].second, parent_PV, child_PV);
        //prev_vect_move.push_back(sorted_moves[0].second);

        int score = 0;
        for (const auto &move : sorted_moves)
        {

            //auto cpy = helpers::copy_board(b);
            auto cpy = b;
            uint64 next_hash = cpy.apply_move(move.second, colo_act, hash);
            ai::meta.treefold.push(hash);
            if (foundpv)
            {
                score = -alphabeta(cpy, inv_color, depth - 1, ply + 1, -alpha - 1, -alpha, move.second, child_PV,
                                   next_hash, true);
                if ((score > alpha) && (score < beta))
                {
                    score = -alphabeta(cpy, inv_color, depth - 1, ply + 1, -beta, -alpha, move.second, child_PV,
                                       next_hash, true);
                }
            }
            else
            {
                score = -alphabeta(cpy, inv_color, depth - 1, ply + 1, -beta, -alpha, move.second, child_PV,
                                   next_hash, true);
            }
            ai::meta.treefold.pop();
            if (score >= beta)
            {
                //if (isPV)
                updatePV(move.second, parent_PV, child_PV);
                transposition_table::tt_search.update(move.second, score, depth, hash, -1);
                return score;
            }
            if (score > best)
            {
                best = score;
                if (score > alpha)
                {

                    if (isPV)
                    {
                        updatePV(move.second, parent_PV, child_PV);
                        alpha = score;
                    }
                }
            }
            foundpv = true;
            child_PV.length = 0;
        }

        if (save_alpha < alpha && alpha < beta)
        {
            transposition_table::tt_search.update(parent_PV.pv[0], best, depth, hash, 0);
        }
        else if (alpha <= save_alpha && isPV)
        {
            transposition_table::tt_search.update(parent_PV.pv[0], best, depth, hash, 1);
        }

        return best;
    }
}
