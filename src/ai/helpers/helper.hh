//
// Created by adrien on 25/10/19.
//

#include "stock.hh"
#include "convertion.hh"
#include "board.hh"

#ifndef FOUCHE_R_CHESS_HELPER_HH
#define FOUCHE_R_CHESS_HELPER_HH


namespace ai::helpers
{

    bool board_is_false(const std::vector<chessBoard::Board>& vecBoard, const std::vector<int>& vecint, const chessBoard::Board& act_board);
    chessBoard::Board copy_board(const chessBoard::Board& b);
    bool vec_board_update(std::vector<chessBoard::Board>& vecBoard, std::vector<int>& vecint, const chessBoard::Board& act_board);
    bool cmp_boards(const chessBoard::Board& b, const chessBoard::Board& n);
    uint64 zobrist(const chessBoard::Board& b);
    uint64 apply_all_moves(std::vector<std::string>& input, chessBoard::Board& board, chessBoard::enumPiece color_, std::vector<std::pair<uint64, int>>& vecBoard);
    bool threefold(std::vector<std::pair<uint64, int>>& vect, const uint64& hash, int move_count,  int& index);
    bool threefoldd2(std::vector<std::pair<uint64, int>>& vect, const uint64& hash, int half_move_count, int& index);
    std::vector<chessBoard::Move> remove_move_repetition(const std::vector<chessBoard::Move>& vec, chessBoard::Board& board, std::vector<std::pair<uint64, int>>& vec_board, uint64 hash);
}


#endif //FOUCHE_R_CHESS_HELPER_HH
