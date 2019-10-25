//
// Created by fouche_r on 5/22/19.
//
#include <strings.h>
#include <chessBoard.hh>
#include <strings.h>
#include <x86intrin.h>
#include <uci.hh>
#include "ia.hh"

#define TURN_CAP_EARLYGAME 7 /* 7 */
#define TURN_CAP_LATEGAME 10 /* 10 */
#define BASE_TIME 5 /* 5 */

namespace ai
{
    chessBoard::Board d_copy_board(const chessBoard::Board& b)
    {
        chessBoard::Board n;
        n.pieceBB[0] = b.pieceBB[0];
        n.pieceBB[1] = b.pieceBB[1];
        n.pieceBB[2] = b.pieceBB[2];
        n.pieceBB[3] = b.pieceBB[3];
        n.pieceBB[4] = b.pieceBB[4];
        n.pieceBB[5] = b.pieceBB[5];
        n.pieceBB[6] = b.pieceBB[6];
        n.pieceBB[7] = b.pieceBB[7];
        n.castlings = b.castlings;
        n.special_moves = b.special_moves;
        n.color = b.color;
        n.mat = n.mat;
        n.check = b.check;
        n.pat = b.pat;
        return n;
    }


    int IA::val_max_depth()
    {
        uint64 piece = boardM.pieceBB[3] | boardM.pieceBB[4] |boardM.pieceBB[5] |boardM.pieceBB[6];
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

    int IA::give_time(int time_left)
    {
        auto number_of_turn = boardM.turn_count_ * 2;
        if (boardM.color == chessBoard::nWhite)
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

    void IA::play_chess()
    {
        init(my_name);
        int max_time = 300;
   //     std::ifstream ifs;
        while (true)
        {
            std::string s = get_board();
            //std::ofstream aa("lol.txt", std::ios::app);
            //aa << s << '\n';
            TimePoint act_time = std::chrono::system_clock::now();
            next_token(s);
            vectBoard.clear();
            uint64 hash = 0;
            if (s[0] == 'f' && s[1] == 'e' && s[2] == 'n')
            {
                next_token(s);
                std::string fen = pop_fen(s);
                boardM = Perft::parse(fen);
                if (s != "")
                {
                    next_token(s);
                    hash = helpers::apply_all_moves(s, boardM, boardM.color, vectBoard);
                }
            }
            else
            {
                boardM = chessBoard::Board();
                next_token(s);
                if (next_token(s) != "") {
                    hash = helpers::apply_all_moves(s, boardM, chessBoard::nWhite, vectBoard);
                }

            }
            const auto& time_to_play = give_time(max_time);
            const auto& move = iterative_deepening(time_to_play * 1000 /* millisecond */, hash);
            if (!move.has_value())
            {
                break;
            }
            max_time -= std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - act_time).count();
            play_move(move.value().to_str());

        }
    }
}
