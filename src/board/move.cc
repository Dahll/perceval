//
// Created by fouche_r on 5/16/19.
//

#include <strings.h>
#include <stock.hh>
#include <pgn-parser.hh>
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

    POSITION_T Move::from_get() const {
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

    std::string Move::to_str()  const
    {
        int indexfrom = ffsll(from_get());
        int indexto = ffsll(to_get());
        std::string s;
        s.push_back(yaka::file_to_char[7 - ((indexfrom -1) % 8)]);
        s.push_back(yaka::rank_to_char[(indexfrom - 1) / 8]);
        s.push_back(yaka::file_to_char[7 - ((indexto -1) % 8)]);
        s.push_back(yaka::rank_to_char[(indexto - 1) / 8]);
        if (this->promotion_.has_value())
        {
            if (this->promotion_.value() == nBishop)
                s.push_back('b');
            else if (this->promotion_.value() == nRook)
                s.push_back('r');
            else if (this->promotion_.value() == nQueen)
                s.push_back('q');
            else if (this->promotion_.value() == nKnight)
                s.push_back('n');
        }
        return s;
    }

    int Move::half_move_get() const {
        return half_move_;
    }

    void Move::print() const
    {
        static const std::map<INDEX_T, const char> index_to_file{
                {7, 'a'},
                {6, 'b'},
                {5, 'c'},
                {4, 'd'},
                {3, 'e'},
                {2, 'f'},
                {1, 'g'},
                {0, 'h'},
        };
        static const std::map<INDEX_T, const char> index_to_rank{
                {0, '1'},
                {1, '2'},
                {2, '3'},
                {3, '4'},
                {4, '5'},
                {5, '6'},
                {6, '7'},
                {7, '8'},
        };

        std::cout << index_to_file.at((ffsll(from_get()) - 1) % 8)
                << index_to_rank.at((ffsll(from_get()) - 1) / 8)
                << index_to_file.at((ffsll(to_get()) - 1) % 8)
                << index_to_rank.at((ffsll(to_get()) - 1) / 8)
                << std::endl;

    }


    void Move::print(std::ofstream& ofs) const
    {
        static const std::map<INDEX_T, const char> index_to_file{
                {7, 'a'},
                {6, 'b'},
                {5, 'c'},
                {4, 'd'},
                {3, 'e'},
                {2, 'f'},
                {1, 'g'},
                {0, 'h'},
        };
        static const std::map<INDEX_T, const char> index_to_rank{
                {0, '1'},
                {1, '2'},
                {2, '3'},
                {3, '4'},
                {4, '5'},
                {5, '6'},
                {6, '7'},
                {7, '8'},
        };

        ofs << index_to_file.at((ffsll(from_get()) - 1) % 8)
                  << index_to_rank.at((ffsll(from_get()) - 1) / 8)
                  << index_to_file.at((ffsll(to_get()) - 1) % 8)
                  << index_to_rank.at((ffsll(to_get()) - 1) / 8)
                  << std::endl;

    }



    Move::Move(INDEX_T from, INDEX_T to, enumPiece pieceType,
               std::optional<enumPiece> captured,
               std::optional<enumPiece> promotion,
               bool is_castling, bool is_en_passant,
               uint64 special_move) :
            from_(tab_pos[from]),
            to_(tab_pos[to]),
            pieceType_(pieceType),
            captured_(captured),
            promotion_(promotion),
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
            captured_(captured),
            promotion_(promotion),
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
            captured_(captured),
            promotion_(promotion),
            is_castling_(is_castling),
            is_en_passant_(is_en_passant),
            special_move_(special_move),
            castlings_(castling),
            half_move_(half_move)
    {}

    bool Move::cmp_move(const chessBoard::Move& move2) const
    {
        if (move2.from_get() != from_)
        {
            return false;
        }
        if (move2.to_get() != to_)
        {
            return false;
        }
        if (move2.pieceType_ != pieceType_)
        {
            return false;
        }
        if (move2.captured_piece_type_get() != captured_)
        {
            return false;
        }
        if (move2.promotion_type_get() != promotion_)
        {
            return false;
        }
        if (move2.special_move_get() != special_move_)
        {
            return false;
        }
        return move2.castlings_get() == castlings_;
    }
}
