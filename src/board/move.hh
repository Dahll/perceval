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
        m_Move = 0u;
    }

    Move(uint16 from, uint16 to, uint16 flags)
    {
        m_Move = flags << 12u | (from << 6u) | to;
    }

    constexpr Move(const chessBoard::Move&) = default;

    Move& operator=(Move a) {m_Move = a.m_Move; return *this; }

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

    class VMove
    {
    public:
        void add_move(const Move& mv);
        uint8 len = 0;
        Move move_tab[256];
    };


}
