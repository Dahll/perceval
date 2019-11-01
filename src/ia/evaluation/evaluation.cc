//
// Created by adrien on 30/05/19.
//

#include <stock.hh>
#include "evaluation.hh"
#include "ia_env.hh"
#include "ia.hh"
#define EARLY_GAME 0
namespace evaluation
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
        uint64 mask_color = ai::env::boardM.pieceBB[enumPiece::nKing] & ai::env::boardM.pieceBB[color_];
        if ((index = chessBoard::split_index(mask_color)) != 0)
        {
            ret += KING;
            ret += chessBoard::board_king[color_][index];
        }
        uint64 mask_no_color = ai::env::boardM.pieceBB[enumPiece::nKing] & ai::env::boardM.pieceBB[not_color_];
        if ((index = chessBoard::split_index(mask_no_color)) != 0)
        {
            ret -= KING;
            ret -= chessBoard::board_king[not_color_][index];
        }
        return ret;
    }

    int eval_queen(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = ai::env::boardM.pieceBB[enumPiece::nQueen] & ai::env::boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += QUEEN;
            ret += chessBoard::board_queen[color_][index];
        }
        uint64 mask_no = ai::env::boardM.pieceBB[enumPiece::nQueen] & ai::env::boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= QUEEN;
            ret -= chessBoard::board_queen[not_color_][index];
        }
        return ret;
    }

    int eval_rook(const chessBoard::enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = ai::env::boardM.pieceBB[enumPiece::nRook] & ai::env::boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += ROOKS;
            ret += chessBoard::board_rooks[color_][index];
        }
        uint64 mask_no = ai::env::boardM.pieceBB[enumPiece::nRook] & ai::env::boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= ROOKS;
            ret -= chessBoard::board_rooks[not_color_][index];
        }
        return ret;
    }

    int eval_bishop(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = ai::env::boardM.pieceBB[enumPiece::nBishop] & ai::env::boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += BISHOP;
            ret += chessBoard::board_bishop[color_][index];
        }
        uint64 mask_no = ai::env::boardM.pieceBB[enumPiece::nBishop] & ai::env::boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= BISHOP;
            ret -= chessBoard::board_bishop[not_color_][index];
        }
        return ret;
    }


    int eval_knight(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = ai::env::boardM.pieceBB[enumPiece::nKnight] & ai::env::boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += KNIGHT;
            ret += chessBoard::board_knight[color_][index];
        }
        uint64 mask_no = ai::env::boardM.pieceBB[enumPiece::nKnight] & ai::env::boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= KNIGHT;
            ret -= chessBoard::board_knight[not_color_][index];
        }
        return ret;
    }


    int eval_pawn(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_)
    {
        int ret = 0;
        uint64 mask = ai::env::boardM.pieceBB[enumPiece::nPawn] & ai::env::boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += PAWN;
            ret += chessBoard::board_pawn[color_][index];
        }
        uint64 mask_no = ai::env::boardM.pieceBB[enumPiece::nPawn] & ai::env::boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= PAWN;
            ret -= chessBoard::board_pawn[not_color_][index];
        }
        return ret;
    }

    int evaluate(const chessBoard::enumPiece& color_)
    {
        const chessBoard::enumPiece& not_color = ai::env::boardM.other_color(color_);
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
        auto str = ai::env::boardM.to_perft();
        if (ai::env::boardM.color == chessBoard::nWhite)
        {
            const auto a = map_test_white.find(str);
            return a->second;
        }
        const auto a = map_test_black.find(str);
        return a->second;
    }

}
