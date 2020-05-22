//
// Created by fouche_r on 5/16/19.
//

//#include "../magic/generate.hh"
#include "chessBoard.hh"
#include "pieces.hh"
#include <stock.hh>
#include <strings.h>
#include "board.hh"
#include "../adapter/pgn-parser.hh"
#include "../adapter/convertion.hh"
//#include <x86intrin.h>


namespace chessBoard
{

    bool Board::is_only_pawn(chessBoard::enumPiece color_) const
    {
        return __builtin_popcountll(pieceBB[2]) + 1 < __builtin_popcountll(pieceBB[color_]);
        //return (((pieceBB[color_] & pieceBB[7]) ^ pieceBB[color_]) & pieceBB[2]) == pieceBB[2];
    }


    void Board::apply_null_move(uint64 &hash_board)
    {
        hash_board ^= position_value[ffsll(special_moves)];
        special_moves = 0;
        hash_board ^= position_value[ffsll(special_moves)];
        hash_board ^= side_to_move;
    }

    void Board::revert_null_move(uint64 &hash_board, uint64 tmp_special_move)
    {
        hash_board ^= position_value[ffsll(special_moves)];
        special_moves = tmp_special_move;
        hash_board ^= position_value[ffsll(special_moves)];
        hash_board ^= side_to_move;
    }


    Board Board::copy_board() const
    {
        Board n;
        n.pieceBB[0] = this->pieceBB[0];
        n.pieceBB[1] = this->pieceBB[1];
        n.pieceBB[2] = this->pieceBB[2];
        n.pieceBB[3] = this->pieceBB[3];
        n.pieceBB[4] = this->pieceBB[4];
        n.pieceBB[5] = this->pieceBB[5];
        n.pieceBB[6] = this->pieceBB[6];
        n.pieceBB[7] = this->pieceBB[7];
        n.castlings = this->castlings;
        n.special_moves = this->special_moves;
        n.color = this->color;
        n.mat = this->mat;
        n.check = this->check;
        n.pat = this->pat;
        return n;
    }

    uint64 Board::apply_castling(const Move& m, const enumPiece& color_, uint64 hash_board)
    {
        /// king to remove
        uint64 king = 0;
        /// rook to remove
        uint64 rook = 0;
        /// new place of the king
        uint64 newking = 0;
        /// new place of the rook
        uint64 newrook = 0;
        /// mask to update the castlings
        uint64 casts = 0;
        if (color_ == nWhite)
        {
            king = tab_pos[3];
            casts = tab_pos[7] | tab_pos[0];
            if (m.isQueenCastle()) {
                rook = tab_pos[7];
                newking = tab_pos[5];
                newrook = tab_pos[4];
            }
            else
            {
                rook = tab_pos[0];
                newking = tab_pos[1];
                newrook = tab_pos[2];
            }
        }
        else
        {
            king = tab_pos[59];
            casts = tab_pos[63] | tab_pos[56];
            if (m.isQueenCastle()) {
                rook = tab_pos[63];
                newking = tab_pos[61];
                newrook = tab_pos[60];
            }
            else
            {
                rook = tab_pos[56];
                newking = tab_pos[57];
                newrook = tab_pos[58];
            }
        }
        /// remove old king

        pieceBB[color_] = pieceBB[color_] ^ king;
        pieceBB[nKing] = pieceBB[nKing] ^ king;
        /// remove old rook
        pieceBB[color_] = pieceBB[color_] ^ rook;
        pieceBB[nRook] = pieceBB[nRook] ^ rook;
        /// place rook
        pieceBB[color_] = pieceBB[color_] | newrook;
        pieceBB[nRook] = pieceBB[nRook] | newrook;
        /// place king
        pieceBB[color_] = pieceBB[color_] | newking;

        pieceBB[nKing] = pieceBB[nKing] | newking;
        /// update castlings
        castlings = (castlings ^ casts) & castlings;

        hash_board ^= position_value[65 + __builtin_ctzll(casts) + 1];
        casts &= casts - 1;
        hash_board ^= position_value[65 + casts];
        return hash_board;
    }

