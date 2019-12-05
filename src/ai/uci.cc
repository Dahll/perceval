#include "uci.hh"
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

namespace uci
{
    std::vector<string> split(const string &s, char d) {
        std::vector<string> v;
        std::stringstream ss(s);
        string item;
        while (getline(ss, item, d)) {
            v.push_back(item);
        }
        return v;
    }

    std::string get_input(const std::string& expected)
    {
        // Get a command following the expected globbing
        // Ignoring all unrecognized commands
        std::string buffer;
        do
        {
            std::getline(std::cin, buffer);
            /*if ("quit" == buffer || "stopuci" == buffer)
                exit(0);*/
        } while (fnmatch(expected.c_str(), buffer.c_str(), 0));
        return buffer;
    }

    void play_move(const std::string& move)
    {
        // Send the computed move
        std::cout << "bestmove " << move << std::endl;
    }

    std::string pop_fen(std::vector<std::string>& input)
    {
        std::string ret = "";
        ret += input[0];
        input.erase(input.begin());
        ret += " ";
        ret += input[0];
        input.erase(input.begin());
        ret += " ";
        ret += input[0];
        input.erase(input.begin());
        ret += " ";
        ret += input[0];
        input.erase(input.begin());
        ret += " ";
        if (input.size() > 1)
        {
            ret += input[0];
            input.erase(input.begin());
            ret += " ";
            ret += input[0];
            input.erase(input.begin());
            ret += " ";
        }
        return ret;
    }

    chessBoard::Move string_to_move(std::string& s)
    {
        auto rf = pgn_parser::to_file(s[0]);
        auto ff = pgn_parser::to_rank(s[1]);
        auto posf = yaka::Position(rf, ff);
        chessBoard::INDEX_T indexf = position_to_index(posf);
        auto rt = pgn_parser::to_file(s[2]);
        auto ft = pgn_parser::to_rank(s[3]);
        auto post = yaka::Position(rt, ft);
        chessBoard::INDEX_T indext = position_to_index(post);
        std::optional<chessBoard::enumPiece> enup = std::nullopt;
        if (s[4] != '\0')
        {
            if (s[4] == 'q')
            {
                enup = chessBoard::nQueen;
            }
            else if (s[4] == 'r')
            {
                enup = chessBoard::nRook;
            }
            else if (s[4] == 'b')
            {
                enup = chessBoard::nBishop;
            }
            else if (s[4] == 'n')
            {
                enup = chessBoard::nKnight;
            }
        }
        return chessBoard::Move(indexf, indext, chessBoard::nWhite, std::nullopt, enup, false, false, 0);
    }

    void set_position(std::vector<std::string>& input)
    {
        ai::meta.treefold.reset();
        ai::meta.vectBoard.clear();
        uint64 hash = 0;
        if (input[0] == "fen")
        {
            input.erase(input.begin());
            std::string fen = pop_fen(input);
            ai::meta.boardM = Perft::parse(fen);
            hash = ai::helpers::zobrist(ai::meta.boardM);
            if (input[0] == "moves")
            {
                input.erase(input.begin());
                hash = ai::helpers::apply_all_moves(input, ai::meta.boardM, ai::meta.boardM.color, ai::meta.vectBoard);
            }
        }
        else
        {
            input.erase(input.begin());
            ai::meta.boardM = chessBoard::Board();
            hash = ai::helpers::zobrist(ai::meta.boardM);
            if (!input.empty())
            {
                input.erase(input.begin());
                hash = ai::helpers::apply_all_moves(input, ai::meta.boardM, chessBoard::nWhite, ai::meta.vectBoard);
            }
        }
        ai::meta.hash = hash;
        ai::meta.treefold.set_root_index();
    }

    void set_option_go(std::vector<std::string>& input)
    {
        while (!input.empty())
        {
            if (input[0] == "infinite")
            {
                /* TODO */
                break;
            }
            else if (input[0] == "wtime")
            {
                input.erase(input.begin());
                ai::meta.wtime = std::stoi(input[0]);
            }
            else if (input[0] == "btime")
            {
                input.erase(input.begin());
                ai::meta.btime = std::stoi(input[0]);
            }
            else if (input[0] == "winc")
            {
                input.erase(input.begin());
                ai::meta.winc = std::stoi(input[0]);
            }
            else if (input[0] == "binc")
            {
                input.erase(input.begin());
                ai::meta.binc = std::stoi(input[0]);

            }
            input.erase(input.begin());
        }
    }


    void loop()
    {
        while (true)
        {
            auto input = split(get_input(), ' ');
            if (input[0] == "uci")
            {
                std::cout << "id name " << ai::meta.my_name << '\n';
                std::cout << "id author " << ai::meta.my_name << '\n';
                std::cout << "uciok" << std::endl;
            }
            else if (input[0] == "quit" || input[0] == "stopuci")
            {
                break;
            }
            else if (input[0] == "isready")
            {
                std::cout << "readyok" << std::endl;
            }
            else if (input[0] == "position")
            {
                input.erase(input.begin());
                set_position(input);
            }
            else if (input[0] == "go")
            {
                input.erase(input.begin());
                set_option_go(input);
                ai::meta.set_time_to_play();
                ai::meta.running = true;
                std::thread main(ai::search::iterative_deepening);
                std::this_thread::sleep_for(std::chrono::milliseconds(ai::meta.time_to_play));
                ai::meta.running = false;
                main.join();
                play_move(ai::meta.best_move.to_str());
            }
            else if (input[0] == "stop")
            {
                if (ai::meta.running)
                {
                    ai::meta.running = false;
                    /* join the thread */
                    /* return move */
                }

            }
        }
    }

} // namespace ai