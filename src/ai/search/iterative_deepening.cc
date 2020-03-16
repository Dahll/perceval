//
// Created by adrien on 25/10/19.
//

#include "search.hh"
#include "string.h"

namespace ai::search
{

    PV G_PV = PV();

    std::string PV_to_str(const PV& pv)
    {
        std::string tmp = "";
        for (int i = 0; i < pv.length; i++)
        {
            tmp.append(pv.pv[i].to_str());
            tmp.append(" ");
        }
        return tmp;
    }

    void mergePV(PV& from, PV& to)
    {
        to.length = from.length;
        for (int i = 0; i < from.length; i++)
        {
            to.pv[i] = from.pv[i];
        }
    }

    void updatePV(Move best_move, PV& parent, PV& child)
    {
        parent.pv[0] = best_move;
        for (int i = 0; i < child.length; i++)
        {
            parent.pv[i+1] = child.pv[i];
        }
        //memcpy(parent.pv + 1, child.pv, child.length * sizeof(Move));
        parent.length = child.length + 1;
    }

    void iterative_deepening() // max_time in milliseconds
    {
        //Initialisation des variables

        int i = 1;

        const auto& start = system_clock::now();
        std::cout << "TIME | DEPTH | SCORE | PV" << std::endl;
        transposition_table::tt_search.increment_age();

        PV tmp = PV();

        auto move = chessBoard::Move();

        int alpha = -INF;
        int beta = INF;

        int score = -MAT;
        while (true)
        {
            auto tmp_move = caller_alphabeta(meta.boardM, i, tmp, meta.hash, alpha, beta, score);
            if (!ai::meta.running)
                break;

            /*if (i >= 6)
            {
                if ((score <= alpha) || (score >= beta))
                {
                    std::cout << "research from " << score << "\n";
                    alpha = -INF;
                    beta = INF;
                    continue;
                }
                alpha = score - 35;
                beta = score + 35;
            }*/
            move = tmp_move;

            mergePV(tmp, G_PV);

            std::cout << duration_cast<milliseconds>(system_clock::now()-start).count() << " | " << i << " | " << score << " | " << PV_to_str(G_PV) << std::endl;

            if (score >= MIN_MAT || score <= -MIN_MAT)
            {
                break;
            }
            ++i;
        }

        auto time = duration_cast<milliseconds>(system_clock::now()-start).count();

        if (meta.boardM.color == chessBoard::nWhite)
        {
            ai::meta.wtime -= time;
        }
        else
        {
            ai::meta.btime -= time;
        }
        transposition_table::tt_search.reset();
        ai::meta.best_move = move;
    }

}