    chessBoard::Move Board::string_to_move(std::string& s, const enumPiece& color_)
    {
        auto rf = pgn_parser::to_file(s[0]);
        auto ff = pgn_parser::to_rank(s[1]);
        auto posf = yaka::Position(rf, ff);
        chessBoard::INDEX_T indexf = position_to_index(posf);
        auto rt = pgn_parser::to_file(s[2]);
        auto ft = pgn_parser::to_rank(s[3]);
        auto post = yaka::Position(rt, ft);
        chessBoard::INDEX_T indext = position_to_index(post);

        auto m = Move(indexf, indext, 0);

        uint16 flag = 0;

        if (m.getFromPosition() & (pieceBB[nPawn] & pieceBB[color_])) // IsPawn
        {
            // IsEnPassant
            if (m.getToPosition() & special_moves)
            {
                flag = 5u;
            }

        }
        if (m.getToPosition() & pieceBB[other_color(color_)]) // IsCapture
        {
            if (m.getFromPosition() & pieceBB[nPawn]) // IsPawn
            {
                if (s[4] != '\0') // test promotion
                {
                    if (s[4] == 'q')
                    {
                        flag = 15u;
                    }
                    else if (s[4] == 'r')
                    {
                        flag = 14u;
                    }
                    else if (s[4] == 'b')
                    {
                        flag = 13u;
                    }
                    else if (s[4] == 'n')
                    {
                        flag = 12u;
                    }
                }
                else
                {
                    flag = 4u;
                }

            }
            else
            {
                flag = 4u;
            }
        }
        else
        {
            if (m.getFromPosition() & pieceBB[nPawn]) // IsPawn
            {
                if (s[4] != '\0') // test promotion
                {
                    if (s[4] == 'q')
                    {
                        flag = 11u;
                    }
                    else if (s[4] == 'r')
                    {
                        flag = 10u;
                    }
                    else if (s[4] == 'b')
                    {
                        flag = 9u;
                    }
                    else if (s[4] == 'n')
                    {
                        flag = 8u;
                    }
                }
                else if (color_ == nWhite && (m.getToPosition() & white_pawn_jump[m.getFromIndex()])) // test double pawn push white
                {
                    flag = 1u;
                }
                else if (color_ == nBlack && (m.getToPosition() & black_pawn_jump[m.getFromIndex()])) // test double pawn push black
                {
                    flag = 1u;
                }
            }
            else if (m.getFromPosition() & pieceBB[nKing]) // IsKing
            {
                if (m.getFromIndex() == 3) // test castling
                {
                    if (m.getToIndex() == 1)
                    {
                        flag = 2u;
                    }
                    else if (m.getToIndex() == 5)
                    {
                        flag = 3u;
                    }
                }
                if (m.getFromIndex() == 59)
                {
                    if (m.getToIndex() == 57)
                    {
                        flag = 2u;
                    }
                    else if (m.getToIndex() == 61)
                    {
                        flag = 3u;
                    }
                }
            }

        }
        m.setFlags(flag);
        return m;
    }


