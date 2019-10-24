//
// Created by fouche_r on 6/6/19.
//

#include "ia.hh"
using namespace std::chrono;

#define TIME_PREV_FACTOR 7


#define SLOPE 1.6
#define Y_INTER -6.5
#define MIN_TIME_ITER 0
#define DEPTH_FACTOR 1
#define EXP_FACTOR 1
#define NB_PIECE_FACTOR 1
#define NB_CAPTURE_FACTOR 1
#define NB_MOVES_FACTOR 0.02
#define DEPTH_IGNORE 0
#define DEPTH_BASE -1

namespace ai
{
    /// Returns a boost factor between 0-100
    int IA::get_boost_factor(int nb_turns) const
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

    /* r2qkb1r/1pp2ppp/p1n5/3n1b2/3Pp3/N3P3/PP1BBPPP/R2QK1NR b KQkq - 1 9 */

    /* Result in millisecond */
    int IA::approx_next_time(const chessBoard::Board& cur_b, int depth) const
    {
        if (depth <= DEPTH_IGNORE)
            return 0;

        auto ret = exp((SLOPE * depth + Y_INTER) * EXP_FACTOR) * DEPTH_FACTOR
            * (0.001 + NB_CAPTURE_FACTOR * cur_b.get_current_captures_count())
            * NB_PIECE_FACTOR * cur_b.get_current_pieces_count()
            * NB_MOVES_FACTOR * cur_b.get_current_moves_count();

        return ret;

    }

    bool IA::approx_enough_time(const TimePoint& act_start_,
            const TimePoint& start, int max_time, size_t /* depth */) const
    {
        int time_prev/* millisecond */ = duration_cast<milliseconds>(act_start_ - start).count();

        //std::cout << "(max time: " << max_time << ") ";
        //std::cout << "guessed: " << time_prev * TIME_PREV_FACTOR << " with:" << std::endl;

        return time_prev * TIME_PREV_FACTOR < max_time;
        /*
        auto current_board = boardM.copy_board().apply_many_moves(input_vect);
        auto ret = approx_next_time(current_board, depth);
        std::cout << "(max time: " << max_time << ") ";
        std::cout << "guessed: " << ret << " with:" << std::endl;
        std::cout << "  depth: " << depth << std::endl;
        std::cout << "  nbcap: " << current_board.get_current_captures_count() << std::endl;
        std::cout << "  nbpic: " << current_board.get_current_pieces_count() << std::endl;
        std::cout << "         " << current_board.get_current_pieces_count() * NB_PIECE_FACTOR << std::endl;
        std::cout << "  nbmov: " << current_board.get_current_moves_count() << std::endl;
        std::cout << "         " << NB_MOVES_FACTOR * current_board.get_current_moves_count() << std::endl;
        std::cout << "  exp:   " << exp((depth - DEPTH_BASE) * EXP_FACTOR) << std::endl;
        std::cout << "  exp:   " << exp((depth - DEPTH_BASE) * EXP_FACTOR) * DEPTH_FACTOR << std::endl;
        return ret + time_prev < max_time;*/
    }
}
