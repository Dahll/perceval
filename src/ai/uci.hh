#pragma once

#include "search/search.hh"
#include <string>
#include <fnmatch.h>
#include <iostream>
#include <pgn-parser.hh>
#include <convertion.hh>
#include <fstream>
#include <ios>
#include <iostream>

namespace uci
{
    /** Initialize the communication with the GUI. This function should be
     * called only once at the begining of a game.
     * name: your ai name
     * Eg:
     * - "One AI To Rule Them All"
     */
    void init(const std::string& name);

    /** Send a move to GUI
     * move: String following EBNF
     * Eg:
     * - e2e4
     * - e1g1
     */
    void play_move(const std::string& move);

    /** Receive and return the command describing a board state
     * Format: position [startpos | fen FEN] (moves ...)
     * Eg:
     * - position startpos
     * - position startpos moves e2e4
     * - position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
     * - position fen rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1
     * moves f7f5
     */

    chessBoard::Move string_to_move(std::string& s);
    std::string pop_fen(std::vector<std::string>& input);
    std::string get_input(const std::string& expected = "*");
    void set_position(std::vector<std::string>& input);
    void set_option_go(std::vector<std::string>& input);


    void loop();

    extern const std::string my_name;
    extern std::vector<std::pair<uint64, int>> vectBoard;
}
