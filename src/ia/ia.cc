//
// Created by fouche_r on 5/22/19.
//
#include <strings.h>
#include <chessBoard.hh>
#include <strings.h>
#include <x86intrin.h>
#include "ia.hh"
#include "hash.hh"

#define TURN_CAP_EARLYGAME 7 /* 7 */
#define TURN_CAP_LATEGAME 10 /* 10 */
#define BASE_TIME 5 /* 5 */

namespace ai
{
    /*
     uint64 zobrist_apply(const chessBoard::Move& m, const chessBoard::enumPiece& color, uint64 board_hash)
    {
        // FIXME move this to apply_move and have apply_move return an uint64 instead of nothing
        // FIXME so en passant, captures and castlings can be handled easily
        if (color == chessBoard::nWhite)
        {
  //          uint64 new_board = board_hash ^ (position_value[ffsll(m.from_get()) + m.piece_get() * 65]);
        }
        else {
    //        uint64 new_board = board_hash ^(position_value[ffsll(m.from_get()) + m.piece_get() * 65]);
        }
        return 0;
    }
     */


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

    std::optional<chessBoard::Move> IA::iterative_deepening(int max_time /* milliseconds */, uint64 hash)
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

        while (/*(i <= max_depth) &&*/(std::chrono::duration_cast<std::chrono::milliseconds>(act_start-start).count() * 6 < max_time ))
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

    /*bool IA::find_opening_move(std::ifstream& ifs)
    {
        for (std::string s; std::getline(ifs, s);)
        {
            std::string current_board = boardM.to_perft();
            current_board = current_board.substr(0, current_board.length() - 5);
            std::string::size_type fen_limit = s.find_last_of(' ');
            if (current_board == s.substr(0, fen_limit))
            {
                std::string move = s.substr(fen_limit + 1, s.length());
                /// apply move
                boardM.ia_apply_move(string_to_move(move), boardM.color);
                play_move(move);
                return true;
            }
        }
        is_opening = false;
        return false;
    }*/

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
                    hash = apply_all_moves(s, boardM, boardM.color, vectBoard);
                }
            }
            else
            {
                boardM = chessBoard::Board();
                next_token(s);
                if (next_token(s) != "") {
                    hash = apply_all_moves(s, boardM, chessBoard::nWhite, vectBoard);
                }

            }
            //std::cout << boardM.to_perft();
            //auto move = best_move(-1);
            //auto move = caller_search(5);
    //        vectBoard = vectBoard1;
            //auto move = caller_alphabeta(IA_DEPTH);
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

    /*
    std::array<uint64, 911> position_value;
    for (int i = 0; i < 65 * 2; ++i)
        position_value[i] = 0;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<unsigned long long> dis(1, UINT64_MAX);
    for (int i = 0; i < 911; ++i)
    {
        position_value[i] = dis(gen);
        bool t = true;
        while(t)
        {
            int j = 0;
            for (j = 0; j < i; j++)
            {
                if (position_value[i] == position_value[j])
                {
                    position_value[i] = dis(gen);
                    break;
                }
            }
            if (j == i)
                t = false;
        }
    }
    position_value[0] = 0ull;
    position_value[65] = 0ull;
    std::cout << print_list_zobrist(position_value);
    }*/
    /// position value de 0 et de 65 = 0 ou tout explose
    /// verifier qu'il y a pas 2 fois la meme valeur ?
}
