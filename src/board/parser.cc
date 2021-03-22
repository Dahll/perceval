#include "parser.hh"

namespace parser
{

    yaka::File to_file(char symbol)
    {
        return static_cast<yaka::File>(symbol - 'a');
    }

    yaka::Rank to_rank(char symbol)
    {
        return static_cast<yaka::Rank>(symbol - '1');
    }

    chessBoard::INDEX_T position_to_index(const yaka::Position &position)
    {
        static const std::map<const yaka::File, chessBoard::INDEX_T> file_to_index{
                {yaka::File::A, 7},
                {yaka::File::B, 6},
                {yaka::File::C, 5},
                {yaka::File::D, 4},
                {yaka::File::E, 3},
                {yaka::File::F, 2},
                {yaka::File::G, 1},
                {yaka::File::H, 0},
        };
        static const std::map<const yaka::Rank, chessBoard::INDEX_T> rank_to_index{
                {yaka::Rank::ONE,   0},
                {yaka::Rank::TWO,   1},
                {yaka::Rank::THREE, 2},
                {yaka::Rank::FOUR,  3},
                {yaka::Rank::FIVE,  4},
                {yaka::Rank::SIX,   5},
                {yaka::Rank::SEVEN, 6},
                {yaka::Rank::EIGHT, 7},
        };

        return file_to_index.at(position.file_get()) +
               rank_to_index.at(position.rank_get()) * 8;
    }
}