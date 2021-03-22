#pragma once

#include <map>
#include "position.hh"
#include "chessBoard.hh"

namespace parser
{
    yaka::File to_file(char symbol);
    yaka::Rank to_rank(char symbol);
    chessBoard::INDEX_T position_to_index(const yaka::Position &position);
}