    uint64 Board::apply_move(const Move& m, const enumPiece& color_, uint64 hash_board)
    {
        int piece_type = 0;


        if (pieceBB[2] & m.getFromPosition())
            piece_type = 2;
        else if (pieceBB[3] & m.getFromPosition())
            piece_type = 3;
        else if (pieceBB[4] & m.getFromPosition())
            piece_type = 4;
        else if (pieceBB[5] & m.getFromPosition())
            piece_type = 5;
        else if (pieceBB[6] & m.getFromPosition())
            piece_type = 6;
        else if (pieceBB[7] & m.getFromPosition())
            piece_type = 7;


        half_move_count_++;
        int color_position_offset = 0;
        hash_board ^= side_to_move;
        if (color_ == nBlack) {
            turn_count_++;
            color_position_offset = 6;
        }
        hash_board = hash_board ^ position_value[m.getFromIndex() + 1 + (piece_type + color_position_offset) * 65];

        hash_board ^= position_value[ffsl(special_moves)];
        special_moves = 0;
        if (m.isQueenCastle() || m.isKingCastle())
        {
            return apply_castling(m, color_, hash_board);
        }
        /// en passant
        else if (m.isEnPassantCapture())
        {
            // FIXME re-generate lists for en passant
            half_move_count_ = 0;
            uint64 pos = m.getToPosition();
            if (color_ == nWhite) {
                pos >>= 8ull;
                pieceBB[nBlack] = pieceBB[nBlack] ^ pos;
                pieceBB[nPawn] = pieceBB[nPawn] ^ pos;
                hash_board = hash_board ^ position_value[ffsll(pos) + (nPawn + 6 * other_color(color_)) * 65];
            }
            else
            {
                pos <<= 8ull;
                pieceBB[nWhite] = pieceBB[nWhite] ^ pos;
                pieceBB[nPawn] = pieceBB[nPawn] ^ pos;
                hash_board = hash_board ^ position_value[ffsll(pos) + (nPawn + 6 * other_color(color_)) * 65];
            }
        }
        /// captures
        else if ((m.isCapture()))
        {
            int capture_piece_type = 0;
            if (pieceBB[2] & m.getToPosition())
                capture_piece_type = 2;
            else if (pieceBB[3] & m.getToPosition())
                capture_piece_type = 3;
            else if (pieceBB[4] & m.getToPosition())
                capture_piece_type = 4;
            else if (pieceBB[5] & m.getToPosition())
                capture_piece_type = 5;
            else if (pieceBB[6] & m.getToPosition())
                capture_piece_type = 6;
            else if (pieceBB[7] & m.getToPosition())
                capture_piece_type = 7;
            hash_board = hash_board ^ position_value[m.getFromIndex() + 1 + (capture_piece_type + 6 * other_color(color_)) * 65];
            half_move_count_ = 0;
            enumPiece c = other_color(color_);
            pieceBB[c] = pieceBB[c] ^ m.getToPosition();
            pieceBB[capture_piece_type] =
                    pieceBB[capture_piece_type] ^ m.getToPosition();

            if (capture_piece_type == nRook)
            {
                hash_board ^= position_value[65 + ffsll(castlings & m.getToPosition())];
                castlings = (castlings ^ m.getToPosition()) & castlings;
                // FIXME is the if needed here ?
            }

        }
        else if (piece_type == nPawn)
        {
            half_move_count_ = 0;
            if (color_ == nWhite)
            {
                if (m.getToPosition() != m.getFromPosition() << 8ull) {
                    special_moves = m.getFromPosition() << 8ull;
                    hash_board ^= position_value[ffsll(special_moves)];
                }
            }
            else
            {
                if (m.getToPosition() != m.getFromPosition() >> 8ull) {
                    special_moves = m.getFromPosition() >> 8ull;
                    hash_board ^= position_value[ffsll(special_moves)];
                }
            }
        }




        /// promotion
        if (m.isPromotion()) {
            int promotion = 0;
            int tmp = m.getFlags() & 3u;
            if (tmp == 3)
                promotion = 6;
            else if (tmp == 2)
                promotion = 5;
            else if (tmp == 1)
                promotion = 4;
            else if (tmp == 0)
                promotion = 3;
            pieceBB[promotion] =
                    pieceBB[promotion] | m.getToPosition();
            hash_board = hash_board ^ position_value[m.getToIndex() + 1 + (promotion + color_position_offset) * 65];
        }
        else {
            pieceBB[piece_type] = pieceBB[piece_type] | m.getToPosition();
            hash_board = hash_board ^ position_value[m.getToIndex() + 1 + (piece_type + color_position_offset) * 65];
        }

        if (piece_type == nRook)
        {
            // FIXME is this actually faster than just applying the mask for any piece type ??
            hash_board ^= position_value[65 + ffsll(castlings & m.getFromPosition())];
            castlings = (castlings ^ m.getFromPosition()) & castlings;
        }
        else if (piece_type == nKing)
        {
            if (color_ == nWhite) {
                hash_board ^= position_value[65 + ffsll(tab_pos[0] & castlings)];
                hash_board ^= position_value[65 + ffsll(castlings & tab_pos[7])];
                castlings = ((castlings ^ tab_pos[7]) ^ tab_pos[0]) & castlings;
            }
            else {
                hash_board ^= position_value[65 + ffsll(castlings & tab_pos[56])];
                hash_board ^= position_value[65 + ffsll(tab_pos[63] & castlings)];
                castlings = ((castlings ^ tab_pos[63]) ^ tab_pos[56]) & castlings;
            }
        }
        /// move the piece
        pieceBB[color_] = pieceBB[color_] ^ m.getFromPosition();
        pieceBB[color_] = pieceBB[color_] | m.getToPosition();
        pieceBB[piece_type] = pieceBB[piece_type] ^ m.getFromPosition();
        return hash_board;
    }


    opt_piece_t Board::operator[](const INDEX_T &i) const
    {
        enumPiece c = nWhite;
        if (pieceBB[nBlack] & tab_pos[i])
            c = nBlack;
        else if (pieceBB[nWhite] & tab_pos[i])
            c = nWhite;
        else
            return std::nullopt;

        enumPiece type = nPawn;
        for (int j = nPawn; j <= nKing; ++j)
        {
            if (tab_pos[i] & pieceBB[j])
                type = int_to_enumPiece[i];
        }
        return opt_piece_t(std::pair<enumPiece, enumPiece >(type, c));
    }

