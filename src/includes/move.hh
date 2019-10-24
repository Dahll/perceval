//
// Created by fouche_r on 5/16/19.
//

#pragma once

#include "chessBoard.hh"
#include "report-type.hh"
#include "position.hh"
#include <tiff.h>
#include <optional>

namespace chessBoard
{
    class Move
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

        void castlings_set(uint64 c);
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
        void print() const;
        void print(std::ofstream& ofs) const;
        bool cmp_move(const chessBoard::Move& move2) const;
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
        int half_move_;
    };

/*    std::ofstream& operator<<(std::ofstream& os, Move& m)
    {
        std::string s = m.to_str();
        os << "test";
        return os;
    }*/

}