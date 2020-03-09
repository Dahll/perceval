#include <utility>

//
// Created by fouche_r on 5/16/19.
//

#include <strings.h>
#include <stock.hh>
#include "../adapter/pgn-parser.hh"
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>

//#include "sstream"
#include "move.hh"

namespace chessBoard
{

    /*POSITION_T Move::from_get() const {
        return from_;
    }

    POSITION_T Move::to_get() const {
        return to_;
    }

    enumPiece Move::piece_get() const{
        return pieceType_;
    }

    enumPiece Move::captured_piece_type_get() const {
        return *captured_;
    }

    enumPiece Move::promotion_type_get() const
    {
        return *promotion_;
    }

    bool Move::is_capture() const {
        return captured_.has_value();
    }

    void Move::capture_set(enumPiece b)
    {
        captured_ = b;
    }

    void Move::piece_set(enumPiece type)
    {
        pieceType_ = type;
    }

    bool Move::is_castling() const {
        return is_castling_;
    }

    bool Move::is_en_passant() const {
        return is_en_passant_;
    }

    bool Move::is_promotion() const {
        return promotion_.has_value();
    }

    uint64 Move::special_move_get() const
    {
        return special_move_;
    }

    bool Move::is_castling_queenside() const
    {
        return to_ > from_;
    }


    uint64 Move::castlings_get() const
    {
        return castlings_;
    }

    int Move::half_move_get() const
    {
        return half_move_;
    }*/

    std::string Move::to_str()  const
    {
        int indexfrom = ffsll(this->getFromPosition());
        int indexto = ffsll(this->getToPosition());
        std::string s;
        s.push_back(yaka::file_to_char[7 - ((indexfrom -1) % 8)]);
        s.push_back(yaka::rank_to_char[(indexfrom - 1) / 8]);
        s.push_back(yaka::file_to_char[7 - ((indexto -1) % 8)]);
        s.push_back(yaka::rank_to_char[(indexto - 1) / 8]);
        if (this->isPromotion())
        {
            if (this->isBishopPromotion() || this->isBishopPromotionCapture())
                s.push_back('b');
            else if (this->isRookPromotion() || this->isRookPromotionCapture())
                s.push_back('r');
            else if (this->isQueenPromotion() || this->isQueenPromotionCapture())
                s.push_back('q');
            else if (this->isKnightPromotion() || this->isKnightPromotionCapture())
                s.push_back('n');
        }
        return s;
    }

    /*Move::Move(INDEX_T from, INDEX_T to, enumPiece pieceType,
               std::optional<enumPiece> captured,
               std::optional<enumPiece> promotion,
               bool is_castling, bool is_en_passant,
               uint64 special_move) :
            from_(tab_pos[from]),
            to_(tab_pos[to]),
            pieceType_(pieceType),
            captured_(std::move(captured)),
            promotion_(std::move(promotion)),
            is_castling_(is_castling),
            is_en_passant_(is_en_passant),
            special_move_(special_move),
            castlings_(0),
            half_move_(0)
    {}

    Move::Move(POSITION_T from, POSITION_T to, enumPiece pieceType,
               std::optional<enumPiece> captured,
               std::optional<enumPiece> promotion,
               bool is_castling, bool is_en_passant,
               uint64 special_move) :
            from_(from),
            to_(to),
            pieceType_(pieceType),
            captured_(std::move(captured)),
            promotion_(std::move(promotion)),
            is_castling_(is_castling),
            is_en_passant_(is_en_passant),
            special_move_(special_move),
            castlings_(0),
            half_move_(0)
    {}


    Move::Move(POSITION_T from, POSITION_T to, enumPiece pieceType,
               std::optional<enumPiece> captured,
               std::optional<enumPiece> promotion,
               bool is_castling, bool is_en_passant,
               uint64 special_move,
               uint64 castling,
               int half_move) :
            from_(from),
            to_(to),
            pieceType_(pieceType),
            captured_(std::move(captured)),
            promotion_(std::move(promotion)),
            is_castling_(is_castling),
            is_en_passant_(is_en_passant),
            special_move_(special_move),
            castlings_(castling),
            half_move_(half_move)
    {}*/


}

