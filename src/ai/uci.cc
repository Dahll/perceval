#include "uci.hh"


namespace uci
{

    const std::string my_name = "Perceval";

    std::vector<std::pair<uint64, int>> vectBoard = std::vector<std::pair<uint64, int>>{};


    void loop()
    {
        init(my_name);
        int max_time = 300;
        while (true)
        {
            std::string s = get_board();
            TimePoint act_time = std::chrono::system_clock::now();
            next_token(s);
            vectBoard.clear();
            uint64 hash = 0;
            if (s[0] == 'f' && s[1] == 'e' && s[2] == 'n')
            {
                next_token(s);
                std::string fen = pop_fen(s);
                chessBoard::boardM = Perft::parse(fen);
                if (s != "")
                {
                    next_token(s);
                    hash = ai::helpers::apply_all_moves(s, chessBoard::boardM, chessBoard::boardM.color, vectBoard);
                }
            }
            else
            {
                chessBoard::boardM = chessBoard::Board();
                next_token(s);
                if (next_token(s) != "") {
                    hash = ai::helpers::apply_all_moves(s, chessBoard::boardM, chessBoard::nWhite, vectBoard);
                }

            }
            const auto& time_to_play = ai::time_management::give_time(max_time);
            const auto& move = ai::search::iterative_deepening(time_to_play * 1000 /* millisecond */, hash);
            if (!move.has_value())
            {
                break;
            }
            max_time -= std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - act_time).count();
            play_move(move.value().to_str());

        }
    }

    namespace
    {
        std::string get_input(const std::string& expected = "*")
        {
            // Get a command following the expected globbing
            // Ignoring all unrecognized commands
            std::string buffer;
            do
            {
                std::getline(std::cin, buffer);
                if ("quit" == buffer || "stopuci" == buffer)
                    exit(0);
            } while (fnmatch(expected.c_str(), buffer.c_str(), 0));
            return buffer;
        }
    } // namespace

    void init(const std::string& name)
    {
        get_input("uci");
        std::cout << "id name " << name << '\n';
        std::cout << "id author " << name << '\n';
        std::cout << "uciok" << std::endl;
        get_input("isready");
        std::cout << "readyok" << std::endl;
    }

    void play_move(const std::string& move)
    {
        // Send the computed move
        std::cout << "bestmove " << move << std::endl;
    }

    std::string next_token(std::string& s)
    {
        std::string ret = "";
        while (s != "" && s.at(0) != ' ' && s.at(0) != '\0' && s.at(0) != '\n' && s.at(0) != '\t')
        {
            ret.push_back(s.at(0));
            s.erase(0, 1);
        }
        while (s != "" && (s.at(0) == ' ' || s.at(0) == '\t'))
        {
            s.erase(0, 1);
        }
        return ret;
    }

    std::string get_board()
    {
        auto board = get_input("position *"); // Get the board
        get_input("go *"); // Wait for a go from GUI
        return board;
    }







    std::string pop_fen(std::string& s)
    {
        std::string ret = "";
        ret += next_token(s);
        ret += " ";
        ret += next_token(s);
        ret += " ";
        ret += next_token(s);
        ret += " ";
        ret += next_token(s);
        ret += " ";
        ret += next_token(s);
        ret += " ";
        ret += next_token(s);
        ret += " ";

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





} // namespace ai