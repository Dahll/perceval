#pragma once

#include "board.hh"
#include <string>

namespace ai
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
     * - position fen rnbqkbnr/ppppp pp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
     * - position fen rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1
     * moves f7f5
     */
    std::string get_board();
    std::string next_token(std::string& s);
    uint64 apply_all_moves(std::string& s, chessBoard::Board& board, chessBoard::enumPiece color_, std::vector<std::pair<uint64, int>>& vecBoard);
    chessBoard::Move string_to_move(std::string& s);
    std::string pop_fen(std::string& s);
} // namespace ai
