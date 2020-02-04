//
// Created by fouche_r on 5/20/19.
//

#pragma once

#include "chessBoard.hh"
#include "listener.hh"
#include "pgn-move.hh"
#include "../board/move.hh"
#include "chessboard-interface.hh"


chessBoard::enumPiece PieceType_to_enumPiece(const yaka::PieceType& p);
yaka::PieceType enumPiece_to_PieceType(const chessBoard::enumPiece& p);

yaka::Color enumPiece_to_color(const chessBoard::enumPiece& p);
chessBoard::enumPiece color_to_enumPiece(const yaka::Color& c);

yaka::ChessboardInterface::opt_piece_t
CBopt_to_CIopt(const chessBoard::opt_piece_t& opt);

chessBoard::INDEX_T position_to_index(const yaka::Position& position);
yaka::Position index_to_position(const chessBoard::INDEX_T& index);

chessBoard::Move PGNmove_to_CBmove(const yaka::PgnMove& PGNmove);

