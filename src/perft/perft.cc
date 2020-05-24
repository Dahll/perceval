//
// Created by fouche_r on 5/17/19.
//

#include "perft.hh"
#include "../adapter/convertion.hh"
#include "stock.hh"
#include <fstream>
#include <strings.h>
#include <iostream>


using namespace chessBoard;
using namespace std;

namespace Perft
{

    Perft::Perft(std::string fen)
    {
        std::string s;
        int i = fen.length() - 1;
        while (fen.at(i) != ' ') {
            s.insert(0, 1, fen[i]);
            i--;
        }
        depth_ = stoi(s);
        b_ = parse(fen);
//        ofs.open("non");
    }

    bool cmp_boards(const Board& b, const Board& n)
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
        if (!(n.color == b.color))
            return false;
        if (n.mat != b.mat)
            return false;
        if (n.check != b.check)
            return false;
        return n.pat == b.pat;
    }

    uint64 Perft::calculate(chessBoard::Board& b, int depth)
    {
        if (!depth)
            return 1;
        uint64 n = 0;
        auto vect = VMove();
        b.generate_moves(b.color, vect);
        if (vect.len == 0)
            return 0;
        for (int j = 0; j < vect.len; j++)
        {
            const auto& m = vect.move_tab[j];
            uint64 h = 0;
            Board cpy = b;
            cpy.apply_move(m, b.color, h);
            cpy.switch_color();
            uint64 i = calculate(cpy, depth - 1);
            n += i;
        }
        return n;
    }

    void Perft::print_board() const
    {
        b_.print();
    }

    uint64 perft(std::string& path)
    {
        // FIXME
        std::string input;
        std::string s;
        std::ifstream ifs;
        ifs.open(path);
        if (!ifs.is_open())
            return -1;
        while (ifs >> input) {
            s.append(input);
            s.append(" ");
        }
        s.pop_back();
        ifs.close();
        Perft p{s};
        //p.print_board();
        //p.print_board();
        return p.calculate(p.b_, p.depth_);
        //Board b = parse(s);
        // std::cout << std::endl << "oui" << std::endl;
        // return ;
    }

    void update(Board& b, const std::string& s)
    {
        for (const auto& c : s)
        {
            /// if shift eight times already throw an exception, but it should not happen as we are testing
            ///  with valid inputs, the goal is to test the board/ai not this parser
            if (c >= '1' && c <= '8')
            {
                for (my64 & i : b.pieceBB)
                    i <<= (uint64)(c - '0');
                continue;
            }
            for (my64 & i : b.pieceBB)
                i <<= 1u;
            switch(c) {
                case 'p':
                    ++b.pieceBB[chessBoard::nPawn];
                    ++b.pieceBB[chessBoard::nBlack];
                    break;
                case 'P':
                    ++b.pieceBB[chessBoard::nPawn];
                    ++b.pieceBB[chessBoard::nWhite];
                    break;
                case 'k':
                    ++b.pieceBB[chessBoard::nKing];
                    ++b.pieceBB[chessBoard::nBlack];
                    break;
                case 'K':
                    ++b.pieceBB[chessBoard::nKing];
                    ++b.pieceBB[chessBoard::nWhite];
                    break;
                case 'q':
                    ++b.pieceBB[chessBoard::nQueen];
                    ++b.pieceBB[chessBoard::nBlack];
                    break;
                case 'Q':
                    ++b.pieceBB[chessBoard::nQueen];
                    ++b.pieceBB[chessBoard::nWhite];
                    break;
                case 'n':
                    ++b.pieceBB[chessBoard::nKnight];
                    ++b.pieceBB[chessBoard::nBlack];
                    break;
                case 'N':
                    ++b.pieceBB[chessBoard::nKnight];
                    ++b.pieceBB[chessBoard::nWhite];
                    break;
                case 'r':
                    ++b.pieceBB[chessBoard::nRook];
                    ++b.pieceBB[chessBoard::nBlack];
                    break;
                case 'R':
                    ++b.pieceBB[chessBoard::nRook];
                    ++b.pieceBB[chessBoard::nWhite];
                    break;
                case 'b':
                    ++b.pieceBB[chessBoard::nBishop];
                    ++b.pieceBB[chessBoard::nBlack];
                    break;
                case 'B':
                    ++b.pieceBB[chessBoard::nBishop];
                    ++b.pieceBB[chessBoard::nWhite];
                    break;
                    /// find another way
                default:
                    throw;
            }
        }
    }

    void set_color(Board& b, const string& s)
    {
        if (s[0] == 'b')
            b.color = chessBoard::nBlack;
    }

    void set_castlings(Board& b, const string& input)
    {
        string s = input.substr(0, input.find(' '));
        b.castlings = 0;
        if (s.size() == 1 && s[0] == '-') {
            return;
        }
        for (const auto& c : s)
        {
            switch(c)
            {
                case 'K':
                    b.castlings |= tab_pos[0];
                    break;
                case 'Q':
                    b.castlings |= tab_pos[7];
                    break;
                case 'k':
                    b.castlings |= tab_pos[56];
                    break;
                case 'q':
                    b.castlings |= tab_pos[63];
                    break;
                default:
                    return;
            }
        }
    }

    void set_en_passant(Board& b, const string& input)
    {
        if (input == "-") {
            return;
        }
        auto f = pgn_parser::to_file(input[0]);
        auto r = pgn_parser::to_rank(input[1]);
        auto pos = yaka::Position(f, r);
        INDEX_T index = position_to_index(pos);
        my64 p = index_to_uint64(index);
        b.special_moves = b.special_moves | p;
    }

    Board parse(std::string& input)
    {
        Board b = Board();
        /// fill the board
        while (input.find('/') != std::string::npos) {
            std::string s = input.substr(0, input.find('/'));
            update(b, s);
            input = input.substr(input.find('/') + 1);
        }
        std::string s = input.substr(0, input.find(' '));
        update(b, s);
        input = input.substr(input.find(' ') + 1);

        /// set color of the player
        s = input.substr(0, input.find(' '));
        set_color(b, s);
        input = input.substr(input.find(' ') + 1);
        /// set castlings if need (if not '-')
        s = input.substr(0, input.find(' '));
        set_castlings(b, s);
        input = input.substr(input.find(' ') + 1);

        /// set en passant if need (if not '-')
        s = input.substr(0, input.find(' '));
        set_en_passant(b, s);
        input = input.substr(input.find(' ') + 1);

        if (input.find(' ') != std::string::npos)
        {
            s = input.substr(0, input.find(' '));
            b.half_move_count_ = stoi(s);
            input = input.substr(input.find(' ') + 1);
        }

        if (input.find(' ') != std::string::npos)
        {
            s = input.substr(0, input.find(' '));
            b.turn_count_ = stoi(s);
            input = input.substr(input.find(' ') + 1);
        }

        return b;
    }


    void print_board(const chessBoard::Board& b)
    {
        char board[64];
        for (int i = nWhite; i <= nKing; ++i)
        {
//            print_bitboard(b.pieceBB[i]);
//            std::cout << std::endl;
        }
        for (char & i : board)
            i = '0';
        for (int i = nPawn; i <= nKing; ++i)
        {
            my64 minib = b.pieceBB[i];
            while (minib != 0) {
                int index = __builtin_ctzll(minib);
                uint64 pos = 1ull << index;
                minib &= minib - 1;
                if (pos & b.pieceBB[nWhite])
                    board[index] = epiece_to_char[i];
                else if (pos & b.pieceBB[nBlack])
                    board[index] = epiece_to_char[i] + 'a' - 'A';
            }
        }
        for (int i = 0; i < 64; i++)
        {
            if (i % 8 == 0)
                std::cout << std::endl;
            std::cout << board[63 - i];
        }
    }

    void print_bitboard(uint64 p)
    {
        std::cout << endl;
        std::cout << p;
        cout << endl;
        my64 board[64];
        for (POSITION_T & i : board)
            i = 0;
        uint64 k = 1;
        for (uint64 i = 0ull; i < 64ull; i++)
        {
            if ((p & (k << i))) {
                board[63 - i] = 1;
            }
        }
        for (my64 i = 0ull; i < 64ull; i++)
        {
            if (i % 8 == 0)
                std::cout << std::endl;
            std::cout << board[i];
        }
    }
}
