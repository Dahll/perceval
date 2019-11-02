//
// Created by fouche_r on 5/22/19.
//
#include <strings.h>
#include <chessBoard.hh>
#include <strings.h>
#include <x86intrin.h>
#include "uci.hh"
#include "ia.hh"
#include "ia_env.hh"



namespace ai
{
    void play_chess()
    {
        init(env::my_name);
        int max_time = 300;
        while (true)
        {
            std::string s = get_board();
            TimePoint act_time = std::chrono::system_clock::now();
            next_token(s);
            env::vectBoard.clear();
            uint64 hash = 0;
            if (s[0] == 'f' && s[1] == 'e' && s[2] == 'n')
            {
                next_token(s);
                std::string fen = pop_fen(s);
                env::boardM = Perft::parse(fen);
                if (s != "")
                {
                    next_token(s);
                    hash = helpers::apply_all_moves(s, env::boardM, env::boardM.color, env::vectBoard);
                }
            }
            else
            {
                env::boardM = chessBoard::Board();
                next_token(s);
                if (next_token(s) != "") {
                    hash = helpers::apply_all_moves(s, env::boardM, chessBoard::nWhite, env::vectBoard);
                }

            }
            const auto& time_to_play = time_management::give_time(max_time);
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
