//
// Created by fouche_r on 5/17/19.
//

#pragma once

#include "../board/board.hh"
//#include "../adapter/pgn-parser.hh"
#include "../board/parser.hh"
#include "chessBoard.hh"
#include <string>


namespace Perft
{
    using my64 = uint64;

    class Perft
    {
    public:
        explicit Perft(std::string fen);
        uint64 calculate(chessBoard::Board& b, int depth);
        void print_board() const;
        chessBoard::Board b_;
        int depth_;
    };

    uint64 perft(std::string& path);
    void update(chessBoard::Board& b, const std::string& s);
    chessBoard::Board parse(std::string& input);
    void set_color(chessBoard::Board& b, const std::string& s);
    void set_castlings(chessBoard::Board& b, const std::string& input);
    void print_board(chessBoard::Board& b);
    void print_bitboard(uint64 p);
}