    Board::Board(bool empty)
    {
        if (empty)
            return;


        add_piece(WhitePawn, pos_from_index(15));
        add_piece(WhitePawn, pos_from_index(14));
        add_piece(WhitePawn, pos_from_index(13));
        add_piece(WhitePawn, pos_from_index(12));
        add_piece(WhitePawn, pos_from_index(11));
        add_piece(WhitePawn, pos_from_index(10));
        add_piece(WhitePawn, pos_from_index(9));
        add_piece(WhitePawn, pos_from_index(8));

        add_piece(WhiteRook, pos_from_index(7));
        add_piece(WhiteKnight, pos_from_index(6));
        add_piece(WhiteBishop, pos_from_index(5));
        add_piece(WhiteQueen, pos_from_index(4));
        add_piece(WhiteKing, pos_from_index(3));
        add_piece(WhiteBishop, pos_from_index(2));
        add_piece(WhiteKnight, pos_from_index(1));
        add_piece(WhiteRook, pos_from_index(0));


        add_piece(BlackPawn, pos_from_index(55));
        add_piece(BlackPawn, pos_from_index(54));
        add_piece(BlackPawn, pos_from_index(53));
        add_piece(BlackPawn, pos_from_index(52));
        add_piece(BlackPawn, pos_from_index(51));
        add_piece(BlackPawn, pos_from_index(50));
        add_piece(BlackPawn, pos_from_index(49));
        add_piece(BlackPawn, pos_from_index(48));

        add_piece(BlackRook, pos_from_index(63));
        add_piece(BlackKnight, pos_from_index(62));
        add_piece(BlackBishop, pos_from_index(61));
        add_piece(BlackQueen, pos_from_index(60));
        add_piece(BlackKing, pos_from_index(59));
        add_piece(BlackBishop, pos_from_index(58));
        add_piece(BlackKnight, pos_from_index(57));
        add_piece(BlackRook, pos_from_index(56));

        castlings = 0;
        castlings = (castlings | tab_pos[0]);
        castlings = (castlings | tab_pos[7]);
        castlings = (castlings | tab_pos[56]);
        castlings = (castlings | tab_pos[63]);
        special_moves = 0;
    }

    void Board::add_piece(const piece_color_t &pc, const POSITION_T &pos)
    {
        pieceBB[pc.second] = (pieceBB[pc.second] | pos);
        pieceBB[pc.first] = (pieceBB[pc.first] | pos);
    }

    uint64 index_to_uint64(int index)
    {
        return tab_pos[index];
    }

    uint16 uint64_to_index(uint64 pos)
    {
        return __builtin_ffsll(pos) - 1;
    }

    int Board::moves_count()
    {
        return generate_moves(this->color).size();
    }

    enumPiece Board::other_color(const enumPiece& color_) const
    {
        return color_ == chessBoard::nWhite
               ? chessBoard::nBlack
               : chessBoard::nWhite;
    }

    void Board::switch_color()
    {
        color = other_color(color);
    }

    /* Check functions */


    bool Board::check_promotion(const Move& move, const Move& m) const
    {
        if (m.isPromotion() == move.isPromotion())
        {
            if (m.isPromotion())
            {
                int promotion_m = 0;
                int tmp = m.getFlags() & 3u;
                if (tmp == 3)
                    promotion_m = 6;
                else if (tmp == 2)
                    promotion_m = 5;
                else if (tmp == 1)
                    promotion_m = 4;
                else if (tmp == 0)
                    promotion_m = 3;

                int promotion_move = 0;
                auto tmp_move = move.getFlags() & 3u;
                if (tmp_move == 3u)
                    promotion_move = 6;
                else if (tmp_move == 2u)
                    promotion_move = 5;
                else if (tmp_move == 1u)
                    promotion_move = 4;
                else if (tmp_move == 0u)
                    promotion_move = 3;

                return promotion_m == promotion_move;
            }
            return true;
        }
        return false;
    }

    // Returns true if move is valid (and applied)
    std::optional<Move> Board::check_apply_move(Move& move)
    {
        // FIXME check if our castlings are done the same way as pgn castlings
        // (first are pgn castlings always the same way, if yes is it the king that moves or the rook)

        if (!move.getToPosition() || !move.getFromPosition())
            return std::nullopt;
        auto& moves = generate_moves(this->color);
        // FIXME set check mat pat
        if (moves.empty())
            return std::nullopt;
        for (auto& m : moves)
        {
            if (m.getFromPosition() == move.getFromPosition() && m.getToPosition() == move.getToPosition())
            {
                if ((m.isQueenCastle() || m.isKingCastle()) && !(move.isQueenCastle() || move.isKingCastle()))
                    continue;
                uint64 h = 0;
                if (check_promotion(move, m)) {
                    apply_move(m, color, h);
                    enumPiece c = other_color(color);
                    set_check_mat_pat(c, generate_moves(c));
                    return m;
                }
            }
        }
        // FIXME set check mat pat here ?

        return std::nullopt;
    }


