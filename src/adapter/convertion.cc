//
// Created by fouche_r on 5/20/19.
//

#include "convertion.hh"
#include "utype.hh"
#include <map>
#include <strings.h>

using namespace yaka;
using namespace chessBoard;

enumPiece PieceType_to_enumPiece(const PieceType& p)
{
    enumPiece index = int_to_enumPiece[piece_to_enumpiece_index(p)];
    return index;
}

PieceType enumPiece_to_PieceType(const enumPiece& p)
{
    return char_to_piece(epiece_to_char[p]);
}

yaka::Color enumPiece_to_color(const enumPiece& p)
{
    return char_to_color(epiece_to_char[p]);
}

chessBoard::enumPiece color_to_enumPiece(const yaka::Color& c)
{
    return utils::utype(c) ? enumPiece::nBlack : enumPiece::nWhite;
}

ChessboardInterface::opt_piece_t
CBopt_to_CIopt(const chessBoard::opt_piece_t &opt)
{
    if (!opt.has_value())
        return std::nullopt;
    auto &CB_piece_color = *opt;

    auto CI_piece_color = ChessboardInterface::side_piece_t(
            enumPiece_to_PieceType(CB_piece_color.first),
            enumPiece_to_color(CB_piece_color.second));

    return ChessboardInterface::opt_piece_t(CI_piece_color);
}

INDEX_T position_to_index(const Position &position)
{
    static const std::map<const File, INDEX_T> file_to_index{
            {File::A, 7},
            {File::B, 6},
            {File::C, 5},
            {File::D, 4},
            {File::E, 3},
            {File::F, 2},
            {File::G, 1},
            {File::H, 0},
    };
    static const std::map<const Rank, INDEX_T> rank_to_index{
            {Rank::ONE,   0},
            {Rank::TWO,   1},
            {Rank::THREE, 2},
            {Rank::FOUR,  3},
            {Rank::FIVE,  4},
            {Rank::SIX,   5},
            {Rank::SEVEN, 6},
            {Rank::EIGHT, 7},
    };

    return file_to_index.at(position.file_get()) + 1 +
           rank_to_index.at(position.rank_get()) * 8;
}

yaka::Position index_to_position(const chessBoard::INDEX_T& index)
{

    static const std::map<INDEX_T, const File> index_to_file{
            {7, File::A},
            {6, File::B},
            {5, File::C},
            {4, File::D},
            {3, File::E},
            {2, File::F},
            {1, File::G},
            {0, File::H},
    };
    static const std::map<INDEX_T, const Rank> index_to_rank{
            {0, Rank::ONE},
            {1, Rank::TWO},
            {2, Rank::THREE},
            {3, Rank::FOUR},
            {4, Rank::FIVE},
            {5, Rank::SIX},
            {6, Rank::SEVEN},
            {7, Rank::EIGHT},
    };

    return yaka::Position(
            index_to_file.at((index - 1) % 8),
            index_to_rank.at((index- 1) / 8));

}

Move PGNmove_to_CBmove(const PgnMove &PGNmove)
{
    const auto &pgnm_promotion = PGNmove.promotion_get();
    std::optional<enumPiece> enum_promotion = std::nullopt;
    if (pgnm_promotion.has_value())
        enum_promotion = std::optional<enumPiece>{
                PieceType_to_enumPiece(*pgnm_promotion)};

    return Move(
            position_to_index(PGNmove.start_get()), //from
            position_to_index(PGNmove.end_get()), // to
            PieceType_to_enumPiece(PGNmove.piece_get()), // pieceType
            std::nullopt, // captured
            enum_promotion, // promotion
            PGNmove.is_castling(),
            false, // it is never a en-passant ??
            0ull
    );
}

/*PgnMove CBmove_to_PGNmove(const chessBoard::Move& move)
{
    std::optional<PieceType > pgn_promotion = std::nullopt;
    if (move.is_promotion())
        pgn_promotion = std::optional<PieceType >{
                enumPiece_to_PieceType(move.promotion_type_get())};

    return PgnMove(
            index_to_position(ffsll(move.from_get()) - 1), //from
            index_to_position(ffsll(move.to_get()) - 1), // to
            enumPiece_to_PieceType(move.piece_get()), // pieceType
            move.is_capture(), // captured
            move.report_get(),
            pgn_promotion // promotion
    );
}*/