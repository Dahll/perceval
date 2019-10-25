#include "uci.hh"
#include "stock.hh"
#include <fnmatch.h>
#include <iostream>
#include <pgn-parser.hh>
#include <convertion.hh>
#include <fstream>
#include <ios>
#include <iostream>

namespace ai
{


    chessBoard::Board deep_copy_board(const chessBoard::Board& b)
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

    bool test_boards(const chessBoard::Board& b, const chessBoard::Board& n)
    {
        if (n.pieceBB[0] != b.pieceBB[0])
            return false;
        if (n.pieceBB[1] != b.pieceBB[1])
            return false;
        if (n.pieceBB[2] != b.pieceBB[2])
            return false;
        if (n.pieceBB[3] != b.pieceBB[3])
            return false;
        if (n.pieceBB[4] != b.pieceBB[4])
            return false;
        if (n.pieceBB[5] != b.pieceBB[5])
            return false;
        if (n.pieceBB[6] != b.pieceBB[6])
            return false;
        if (n.pieceBB[7] != b.pieceBB[7])
            return false;
        if (n.castlings != b.castlings)
            return false;
        return n.special_moves == b.special_moves;
    }

    bool threefold(std::vector<std::pair<uint64, int>>& vect, const uint64& hash, int move_count,  int& index)
    {
        bool t = false;
        int i = 0;
        if (move_count >= 100)
            return true;
        for (auto& p : vect)
        {
            if (p.first == hash)
            {
                index = i;
                t = true;
                p.second++;
                if (p.second >= 3) {
                    return true;
                }
                break;
            }
            i++;
        }
        if (!t) {
            index = i;
            vect.emplace_back(hash, 1);
        }
        return false;
    }


    bool threefoldd2(std::vector<std::pair<uint64, int>>& vect, const uint64& hash, int half_move_count, int& index)
    {
        bool t = false;
        int i = 0;
        if (half_move_count >= 100)
            return true;
        for (auto& p : vect)
        {
            if (p.first == hash)
            {
                index = i;
                t = true;
                p.second++;
                if (p.second >= 3) {
                    return true;
                }
                break;
            }
            i++;
        }
        if (!t) {
            index = i;
        }
        return false;
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

    chessBoard::Board copy_board(const chessBoard::Board& b)
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

    bool cmp_boards(const chessBoard::Board& b, const chessBoard::Board& n)
    {
        if (n.pieceBB[0] != b.pieceBB[0])
            return false;
        if (n.pieceBB[1] != b.pieceBB[1])
            return false;
        if (n.pieceBB[2] != b.pieceBB[2])
            return false;
        if (n.pieceBB[3] != b.pieceBB[3])
            return false;
        if (n.pieceBB[4] != b.pieceBB[4])
            return false;
        if (n.pieceBB[5] != b.pieceBB[5])
            return false;
        if (n.pieceBB[6] != b.pieceBB[6])
            return false;
        if (n.pieceBB[7] != b.pieceBB[7])
            return false;
        if (n.castlings != b.castlings)
            return false;
        if (n.special_moves != b.special_moves)
            return false;
        return true;
    }

    bool board_is_false(const std::vector<chessBoard::Board>& vecBoard, const std::vector<int>& vecint, const chessBoard::Board& act_board)
    {
        int i = 0;
        int size = vecBoard.size();
        while (i < size)
        {
            if (cmp_boards(vecBoard[i], act_board) && vecint[i] >= 1)
            {
                return true;
            }
            i++;
        }
        return false;
    }

    bool vec_board_update(std::vector<chessBoard::Board>& vecBoard, std::vector<int>& vecint, const chessBoard::Board& act_board)
    {
        int i = 0;
        for (auto& board: vecBoard)
        {
            if (cmp_boards(board, act_board))
            {
                vecint[i] += 1;
                return true;
            }
            i++;
        }
        vecBoard.emplace_back(copy_board(act_board));
        vecint.emplace_back(1);
        return false;
    }

    uint64 zobrist(const chessBoard::Board& b)
    {
        using namespace chessBoard;
        uint64 hash_value = 0;
        //position_values[x]
        for (int i = chessBoard::nPawn; i <= chessBoard::nKing; ++i)
        {
            uint64 pos = b.pieceBB[i] & b.pieceBB[nWhite];
            while (int j = split_index(pos) != 0)
            {
                hash_value = hash_value ^ (position_value[ffsll(j) + i * 65]);
            }
            pos = b.pieceBB[i] & b.pieceBB[nBlack];
            while (int j = split_index(pos) != 0)
            {
                hash_value = hash_value ^ (position_value[ffsll(j) + (i + 6) * 65]);
            }
        }
        hash_value ^= side_to_move * (b.color == nBlack);
        uint64 castlings = b.castlings;
        while (int i = split_index(castlings))
        {
            hash_value ^= position_value[65 + ffsll(i)];
        }
        hash_value ^= position_value[ffsll(b.special_moves)];
        return hash_value;
    }

    uint64 apply_all_moves(std::string& s, chessBoard::Board& board, chessBoard::enumPiece color_, std::vector<std::pair<uint64, int>>& vecBoard)
    {
        uint64 hash = zobrist(board);
        std::string ret = "";
        int i =0;
        vecBoard.emplace_back(hash, 1);
        while ((ret = next_token(s)) != "")
        {
            chessBoard::Move m = board.ia_apply_move(string_to_move(ret), color_, hash);
            color_ = board.other_color(color_);
            board.color = color_;
            if (m.is_capture()) {
                vecBoard.clear();
                vecBoard.emplace_back(hash, 1);
            }
            else {
                threefold(vecBoard, hash, board.half_move_count_, i);
            }
            //      vec_board_update(vecBoard, vecint, board);
        }
        return hash;
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

    std::vector<chessBoard::Move> remove_move_repetition(const std::vector<chessBoard::Move>& vec,
            chessBoard::Board& board, std::vector<std::pair<uint64, int>>& vec_board, uint64 hash)
    {
        auto ret = std::vector<chessBoard::Move>();
        for (const auto& move : vec)
        {
            // FIXME change for zobrist here
            const uint64 h = board.apply_move(move, board.color, hash);
            int i = 0;
            if (!threefoldd2(vec_board, h, board.half_move_count_, i)) {
                ret.push_back(move);
            }
            board.revert_move(move, board.color);
        }
        if (ret.empty()) {
            return vec;
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





} // namespace ai