    bool Board::square_is_check(const enumPiece& color_, const POSITION_T& pos) const
    {
        uint64 king = pos;
        int index = __builtin_ctzll(king);
        const uint64 occ = (pieceBB[0] | pieceBB[1]);
        enumPiece no_color = other_color(color_);

        // Check for knight
        if ((knight_move[index] & (pieceBB[enumPiece::nKnight] & pieceBB[no_color])) != 0)
        {
            return true;
        }
        if ((king_move[index] & (pieceBB[no_color] & pieceBB[7])) != 0)
        {
            return true;
        }

        // check for tower + queen tower
        const uint64 tower_pos = (pieceBB[5] & pieceBB[no_color]) | (pieceBB[6] & pieceBB[no_color]);
        //const int pos_magique_tower = ((tower_move[index] & (pieceBB[0] | pieceBB[1])) * magic_number_tower[index]) >> 52ull;
        //if ((tower_output[index][pos_magique_tower] & tower_pos) != 0)
        if ((rookAttacks(occ, index) & tower_pos) != 0)
        {
            return true;
        }

        // check for bishop  + queen bishop
        const uint64 bishop_pos = (pieceBB[4] & pieceBB[no_color]) | (pieceBB[6] & pieceBB[no_color]);
        //const int pos_magique_bishop = (((pieceBB[0] | pieceBB[1]) & bishop_move[index]) * magic_number_bishop[index]) >> 52ull;
        //if ((bishop_output[index][pos_magique_bishop] & bishop_pos) != 0)
        if ((bishopAttacks(occ, index) & bishop_pos) != 0)
        {
            return true;
        }

        // check pawn
        if (color_ == nWhite)
        {
            if ((white_pawn_attack[index] & (pieceBB[2] & pieceBB[no_color])) != 0)
            {
                return true;
            }
        }
        else
        {
            if ((black_pawn_attack[index] & (pieceBB[2] & pieceBB[no_color])) != 0)
            {
                return true;
            }
        }
        return false;
    }

