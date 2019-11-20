//
// Created by adrien on 25/10/19.
//

#include "search.hh"

namespace ai::search
{
    std::optional<chessBoard::Move> iterative_deepening(int max_time) // max_time in milliseconds
    {
        //Initialisation des variables
        max_time += 1;
        int i = 1;
        //const int& max_depth = val_max_depth();
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
            if (winning_move)
            {
                return move;
            }
            time_management::act_start = system_clock::now();
            /// This prints the time spend for this depth
            std::cout << duration_cast<milliseconds>(time_management::act_start-start).count() << std::endl;
            ++i;
        }
        return move;
    }

}