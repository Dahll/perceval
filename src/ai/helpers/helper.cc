//
// Created by adrien on 25/10/19.
//

#include "../uci.hh"
#include "helper.hh"
using namespace ai;


namespace ai::helpers
{

    bool board_is_false(const std::vector<chessBoard::Board> &vecBoard, const std::vector<int> &vecint,
                        const chessBoard::Board &act_board)
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

    chessBoard::Board copy_board(const chessBoard::Board &b)
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
        n.turn_count_ = b.turn_count_;
        n.half_move_count_ = b.half_move_count_;
        return n;
    }

    bool vec_board_update(std::vector<chessBoard::Board> &vecBoard, std::vector<int> &vecint,
                          const chessBoard::Board &act_board)
    {
        int i = 0;
        for (auto &board: vecBoard)
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

    bool cmp_boards(const chessBoard::Board &b, const chessBoard::Board &n)
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

    uint64 zobrist(const chessBoard::Board& b)
    {
        using namespace chessBoard;
        uint64 hash_value = 0;
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

    uint64 apply_all_moves(std::vector<std::string>& input, chessBoard::Board& board, chessBoard::enumPiece color_, std::vector<std::pair<uint64, int>>& vecBoard)
    {
        uint64 hash = zobrist(board);
        std::string ret = "";
        int i =0;
        int pos = 0;
        vecBoard.emplace_back(hash, 1);
        while (pos < (int)input.size())
        {
            ret = input[pos];
            chessBoard::Move m = board.ia_apply_move(uci::string_to_move(ret), color_, hash);
            color_ = board.other_color(color_);
            board.color = color_;
            if (m.is_capture()) {
                vecBoard.clear();
                vecBoard.emplace_back(hash, 1);
            }
            else {
                threefold(vecBoard, hash, board.half_move_count_, i);
            }
            pos++;
        }
        return hash;
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

    std::vector<chessBoard::Move> remove_move_repetition(const std::vector<chessBoard::Move>& vec,
                                                         chessBoard::Board board, std::vector<std::pair<uint64, int>>& vec_board, uint64 hash)
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
    void swap_vector_values(std::vector<chessBoard::Move>& vect)
    {
        auto tmp = std::vector<chessBoard::Move>();
        for (auto& mov : vect)
        {
            tmp.insert(tmp.begin(), mov);
        }
        vect = tmp;
    }

}