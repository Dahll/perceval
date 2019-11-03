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

    int eval_king(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        int index = 0;
        uint64 mask_color = chessBoard::boardM.pieceBB[enumPiece::nKing] & chessBoard::boardM.pieceBB[color_];
        if ((index = chessBoard::split_index(mask_color)) != 0)
        {
            ret += VAL_KING;
            ret += chessBoard::board_king[color_][index];
        }
        uint64 mask_no_color = chessBoard::boardM.pieceBB[enumPiece::nKing] & chessBoard::boardM.pieceBB[not_color_];
        if ((index = chessBoard::split_index(mask_no_color)) != 0)
        {
            ret -= VAL_KING;
            ret -= chessBoard::board_king[not_color_][index];
        }
        return ret;
    }

    int eval_queen(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = chessBoard::boardM.pieceBB[enumPiece::nQueen] & chessBoard::boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += VAL_QUEEN;
            ret += chessBoard::board_queen[color_][index];
        }
        uint64 mask_no = chessBoard::boardM.pieceBB[enumPiece::nQueen] & chessBoard::boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= VAL_QUEEN;
            ret -= chessBoard::board_queen[not_color_][index];
        }
        return ret;
    }

    int eval_rook(const chessBoard::enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = chessBoard::boardM.pieceBB[enumPiece::nRook] & chessBoard::boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += VAL_ROOKS;
            ret += chessBoard::board_rooks[color_][index];
        }
        uint64 mask_no = chessBoard::boardM.pieceBB[enumPiece::nRook] & chessBoard::boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= VAL_ROOKS;
            ret -= chessBoard::board_rooks[not_color_][index];
        }
        return ret;
    }

    int eval_bishop(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = chessBoard::boardM.pieceBB[enumPiece::nBishop] & chessBoard::boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += VAL_BISHOP;
            ret += chessBoard::board_bishop[color_][index];
        }
        uint64 mask_no = chessBoard::boardM.pieceBB[enumPiece::nBishop] & chessBoard::boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= VAL_BISHOP;
            ret -= chessBoard::board_bishop[not_color_][index];
        }
        return ret;
    }


    int eval_knight(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = chessBoard::boardM.pieceBB[enumPiece::nKnight] & chessBoard::boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += VAL_KNIGHT;
            ret += chessBoard::board_knight[color_][index];
        }
        uint64 mask_no = chessBoard::boardM.pieceBB[enumPiece::nKnight] & chessBoard::boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= VAL_KNIGHT;
            ret -= chessBoard::board_knight[not_color_][index];
        }
        return ret;
    }


    int eval_pawn(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = chessBoard::boardM.pieceBB[enumPiece::nPawn] & chessBoard::boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += VAL_PAWN;
            ret += chessBoard::board_pawn[color_][index];
        }
        uint64 mask_no = chessBoard::boardM.pieceBB[enumPiece::nPawn] & chessBoard::boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= VAL_PAWN;
            ret -= chessBoard::board_pawn[not_color_][index];
        }
        return ret;
    }

    int evaluate(const chessBoard::enumPiece& color_)
    {
        const chessBoard::enumPiece& not_color = chessBoard::boardM.other_color(color_);
        int ret = 0;
        ret += eval_queen(color_, not_color);
        ret += eval_rook(color_, not_color);
        ret += eval_bishop(color_, not_color);
        ret += eval_knight(color_, not_color);
        ret += eval_pawn(color_, not_color);
        ret += eval_king(color_, not_color);
        return ret;
    }

    int test_mini_max()
    {
        auto str = chessBoard::boardM.to_perft();
        if (chessBoard::boardM.color == chessBoard::nWhite)
        {
            const auto a = map_test_white.find(str);
            return a->second;
        }
        const auto a = map_test_black.find(str);
        return a->second;
    }

}
