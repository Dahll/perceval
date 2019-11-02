//
// Created by fouche_r on 6/6/19.
//

#include "time_management.hh"


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


#define TURN_CAP_EARLYGAME 7 /* 7 */
#define TURN_CAP_LATEGAME 10 /* 10 */
#define BASE_TIME 5 /* 5 */

namespace ai::time_management
{
    /// Returns a boost factor between 0-100

    int give_time(int time_left)
    {
        auto number_of_turn = ai::env::boardM.turn_count_ * 2;
        if (ai::env::boardM.color == chessBoard::nWhite)
            number_of_turn += 1;
        const auto& boost_factor = get_boost_factor(number_of_turn);
        int base_time = 0;
        if (time_left < 120)
        {
            return 1;
        }
        else
        {
            base_time = BASE_TIME + (2 * boost_factor / 100);
            return base_time;
        }
    }

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

    /* r2qkb1r/1pp2ppp/p1n5/3n1b2/3Pp3/N3P3/PP1BBPPP/R2QK1NR b KQkq - 1 9 */

    /* Result in millisecond */
    int approx_next_time(const chessBoard::Board& cur_b, int depth)
    {
        if (depth <= DEPTH_IGNORE)
            return 0;

        auto ret = exp((SLOPE * depth + Y_INTER) * EXP_FACTOR) * DEPTH_FACTOR
            * (0.001 + NB_CAPTURE_FACTOR * cur_b.get_current_captures_count())
            * NB_PIECE_FACTOR * cur_b.get_current_pieces_count()
            * NB_MOVES_FACTOR * cur_b.get_current_moves_count();

        return ret;

    }

    bool approx_enough_time(const TimePoint& act_start_,
            const TimePoint& start, int max_time, size_t /* depth */)
    {
        int time_prev/* millisecond */ = duration_cast<milliseconds>(act_start_ - start).count();
        return time_prev * TIME_PREV_FACTOR < max_time;
    }

    int val_max_depth()
    {
        uint64 piece = ai::env::boardM.pieceBB[3] | ai::env::boardM.pieceBB[4] | ai::env::boardM.pieceBB[5] | ai::env::boardM.pieceBB[6];
        int a = _popcnt64(piece);
        if (a <= 4)
        {
            return TURN_CAP_LATEGAME;
        }
        else
        {
            return TURN_CAP_EARLYGAME;
        }
    }
}