    void Board::move_attack_square(MOVES_T& moves, const enumPiece& no_color,
            uint64 pos) const
    {
        uint64 king = pos;

        uint16 index = __builtin_ctzll(king);
        const uint64 occ = pieceBB[0] | pieceBB[1];
        uint64 i = 0;

        // Check for knight
        uint64 knight_mask = knight_move[index] & (pieceBB[3] & pieceBB[no_color]);
        while (knight_mask != 0)
        {
            i = __builtin_ctzll(knight_mask);
            knight_mask &= knight_mask - 1;
            add_move(no_color, moves,
                     {static_cast<uint16 >(i), index, 4});
        }
        // check for king
        uint64 king_mask = king_move[index] & (pieceBB[no_color] & pieceBB[7]);
        if (king_mask != 0)
        {
            i = __builtin_ctzll(king_mask);
            add_move(no_color, moves,
                     {static_cast<uint16 >(i), index, 4});
        }
        // check for tower + queen tower
        const uint64 tower_pos = (pieceBB[5] & pieceBB[no_color]);
        //const int pos_magique_tower = ((tower_move[index] & (pieceBB[0] | pieceBB[1])) * magic_number_tower[index]) >> 52ull;
        //uint64 tower_mask = tower_output[index][pos_magique_tower] & tower_pos;
        uint64 tower_mask = rookAttacks(occ, index) & tower_pos;
        while (tower_mask != 0)
        {
            i = __builtin_ctzll(tower_mask);
            tower_mask &= tower_mask - 1;
            add_move(no_color, moves,
                     {static_cast<uint16 >(i), index, 4});
        }
        // check for bishop  + queen bishop
        const uint64 bishop_pos = (pieceBB[4] & pieceBB[no_color]);
        //const int pos_magique_bishop = (((pieceBB[0] | pieceBB[1]) & bishop_move[index]) * magic_number_bishop[index]) >> 52ull;
        //uint64 bishop_mask = bishop_output[index][pos_magique_bishop] & bishop_pos;
        uint64 bishop_mask = bishopAttacks(occ, index) & bishop_pos;
        while (bishop_mask != 0)
        {
            i = __builtin_ctzll(bishop_mask);
            bishop_mask &= bishop_mask - 1;
            add_move(no_color, moves,
                     {static_cast<uint16 >(i), index, 4});
        }
        // check queen_tower
        const uint64 queen_tower_pos = (pieceBB[6] & pieceBB[no_color]);
        //const int queen_pos_magique_tower = ((tower_move[index] & (pieceBB[0] | pieceBB[1])) * magic_number_tower[index]) >> 52ull;
        //uint64 queen_tower_mask = tower_output[index][queen_pos_magique_tower] & queen_tower_pos;
        uint64 queen_tower_mask = rookAttacks(occ, index) & queen_tower_pos;
        while (queen_tower_mask != 0)
        {
            i = __builtin_ctzll(queen_tower_mask);
            queen_tower_mask &= queen_tower_mask - 1;
            add_move(no_color, moves,
                     {static_cast<uint16 >(i), index, 4});
        }
        // check queen_bishop
        const uint64 queen_bishop_pos = (pieceBB[6] & pieceBB[no_color]);
        //const int queen_pos_magique_bishop = (((pieceBB[0] | pieceBB[1]) & bishop_move[index]) * magic_number_bishop[index]) >> 52ull;
        //uint64 queen_bishop_mask = bishop_output[index][queen_pos_magique_bishop] & queen_bishop_pos;
        uint64 queen_bishop_mask = bishopAttacks(occ, index) & queen_bishop_pos;
        while (queen_bishop_mask != 0)
        {
            i = __builtin_ctzll(queen_bishop_mask);
            queen_bishop_mask &= queen_bishop_mask - 1;
            add_move(no_color, moves,
                     {static_cast<uint16 >(i), index, 4});
        }

        // check pawn
        if (no_color == nBlack)
        {
            uint64  mask_black_pawn = white_pawn_attack[index] & (pieceBB[2] & pieceBB[no_color]);
            while (mask_black_pawn != 0)
            {
                i = __builtin_ctzll(mask_black_pawn);
                mask_black_pawn &= mask_black_pawn - 1;
                if ((index) / 8 == 7)
                {
                    add_move(no_color, moves,
                             {static_cast<uint16 >(i), index, 15});
                    add_move(no_color, moves,
                             {static_cast<uint16 >(i), index, 14});
                    add_move(no_color, moves,
                             {static_cast<uint16 >(i), index, 13});
                    add_move(no_color, moves,
                             {static_cast<uint16 >(i), index, 12});
                }
                else
                {
                    add_move(no_color, moves,
                             {static_cast<uint16 >(i), index, 4});
                }
            }
        }
        else
        {
            uint64 mask_white_pawn = black_pawn_attack[index] & (pieceBB[2] & pieceBB[no_color]);
            while (mask_white_pawn != 0)
            {
                i = __builtin_ctzll(mask_white_pawn);
                mask_white_pawn &= mask_white_pawn - 1;
                if ((index) / 8 == 0)
                {
                    add_move(no_color, moves,
                             {static_cast<uint16 >(i), index, 15});
                    add_move(no_color, moves,
                             {static_cast<uint16 >(i), index, 14});
                    add_move(no_color, moves,
                             {static_cast<uint16 >(i), index, 13});
                    add_move(no_color, moves,
                             {static_cast<uint16 >(i), index, 12});
                }
                else
                {
                    add_move(no_color, moves,
                             {static_cast<uint16 >(i), index, 4});
                }
            }
        }
    }

    bool Board::player_is_check(const enumPiece& color_) const
    {
        uint64 king = pieceBB[7] & pieceBB[color_];
        int index = __builtin_ctzll(king);
        enumPiece no_color = other_color(color_);
        const uint64 occ = pieceBB[0] | pieceBB[1];
        // Check for knight
        if ((knight_move[index] & (pieceBB[3] & pieceBB[no_color])) != 0)
        {
            return true;
        }
        // check for queen
        if ((king_move[index] & (pieceBB[no_color] & pieceBB[7])) != 0)
        {
            return true;
        }

        // check for tower + queen tower
        const uint64 tower_pos = (pieceBB[5] & pieceBB[no_color]) | (pieceBB[6] & pieceBB[no_color]);
        //const int pos_magique_tower = ((tower_move[index] & (pieceBB[0] | pieceBB[1])) * magic_number_tower[index]) >> 52ull;
        //if ((tower_output[index][pos_magique_tower] & tower_pos) != 0)
        if ((rookAttacks(occ, index) & tower_pos) != 0)
        {
            return true;
        }

        // check for bishop  + queen bishop
        const uint64 bishop_pos = (pieceBB[4] & pieceBB[no_color]) | (pieceBB[6] & pieceBB[no_color]);
        //const int pos_magique_bishop = (((pieceBB[0] | pieceBB[1]) & bishop_move[index]) * magic_number_bishop[index]) >> 52ull;
        //if ((bishop_output[index][pos_magique_bishop] & bishop_pos) != 0)
        if ((bishopAttacks(occ, index) & bishop_pos) != 0)
        {
            return true;
        }

        // check pawn
        if (color_ == nWhite)
        {
            if ((white_pawn_attack[index] & (pieceBB[2] & pieceBB[no_color])) != 0)
            {
                return true;
            }
        }
        else
        {
            if ((black_pawn_attack[index] & (pieceBB[2] & pieceBB[no_color])) != 0)
            {
                return true;
            }
        }

        return false;
    }

