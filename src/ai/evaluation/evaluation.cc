//
// Created by adrien on 30/05/19.
//

#include "evaluation.hh"
namespace ai::evaluation
{
    namespace
    {
        using chessBoard::enumPiece;
    }

    int eval_king(const chessBoard::Board& b, const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        int index = 0;
        uint64 mask_color = b.pieceBB[enumPiece::nKing] & b.pieceBB[color_];
        if ((index = chessBoard::split_index(mask_color)) != 0)
        {
            ret += VAL_KING;
            ret += chessBoard::board_king[color_][index];
        }
        uint64 mask_no_color = b.pieceBB[enumPiece::nKing] & b.pieceBB[not_color_];
        if ((index = chessBoard::split_index(mask_no_color)) != 0)
        {
            ret -= VAL_KING;
            ret -= chessBoard::board_king[not_color_][index];
        }
        return ret;
    }

    int eval_queen(const chessBoard::Board& b, const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = b.pieceBB[enumPiece::nQueen] & b.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += VAL_QUEEN;
            ret += chessBoard::board_queen[color_][index];
        }
        uint64 mask_no = b.pieceBB[enumPiece::nQueen] & b.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= VAL_QUEEN;
            ret -= chessBoard::board_queen[not_color_][index];
        }
        return ret;
    }

    int eval_rook(const chessBoard::Board& b, const chessBoard::enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = b.pieceBB[enumPiece::nRook] & b.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += VAL_ROOKS;
            ret += chessBoard::board_rooks[color_][index];
        }
        uint64 mask_no = b.pieceBB[enumPiece::nRook] & b.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= VAL_ROOKS;
            ret -= chessBoard::board_rooks[not_color_][index];
        }
        return ret;
    }

    int eval_bishop(const chessBoard::Board& b, const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = b.pieceBB[enumPiece::nBishop] & b.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += VAL_BISHOP;
            ret += chessBoard::board_bishop[color_][index];
        }
        uint64 mask_no = b.pieceBB[enumPiece::nBishop] & b.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= VAL_BISHOP;
            ret -= chessBoard::board_bishop[not_color_][index];
        }
        return ret;
    }


    int eval_knight(const chessBoard::Board& b, const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = b.pieceBB[enumPiece::nKnight] & b.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += VAL_KNIGHT;
            ret += chessBoard::board_knight[color_][index];
        }
        uint64 mask_no = b.pieceBB[enumPiece::nKnight] & b.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= VAL_KNIGHT;
            ret -= chessBoard::board_knight[not_color_][index];
        }
        return ret;
    }


    int eval_pawn(const chessBoard::Board& b, const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = b.pieceBB[enumPiece::nPawn] & b.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += VAL_PAWN;
            ret += chessBoard::board_pawn[color_][index];
        }
        uint64 mask_no = b.pieceBB[enumPiece::nPawn] & b.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= VAL_PAWN;
            ret -= chessBoard::board_pawn[not_color_][index];
        }
        return ret;
    }

    int evaluate(const chessBoard::Board& b, const chessBoard::enumPiece& color_)
    {
        const chessBoard::enumPiece& not_color = b.other_color(color_);
        int ret = 0;
        ret += eval_queen(b, color_, not_color);
        ret += eval_rook(b, color_, not_color);
        ret += eval_bishop(b, color_, not_color);
        ret += eval_knight(b, color_, not_color);
        ret += eval_pawn(b, color_, not_color);
        ret += eval_king(b, color_, not_color);
        return ret;
    }

    int test_mini_max(const chessBoard::Board& b)
    {
        auto str = b.to_perft();
        if (b.color == chessBoard::nWhite)
        {
            const auto a = map_test_white.find(str);
            return a->second;
        }
        const auto a = map_test_black.find(str);
        return a->second;
    }

}
