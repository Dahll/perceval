//
// Created by adrien on 16/05/19.
//

#pragma once

#include "board.hh"
#include "piece-type.hh"


namespace rules
{
    std::vector<chessBoard::Move> get_moves(chessBoard::enumPiece piece,
            uint64_t pos, chessBoard::Board board);
    bool is_valid_promotion();
}
