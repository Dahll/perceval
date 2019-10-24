//
// Created by fouche_r on 5/18/19.
//

#pragma once

#include "board.hh"
namespace chessBoard
{
    using Piece = chessBoard::enumPiece;
    using side_piece_t = std::pair<Piece, Piece>;

    constexpr side_piece_t BlackKing = side_piece_t(Piece::nKing, Piece::nBlack);
    constexpr side_piece_t BlackQueen = side_piece_t(Piece::nQueen, Piece::nBlack);
    constexpr side_piece_t BlackRook = side_piece_t(Piece::nRook, Piece::nBlack);
    constexpr side_piece_t BlackBishop = side_piece_t(Piece::nBishop, Piece::nBlack);
    constexpr side_piece_t BlackKnight = side_piece_t(Piece::nKnight, Piece::nBlack);
    constexpr side_piece_t BlackPawn = side_piece_t(Piece::nPawn, Piece::nBlack);

    constexpr side_piece_t WhiteKing = side_piece_t(Piece::nKing, Piece::nWhite);
    constexpr side_piece_t WhiteQueen = side_piece_t(Piece::nQueen, Piece::nWhite);
    constexpr side_piece_t WhiteRook = side_piece_t(Piece::nRook, Piece::nWhite);
    constexpr side_piece_t WhiteBishop = side_piece_t(Piece::nBishop, Piece::nWhite);
    constexpr side_piece_t WhiteKnight = side_piece_t(Piece::nKnight, Piece::nWhite);
    constexpr side_piece_t WhitePawn = side_piece_t(Piece::nPawn, Piece::nWhite);
}