//
// Created by adrien on 30/05/19.
//

#include "ia.hh"
#define EARLY_GAME 0
namespace ai
{
    namespace
    {
        using chessBoard::enumPiece;
    }

    int IA::eval_king(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_) const
    {
        int ret = 0;
        int index = 0;
        uint64 mask_color = boardM.pieceBB[enumPiece::nKing] & boardM.pieceBB[color_];
        if ((index = chessBoard::split_index(mask_color)) != 0)
        {
            ret += KING;
            ret += chessBoard::board_king[color_][index];
        }
        uint64 mask_no_color = boardM.pieceBB[enumPiece::nKing] & boardM.pieceBB[not_color_];
        if ((index = chessBoard::split_index(mask_no_color)) != 0)
        {
            ret -= KING;
            ret -= chessBoard::board_king[not_color_][index];
        }
        return ret;
    }

    int IA::eval_queen(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_) const
    {
        int ret = 0;
        uint64 mask = boardM.pieceBB[enumPiece::nQueen] & boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += QUEEN;
            ret += chessBoard::board_queen[color_][index];
        }
        uint64 mask_no = boardM.pieceBB[enumPiece::nQueen] & boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= QUEEN;
            ret -= chessBoard::board_queen[not_color_][index];
        }
        return ret;
    }

    int IA::eval_rook(const chessBoard::enumPiece& color_,
            const chessBoard::enumPiece& not_color_) const
    {
        int ret = 0;
        uint64 mask = boardM.pieceBB[enumPiece::nRook] & boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += ROOKS;
            ret += chessBoard::board_rooks[color_][index];
        }
        uint64 mask_no = boardM.pieceBB[enumPiece::nRook] & boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= ROOKS;
            ret -= chessBoard::board_rooks[not_color_][index];
        }
        return ret;
    }

    int IA::eval_bishop(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_) const
    {
        int ret = 0;
        uint64 mask = boardM.pieceBB[enumPiece::nBishop] & boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += BISHOP;
            ret += chessBoard::board_bishop[color_][index];
        }
        uint64 mask_no = boardM.pieceBB[enumPiece::nBishop] & boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= BISHOP;
            ret -= chessBoard::board_bishop[not_color_][index];
        }
        return ret;
    }


    int IA::eval_knight(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_) const
    {
        int ret = 0;
        uint64 mask = boardM.pieceBB[enumPiece::nKnight] & boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += KNIGHT;
            ret += chessBoard::board_knight[color_][index];
        }
        uint64 mask_no = boardM.pieceBB[enumPiece::nKnight] & boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= KNIGHT;
            ret -= chessBoard::board_knight[not_color_][index];
        }
        return ret;
    }


    int IA::eval_pawn(const enumPiece& color_,
            const chessBoard::enumPiece& not_color_) const
    {
        int ret = 0;
        uint64 mask = boardM.pieceBB[enumPiece::nPawn] & boardM.pieceBB[color_];
        int index = 0;
        while ((index = chessBoard::split_index(mask)) != 0) {
            ret += PAWN;
            ret += chessBoard::board_pawn[color_][index];
        }
        uint64 mask_no = boardM.pieceBB[enumPiece::nPawn] & boardM.pieceBB[not_color_];
        while ((index = chessBoard::split_index(mask_no)) != 0) {
            ret -= PAWN;
            ret -= chessBoard::board_pawn[not_color_][index];
        }
        return ret;
    }

    int IA::evaluate(const chessBoard::enumPiece& color_) const
    {
        const chessBoard::enumPiece& not_color = boardM.other_color(color_);
        int ret = 0;
        ret += eval_queen(color_, not_color);
        ret += eval_rook(color_, not_color);
        ret += eval_bishop(color_, not_color);
        ret += eval_knight(color_, not_color);
        ret += eval_pawn(color_, not_color);
        ret += eval_king(color_, not_color);
        return ret;
    }

    int IA::test_mini_max() const
    {
        auto str = boardM.to_perft();
        if (boardM.color == chessBoard::nWhite)
        {
            const auto a = map_test_white.find(str);
            return a->second;
        }
        const auto a = map_test_black.find(str);
        return a->second;
    }

}