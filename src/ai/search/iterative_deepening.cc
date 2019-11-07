//
// Created by adrien on 25/10/19.
//

#include "search.hh"

namespace ai::search
{
    std::optional<chessBoard::Move> iterative_deepening(int max_time , uint64 hash) // max_time in milliseconds
    {
        //Initialisation des variables
        max_time += 1;
        int i = 1;
        //const int& max_depth = val_max_depth();
        const auto& start = system_clock::now();
        transposition_table::tt_search.init();

        refutation_table::input_vect = std::vector<chessBoard::Move>();
        auto output_vect = std::vector<chessBoard::Move>();
        time_management::act_start = system_clock::now();
        auto output_vect_quiescence = std::vector<chessBoard::Move>();
        auto move = 0;

        while ((std::chrono::duration_cast<std::chrono::milliseconds>(time_management::act_start-start).count() * 6 < max_time ))
        {

            time_management::start_depth = i;
            move = caller_alphabeta(i, output_vect, output_vect_quiescence, hash);
            refutation_table::input_vect = output_vect;
            output_vect_quiescence.resize(0);
            output_vect.resize(0);
            if (move == INT32_MAX)
            {
                transposition_table::tt_search.clean();
                return refutation_table::input_vect[0];
            }
            time_management::act_start = system_clock::now();
            /// This prints the time spend for this depth
            std::cout << duration_cast<milliseconds>(time_management::act_start-start).count() << std::endl;
            ++i;
        }
        transposition_table::tt_search.clean();
        return refutation_table::input_vect[0];
    }

}