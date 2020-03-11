//
// Created by fouche_r on 5/16/19.
//

#pragma once

#include "chessBoard.hh"
#include "report-type.hh"
#include "position.hh"
#include <tiff.h>
#include <optional>

/*
-----------------------------------------------------------------------------
code 	promotion 	capture 	special 1 	special 0 	kind of move         |
-----------------------------------------------------------------------------|
0 	    0 	        0 	        0 	        0 	        quiet moves          |
1 	    0 	        0 	        0 	        1 	        double pawn push     |
2 	    0 	        0 	        1 	        0 	        king castle          |
3 	    0 	        0 	        1 	        1 	        queen castle         |
4 	    0 	        1 	        0 	        0 	        captures             |
5 	    0 	        1 	        0 	        1 	        ep-capture           |
8 	    1 	        0 	        0 	        0 	        knight-promotion     |
9 	    1 	        0 	        0 	        1 	        bishop-promotion     |
10 	    1 	        0 	        1 	        0 	        rook-promotion       |
11 	    1 	        0 	        1 	        1 	        queen-promotion      |
12 	    1 	        1 	        0 	        0 	        knight-promo capture |
13 	    1 	        1 	        0 	        1 	        bishop-promo capture |
14 	    1 	        1 	        1 	        0 	        rook-promo capture   |
15 	    1 	        1 	        1 	        1 	        queen-promo capture  |
-----------------------------------------------------------------------------
*/

namespace chessBoard
{
class Move {
public:
    Move()
    {
        m_Move = 0;
    }

    Move(uint16 from, uint16 to, uint16 flags)
    {
        m_Move = flags << 12u | (from << 6u) | to;
    }

    void operator=(Move a) {m_Move = a.m_Move;}

    uint8 getToIndex() const {return m_Move & 0x3fu;}

    uint64 getToPosition() const {return 1ull << (m_Move & 0x3fu);}

    uint8 getFromIndex() const {uint8 t = m_Move >> 6u; return t & 0x3fu;}

    uint64 getFromPosition() const {uint8 t = m_Move >> 6u; return 1ull << (t & 0x3fu);}

    uint8 getFlags() const {return m_Move >> 12u;}

    void setTo(uint8 to) {m_Move &= 0xf03fu; m_Move |= to;}

    void setFrom(uint16 from) {m_Move &= 0xffc0u; from = from << 6u; m_Move |= from;}

    void setFlags(uint16 flags) {m_Move &= 0xfffu;flags = flags << 12u;m_Move |= flags;}

    bool isCapture() const {return (m_Move & 0x4000u) != 0;}

    bool isPromotion() const {return (m_Move & 0x8000u) != 0;}

    bool isQuiet() const {uint8 t = m_Move >> 12u; return t == 0;}

    bool isDoublePawnPush() const {uint8 t = m_Move >> 12u; return t == 1u;}

    bool isKingCastle() const {uint8 t = m_Move >> 12u; return t == 2u;}

    bool isQueenCastle() const {uint8 t = m_Move >> 12u; return t == 3u;}

    bool isSimpleCapture() const {uint8 t = m_Move >> 12u; return t == 4u;}

    bool isEnPassantCapture() const {uint8 t = m_Move >> 12u; return t == 5u;}

    bool isKnightPromotion() const {uint8 t = m_Move >> 12u; return t == 8u;}

    bool isBishopPromotion() const {uint8 t = m_Move >> 12u; return t == 9u;}

    bool isRookPromotion() const {uint8 t = m_Move >> 12u; return t == 10u;}

    bool isQueenPromotion() const {uint8 t = m_Move >> 12u; return t == 11u;}

    bool isKnightPromotionCapture() const {uint8 t = m_Move >> 12u; return t == 12u;}

    bool isBishopPromotionCapture() const {uint8 t = m_Move >> 12u; return t == 13u;}

    bool isRookPromotionCapture() const {uint8 t = m_Move >> 12u; return t == 14u;}

    bool isQueenPromotionCapture() const {uint8 t = m_Move >> 12u; return t == 15u;}

    bool operator==(Move a) const {return m_Move == a.m_Move;}

    bool operator!=(Move a) const {return m_Move != a.m_Move;}

    std::string to_str() const ;

protected:
    uint16 m_Move;
};


    /*class Move
    {
    public:
        Move() = default;
        Move(INDEX_T from, INDEX_T to,
            enumPiece pieceType,
            std::optional<enumPiece> captured,
            std::optional<enumPiece> promotion,
            bool is_castling, bool is_en_passant,
            uint64 special_move);

        Move(POSITION_T from, POSITION_T to, enumPiece pieceType,
                   std::optional<enumPiece> captured,
                   std::optional<enumPiece> promotion,
                   bool is_castling, bool is_en_passant,
                   uint64 special_move);

        Move(POSITION_T from, POSITION_T to, enumPiece pieceType,
             std::optional<enumPiece> captured,
             std::optional<enumPiece> promotion,
             bool is_castling, bool is_en_passant,
             uint64 special_move,
             uint64 castling_,
             int half_move);


        POSITION_T from_get() const;

        POSITION_T to_get() const;

        uint64 castlings_get() const;

        enumPiece piece_get() const;
        void piece_set(enumPiece type);
        enumPiece captured_piece_type_get() const;
        enumPiece promotion_type_get() const;

        bool is_capture() const;
        void capture_set(enumPiece b);
        bool is_castling() const;
        bool is_castling_queenside() const;
        bool is_en_passant() const;
        bool is_promotion() const;

        uint64 special_move_get() const;
        std::string to_str() const;
        int half_move_get() const;
    private:
        POSITION_T from_;
        POSITION_T to_;

        enumPiece pieceType_;
        std::optional<enumPiece> captured_;
        std::optional<enumPiece> promotion_;

        bool is_castling_;
        bool is_en_passant_;
        uint64 special_move_;
        uint64 castlings_;
        int half_move_; //supr this
    };*/

}