//
// Created by adrien on 20/11/19.
//

#include "metadata.hh"

namespace ai
{
    Metadata meta = Metadata();

    int get_boost_factor(int nb_turns)
    {
        if (nb_turns < 3)
            return nb_turns;
        else if (nb_turns <= 18)
            return nb_turns * 100 / 15;
        else if (nb_turns <= 40)
            return 140 + nb_turns * (-11) / 25;
        else if (nb_turns <= 60)
            return 132 + nb_turns * (-20) / 11;
        return 0;
    }


    /*void Metadata::set_time_to_play()
    {
        auto number_of_turn = chessBoard::boardM.turn_count_ * 2;
        if (chessBoard::boardM.color == chessBoard::nWhite)
            number_of_turn += 1;
        const auto& boost_factor = get_boost_factor(number_of_turn);
        int base_time = 0;
        int time_left = 0;
        if (chessBoard::boardM.color == chessBoard::nWhite)
            time_left = ai::meta.wtime;
        else
            time_left = ai::meta.btime;
        if (time_left < 120000)
        {
            time_to_play = 1;
        }
        else
        {
            base_time = BASE_TIME + (2 * boost_factor / 100);
            time_to_play = (base_time) * 1000;
        }
    }*/

    void Metadata::set_time_to_play()
    {
        /* Set factor*/
        int number_of_moves_play = vectBoard.size();
        int nMoves = 0;
        if (number_of_moves_play >= 12)
            number_of_moves_play -= 12;
        nMoves = std::min(10, number_of_moves_play);

        double factor = (double)2 - (double)nMoves / (double)10;

        /* Set target */

        int number_max_moves_left = 0;
        if (vectBoard.size() < 60)
            number_max_moves_left = 80 - vectBoard.size();
        else if (vectBoard.size() < 100)
            number_max_moves_left = 120 - vectBoard.size();
        else
            number_max_moves_left = vectBoard.size() + 30;

        int time_left = 0;
        if (meta.boardM.color == chessBoard::nWhite)
            time_left = meta.wtime;
        else
            time_left = meta.btime;

        double target = (double)time_left / (double)number_max_moves_left;

        /* Set normal_move_time */

        int normal_move_time = int(factor * target);

        /* Set increment */
        int increment = 0;
        if (meta.boardM.color == chessBoard::nWhite)
            increment = meta.winc;
        else
            increment = meta.binc;

        time_to_play = normal_move_time + increment;
        std::cout << "move_time :" << normal_move_time << std::endl;
        std::cout << "increment : " << increment << std::endl;
    }

    bool TreefoldStack::is_treefold(uint64 hash)
    {
        for (int i = stack_index-1; i >= 0; i--) {
            if (stack[i] == hash) {
                if (i <= root_index) {
                    for (int j = i-1; j >= 0; j--) {
                        if (stack[j] == hash) return true;
                    }
                }
                else return true;
            }
        }
        return false;
    }

    void TreefoldStack::push(uint64 hash)
    {
        stack[stack_index] = hash;
        stack_index += 1;
    }

    void TreefoldStack::pop()
    {
        stack_index -= 1;
    }

    void TreefoldStack::reset()
    {
        root_index = 0;
        stack_index = 0;
    }

    void TreefoldStack::set_root_index()
    {
        root_index = stack_index--;
    }
}