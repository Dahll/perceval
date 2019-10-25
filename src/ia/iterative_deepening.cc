//
// Created by adrien on 25/10/19.
//

#include "ia.hh"

namespace ai
{
    std::optional<chessBoard::Move> IA::iterative_deepening(int max_time , uint64 hash) // max_time in milliseconds
    {
        //Initialisation des variables
        max_time += 1;
        int i = 1;
        //const int& max_depth = val_max_depth();
        const auto& start = system_clock::now();
        transposition_table = new std::unordered_map<uint64 , Data>();
        transposition_table_quiescence = new std::unordered_map<uint64 , Data>();

        input_vect = std::vector<chessBoard::Move>();
        auto output_vect = std::vector<chessBoard::Move>();
        act_start = system_clock::now();
        auto output_vect_quiescence = std::vector<chessBoard::Move>();
        input_vect_quiescence = std::vector<chessBoard::Move>();
        auto move = 0;

        while ((std::chrono::duration_cast<std::chrono::milliseconds>(act_start-start).count() * 6 < max_time ))
        {

            start_depth = i;
            move = caller_alphabeta(i, output_vect, output_vect_quiescence, hash);
            input_vect = output_vect;
            input_vect_quiescence = output_vect_quiescence;
            output_vect_quiescence.resize(0);
            output_vect.resize(0);
            if (move == INT32_MAX)
            {
                delete(transposition_table_quiescence);
                delete(transposition_table);
                return input_vect[0];
            }
            act_start = system_clock::now();
            /// This prints the time spend for this depth
            std::cout << duration_cast<milliseconds>(act_start-start).count() << std::endl;
            ++i;
        }
        delete(transposition_table);
        delete(transposition_table_quiescence);
        return input_vect[0];
    }

}