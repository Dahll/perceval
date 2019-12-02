//
// Created by adrien on 25/10/19.
//

#include "search.hh"

using namespace ai::refutation_table;

namespace ai::search
{
    void iterative_deepening() // max_time in milliseconds
    {
        //Initialisation des variables

        int i = 1;

        const auto& start = system_clock::now();

        transposition_table::tt_search.increment_age();

        /* Setup input_vect */
        input_vect = std::vector<chessBoard::Move>();
        input_vect.insert(input_vect.begin(), chessBoard::Move());
        input_vect.insert(input_vect.begin(), chessBoard::Move());

        auto output_vect = std::vector<chessBoard::Move>();

        auto move = chessBoard::Move();
        bool winning_move = false;
        while (true)
        {
            auto tmp_move = caller_alphabeta(meta.boardM, i, output_vect, meta.hash, winning_move);
            if (!ai::meta.running)
                break;
            move = tmp_move;
            /* Update input vect */
            input_vect = output_vect;
            ai::helpers::swap_vector_values(input_vect);
            input_vect.insert(input_vect.begin(), chessBoard::Move());
            input_vect.insert(input_vect.begin(), chessBoard::Move());

            /* Reset output_vect */
            output_vect.resize(0);

            /// This prints the time spend for this depth
            std::cout << duration_cast<milliseconds>(system_clock::now()-start).count() << std::endl;
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