    bool Board::player_is_mat(enumPiece color_, const std::vector<Move>& moves)
    {

        return moves.empty() && player_is_check(color_);
    }

    bool Board::player_is_pat(enumPiece color_, const std::vector<Move>& moves)
    {
        return moves.empty() && !player_is_check(color_);
    }

    void Board::set_check_mat_pat(enumPiece color_, const std::vector<Move>& moves)
    {
        if (player_is_check(color_))
        {
            check = true;
            pat = false;
            mat = moves.empty();
        }
        else
        {
            check = false;
            mat = false;
            pat = moves.empty();
        }
    }


    POSITION_T Board::captured_piece_position(const Move& move)
    {
/*        if (move.is_en_passant()) {
            if (move.to_get() > move.from_get()) {
                if (move.to_get() > (move.from_get() << 8ull))
                    return move.from_get() << 1ull;
                return move.from_get() >> 1ull;
            }
            if (move.from_get() > (move.to_get() << 8ull))
                return move.from_get() >> 1ull;
            return move.from_get() << 1ull;
        }*/
        return move.getToPosition();
    }


    bool Board::game_is_draw(const Move &)
    {
        // FIXME
        return false;
    }

    /* What ?
    // Was the previous move a castling ?
    bool Board::check_castling()
    {
        // FIXME
        return false;
    }

    bool Board::castling_is_queenside()
    {
        // FIXME
        return false;
    }*/

    void Board::print() const
    {
        char board[64];
        for (int i = nWhite; i <= nKing; i++)
        {
//            print_bitboard(b.pieceBB[i]);
//            std::cout << std::endl;
        }
        for (char & i : board)
            i = '0';
        for (int i = nPawn; i <= nKing; i++)
        {
            uint64 minib = pieceBB[i];
            while (minib != 0) {
                uint64 pos = 1ull << (__builtin_ctzll(minib));
                minib &= minib - 1;
                if (pos & pieceBB[nWhite])
                    board[ffsll(pos) - 1] = epiece_to_char[i];
                else if (pos & pieceBB[nBlack])
                    board[ffsll(pos) - 1] = epiece_to_char[i] + 'a' - 'A';
            }
        }
        int h = 8;
        for (int i = 0; i < 64; i++)
        {
            if (i % 8 == 0)
                std::cout << std::endl << h-- << "   ";
            std::cout << board[63 - i] << " ";
        }
        std::cout << std::endl << std::endl << "    a b c d e f g h";
        std::cout << std::endl;
    }

    bool Board::is_board_valide(Move& mov, enumPiece& color_)
    {
        uint64 h = 0;
        this->apply_move(mov, color_, h);
        if (number_piece(pieceBB[0] & pieceBB[2]) > 8)
        {
            return false;
        }
        if (number_piece(pieceBB[0] & pieceBB[3]) > 2)
        {
            return false;
        }
        if (number_piece(pieceBB[0] & pieceBB[4]) > 2)
        {
            return false;
        }
        if (number_piece(pieceBB[0] & pieceBB[5]) > 2)
        {
            return false;
        }
        if (number_piece(pieceBB[0] & pieceBB[6]) > 1)
        {
            return false;
        }
        if (number_piece(pieceBB[0] & pieceBB[7]) > 1)
        {
            return false;
        }
        if (number_piece(pieceBB[1] & pieceBB[2]) > 8)
        {
            return false;
        }
        if (number_piece(pieceBB[1] & pieceBB[3]) > 2)
        {
            return false;
        }
        if (number_piece(pieceBB[1] & pieceBB[4]) > 2)
        {
            return false;
        }
        if (number_piece(pieceBB[1] & pieceBB[5]) > 2)
        {
            return false;
        }
        if (number_piece(pieceBB[1] & pieceBB[6]) > 1)
        {
            return false;
        }
        if (number_piece(pieceBB[1] & pieceBB[7]) > 1)
        {
            return false;
        }
        return true;
    }


    /*void Board::test_board(std::vector<Move>& vec)
    {
        int count = vec.size();
        std::stringstream ss;
        ss << count;
        std::string str = ss.str();
        std::string a = this->to_perft();
        //std::string ret = "python3 ../tests/bad_board/output.py \"";
        std::string ret = "python3 /home/sqwaz/chess/tests/bad_board/output.py \"";
        ret += a;
        ret += "\" ";
        ret += str;
        auto zz = std::system(ret.c_str());
        zz = zz;
    }*/


