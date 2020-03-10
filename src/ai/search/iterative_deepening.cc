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
        /*for (int i = 0; i < child.length; i++)
        {
            parent.pv[i+1] = child.pv[i];
        }*/
        memcpy(parent.pv + 1, child.pv, child.length * sizeof(Move));
        parent.length = child.length + 1;
    }

    void iterative_deepening() // max_time in milliseconds
    {
        //Initialisation des variables

        int i = 1;

        const auto& start = system_clock::now();
        std::cout << "TIME | DEPTH | PV" << std::endl;
        transposition_table::tt_search.increment_age();

        PV tmp = PV();

        auto move = chessBoard::Move();
        bool winning_move = false;
        while (true)
        {

            auto tmp_move = caller_alphabeta(meta.boardM, i, 0, tmp, meta.hash, winning_move);
            if (!ai::meta.running)
                break;
            move = tmp_move;

            mergePV(tmp, G_PV);

            std::cout << duration_cast<milliseconds>(system_clock::now()-start).count() << " | " << i << " | " << PV_to_str(G_PV) << std::endl;

            if (winning_move)
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
        ai::meta.best_move = move;
    }

}