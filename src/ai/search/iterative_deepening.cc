//
// Created by adrien on 25/10/19.
//

#include "search.hh"

namespace ai::search
{
    void iterative_deepening() // max_time in milliseconds
    {
        //Initialisation des variables

        int i = 1;

        int max_time = ai::time_management::give_time();

        max_time += 1;

        const auto& start = system_clock::now();

        transposition_table::tt_search.increment_age();

        refutation_table::input_vect = std::vector<chessBoard::Move>();
        auto output_vect = std::vector<chessBoard::Move>();
        time_management::act_start = system_clock::now();
        auto move = chessBoard::Move();
        bool winning_move = false;
        while ((std::chrono::duration_cast<std::chrono::milliseconds>(time_management::act_start-start).count() * 6 < max_time ))
        {
            time_management::start_depth = i;
            move = caller_alphabeta(i, output_vect, meta.hash, winning_move);
            refutation_table::input_vect = output_vect;
            output_vect.resize(0);

            time_management::act_start = system_clock::now();

            /// This prints the time spend for this depth
            std::cout << duration_cast<milliseconds>(time_management::act_start-start).count() << std::endl;
            if (winning_move)
            {
                break;
            }
            ++i;
        }

        auto time = duration_cast<milliseconds>(system_clock::now()-start).count();

        if (chessBoard::boardM.color == chessBoard::nWhite)
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