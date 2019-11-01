//
// Created by adrien on 25/10/19.
//

#include "ia.hh"
#include "ia_env.hh"

namespace ai
{
    std::optional<chessBoard::Move> IA::iterative_deepening(int max_time , uint64 hash) // max_time in milliseconds
    {
        //Initialisation des variables
        max_time += 1;
        int i = 1;
        //const int& max_depth = val_max_depth();
        const auto& start = system_clock::now();
        env::transposition_table = new std::unordered_map<uint64 , transposition_table::Data>();
        env::transposition_table_quiescence = new std::unordered_map<uint64 , transposition_table::Data>();

        env::input_vect = std::vector<chessBoard::Move>();
        auto output_vect = std::vector<chessBoard::Move>();
        env::act_start = system_clock::now();
        auto output_vect_quiescence = std::vector<chessBoard::Move>();
        env::input_vect_quiescence = std::vector<chessBoard::Move>();
        auto move = 0;

        while ((std::chrono::duration_cast<std::chrono::milliseconds>(env::act_start-start).count() * 6 < max_time ))
        {

            env::start_depth = i;
            move = caller_alphabeta(i, output_vect, output_vect_quiescence, hash);
            env::input_vect = output_vect;
            env::input_vect_quiescence = output_vect_quiescence;
            output_vect_quiescence.resize(0);
            output_vect.resize(0);
            if (move == INT32_MAX)
            {
                delete(env::transposition_table_quiescence);
                delete(env::transposition_table);
                return env::input_vect[0];
            }
            env::act_start = system_clock::now();
            /// This prints the time spend for this depth
            std::cout << duration_cast<milliseconds>(env::act_start-start).count() << std::endl;
            ++i;
        }
        delete(env::transposition_table);
        delete(env::transposition_table_quiescence);
        return env::input_vect[0];
    }

}