    std::string Board::to_perft() const
    {
        std::string ret = "";
        int i = 64;
        int temp = 0;
        while (i > 0)
        {
            char a = '\0';
            if ((tab_pos[i] & (pieceBB[1] & pieceBB[2])) != 0)
            {
                a = 'p';
            }
            else if ((tab_pos[i] & (pieceBB[1] & pieceBB[3])) != 0)
            {
                a = 'n';
            }
            else if ((tab_pos[i] & (pieceBB[1] & pieceBB[4])) != 0)
            {
                a = 'b';
            }
            else if ((tab_pos[i] & (pieceBB[1] & pieceBB[5])) != 0)
            {
                a = 'r';
            }
            else if ((tab_pos[i] & (pieceBB[1] & pieceBB[6])) != 0)
            {
                a = 'q';
            }
            else if ((tab_pos[i] & (pieceBB[1] & pieceBB[7])) != 0)
            {
                a = 'k';
            }
            else if ((tab_pos[i] & (pieceBB[0] & pieceBB[2])) != 0)
            {
                a = 'P';
            }
            else if ((tab_pos[i] & (pieceBB[0] & pieceBB[3])) != 0)
            {
                a = 'N';
            }
            else if ((tab_pos[i] & (pieceBB[0] & pieceBB[4])) != 0)
            {
                a = 'B';
            }
            else if ((tab_pos[i] & (pieceBB[0] & pieceBB[5])) != 0)
            {
                a = 'R';
            }
            else if ((tab_pos[i] & (pieceBB[0] & pieceBB[6])) != 0)
            {
                a = 'Q';
            }
            else if ((tab_pos[i] & (pieceBB[0] & pieceBB[7])) != 0)
            {
                a = 'K';
            }
            if (a == '\0')
            {
                temp += 1;
            }
            else
            {
                if (temp != 0)
                {
                    ret.push_back(temp + 48);
                    temp = 0;
                }
                ret.push_back(a);
            }
            if ((i - 1) % 8 == 0 && i - 1 != 0)
            {
                if (temp != 0)
                {
                    ret.push_back(temp + 48);
                    temp = 0;
                }
                ret.push_back('/');
            }
            else if ((i - 1) % 8 == 0 && i - 1 == 0)
            {
                if (temp != 0)
                {
                    ret.push_back(temp + 48);
                    temp = 0;
                }
            }
            i--;
        }
        ret.push_back(' ');
        if (color == nWhite)
        {
            ret.push_back('w');
        }
        else
        {
            ret.push_back('b');
        }
        ret.push_back(' ');
        int tp = 0;
        if ((castlings & tab_pos[0]) != 0)
        {
            ret.push_back('K');
            tp = 1;
        }
        if ((castlings & tab_pos[7]) != 0)
        {
            ret.push_back('Q');
            tp = 1;
        }
        if ((castlings & tab_pos[56]) != 0)
        {
            ret.push_back('k');
            tp = 1;
        }
        if ((castlings & tab_pos[63]) != 0)
        {
            ret.push_back('q');
            tp = 1;
        }
        if (tp == 0)
            ret.push_back('-');
        ret.push_back(' ');

        int min_ = 17;
        int max_ = 25;
        if (color == nBlack)
        {
            min_ = 41;
            max_ = 49;
        }

        int j = min_;
        while (j < max_)
        {
            if ((tab_pos[j] && special_moves) != 0)
            {
                ret.push_back(yaka::file_to_char[7 - ((j - 1) % 8)]);
                ret.push_back(yaka::rank_to_char[(j - 1) / 8]);
                break;
            }
            ++j;
        }
        if (j == 25 || j == 49)
            ret.push_back('-');
        ret.push_back(' ');
        ret.push_back('0');
        ret.push_back(' ');
        ret.push_back('0');
        ret.push_back('\n');
        return ret;
    }

    /*int Board::get_current_pieces_count() const
    {
        return _popcnt64(pieceBB[nWhite] | pieceBB[nBlack]);
    }*/

    int Board::get_current_captures_count() const
    {
        return generate_capture_moves(this->color).size();
    }

    int Board::get_current_moves_count() const
    {
        return generate_moves(this->color).size();
    }

    /*Board Board::apply_many_moves(const MOVES_T& moves)
    {
        auto cur_color = this->color;
        uint64 h = 0;
        for (const auto& move : moves)
        {
            this->apply_move(move, cur_color, h);
            cur_color = other_color(cur_color);
        }
        return *this;
    }*/
}
