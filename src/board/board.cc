//
// Created by fouche_r on 5/16/19.
//

#include "generate.hh"
#include "chessBoard.hh"
#include "pieces.hh"
#include <stock.hh>
#include <strings.h>
#include <board.hh>
#include <x86intrin.h>


namespace chessBoard
{

    chessBoard::Board boardM = chessBoard::Board();

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
            king = tab_pos[4];
            casts = tab_pos[8] | tab_pos[1];
            if (m.is_castling_queenside()) {
                rook = tab_pos[8];
                newking = tab_pos[6];
                newrook = tab_pos[5];
            }
            else
            {
                rook = tab_pos[1];
                newking = tab_pos[2];
                newrook = tab_pos[3];
            }
        }
        else
        {
            king = tab_pos[60];
            casts = tab_pos[64] | tab_pos[57];
            if (m.is_castling_queenside()) {
                rook = tab_pos[64];
                newking = tab_pos[62];
                newrook = tab_pos[61];
            }
            else
            {
                rook = tab_pos[57];
                newking = tab_pos[58];
                newrook = tab_pos[59];
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

        hash_board ^= position_value[65 + split_index(casts)];
        hash_board ^= position_value[65 + casts];
        return hash_board;
    }


    Move Board::ia_apply_move(const Move& m, const enumPiece& color_, uint64& hash_board)
    {
        for (int i = nPawn; i <= nKing; ++i)
        {
            if (!(m.from_get() & pieceBB[i]))
                continue;
            std::optional<enumPiece> opponent_piece = std::nullopt;
            for (int j = nPawn; j <= nKing; ++j)
            {
                if (pieceBB[other_color(color_)] & m.to_get() & pieceBB[j])
                {
                    opponent_piece = int_to_enumPiece[j];
                    break;
                }
            }
            bool is_en_passant = false;
            std::optional<enumPiece> promotion = std::nullopt;
            if (m.is_promotion())
                promotion = m.promotion_type_get();
            bool is_castling = false;
            if (i == nKing && !(m.to_get() & king_move[ffsll(m.from_get())]))
            {
                is_castling = true;
            }

            if (i == nPawn)
            {
                if (opponent_piece == std::nullopt && (m.to_get() & white_pawn_attack[ffsll(m.from_get())]))
                {
                    opponent_piece = nPawn;
                    is_en_passant = true;
                }
                else if (opponent_piece == std::nullopt && (m.to_get() & black_pawn_attack[ffsll(m.from_get())]))
                {
                    opponent_piece = nPawn;
                    is_en_passant = true;
                }
            }
            const auto move = Move(m.from_get(), m.to_get(),
                                   int_to_enumPiece[i],
                                   opponent_piece,
                                   promotion,
                                   is_castling, is_en_passant, //en passant
                                   special_moves, castlings, half_move_count_); //special move
            hash_board = this->apply_move(move, color_, hash_board);
            return move;
        }
        return Move();
    }

    uint64 Board::apply_move(const Move& m, const enumPiece& color_, uint64 hash_board)
    {
        half_move_count_++;
        int color_position_offset = 0;
        hash_board ^= side_to_move;
        if (color_ == nBlack) {
            turn_count_++;
            color_position_offset = 6;
        }
        hash_board = hash_board ^ position_value[ffsll(m.from_get()) + (m.piece_get() + color_position_offset) * 65];

        hash_board ^= position_value[ffsl(special_moves)];
        special_moves = 0;
        if (m.is_castling())
        {
            return apply_castling(m, color_, hash_board);
        }
        /// en passant
        else if (m.is_en_passant())
        {
            // FIXME re-generate lists for en passant
            half_move_count_ = 0;
            uint64 pos = m.to_get();
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
        else if (m.is_capture())
        {
            hash_board = hash_board ^ position_value[ffsll(m.to_get()) + (m.captured_piece_type_get() + 6 * other_color(color_)) * 65];
            half_move_count_ = 0;
            enumPiece c = other_color(color_);
            pieceBB[c] = pieceBB[c] ^ m.to_get();
            pieceBB[m.captured_piece_type_get()] =
                    pieceBB[m.captured_piece_type_get()] ^ m.to_get();

            if (m.captured_piece_type_get() == nRook)
            {
                hash_board ^= position_value[65 + ffsll(castlings & m.to_get())];
                castlings = (castlings ^ m.to_get()) & castlings;
                // FIXME is the if needed here ?
            }

        }
        else if (m.piece_get() == nPawn)
        {
            half_move_count_ = 0;
            if (color_ == nWhite)
            {
                if (m.to_get() != m.from_get() << 8ull) {
                    special_moves = m.from_get() << 8ull;
                    hash_board ^= position_value[ffsll(special_moves)];
                }
            }
            else
            {
                if (m.to_get() != m.from_get() >> 8ull) {
                    special_moves = m.from_get() >> 8ull;
                    hash_board ^= position_value[ffsll(special_moves)];
                }
            }
        }
        /// pro;otion
        if (m.is_promotion()) {
            pieceBB[m.promotion_type_get()] =
                    pieceBB[m.promotion_type_get()] | m.to_get();
            hash_board = hash_board ^ position_value[ffsll(m.to_get()) + (m.promotion_type_get() + color_position_offset) * 65];
        }
        else {
            pieceBB[m.piece_get()] = pieceBB[m.piece_get()] | m.to_get();
            hash_board = hash_board ^ position_value[ffsll(m.to_get()) + (m.piece_get() + color_position_offset) * 65];
        }

        if (m.piece_get() == nRook)
        {
            // FIXME is this actually faster than just applying the mask for any piece type ??
            hash_board ^= position_value[65 + ffsll(castlings & m.from_get())];
            castlings = (castlings ^ m.from_get()) & castlings;
        }
        else if (m.piece_get() == nKing)
        {
            if (color_ == nWhite) {
                hash_board ^= position_value[65 + ffsll(tab_pos[1] & castlings)];
                hash_board ^= position_value[65 + ffsll(castlings & tab_pos[8])];
                castlings = ((castlings ^ tab_pos[8]) ^ tab_pos[1]) & castlings;
            }
            else {
                hash_board ^= position_value[65 + ffsll(castlings & tab_pos[57])];
                hash_board ^= position_value[65 + ffsll(tab_pos[64] & castlings)];
                castlings = ((castlings ^ tab_pos[64]) ^ tab_pos[57]) & castlings;
            }
        }
        /// move the piece
        pieceBB[color_] = pieceBB[color_] ^ m.from_get();
        pieceBB[color_] = pieceBB[color_] | m.to_get();
        pieceBB[m.piece_get()] = pieceBB[m.piece_get()] ^ m.from_get();
        return hash_board;
    }

    void Board::revert_castling(const Move &m, const enumPiece& color_)
    {
        /// rook to place back
        uint64 oldrook = 0;
        /// king to remove
        uint64 king = 0;
        /// rook to remove
        uint64 rook = 0;
        /// king to place
        uint64 oldking = 0;

        if (color_ == nWhite)
        {
            oldking = tab_pos[4];
            if (m.is_castling_queenside())
            {
                oldrook = tab_pos[8];
                king = tab_pos[6];
                rook = tab_pos[5];
            }
            else
            {
                oldrook = tab_pos[1];
                king = tab_pos[2];
                rook = tab_pos[3];
            }
        }
        else
        {
            oldking = tab_pos[60];
            if (m.is_castling_queenside())
            {
                oldrook = tab_pos[64];
                king = tab_pos[62];
                rook = tab_pos[61];
            }
            else
            {
                oldrook = tab_pos[57];
                king = tab_pos[58];
                rook = tab_pos[59];
            }
        }
        /// remove king
       // std::cout << std::endl << "oui" << std::endl << int_to_string(pieceBB[nKing]);
        pieceBB[color_] = pieceBB[color_] ^ king;
        pieceBB[nKing] = pieceBB[nKing] ^ king;
        /// remove rook
        pieceBB[color_] = pieceBB[color_] ^ rook;
        pieceBB[nRook] = pieceBB[nRook] ^ rook;
        /// place back king
        pieceBB[color_] = pieceBB[color_] | oldking;
        pieceBB[nKing] = pieceBB[nKing] | oldking;
        /// place back rook
        //std::cout << std::endl << "oui" << std::endl << int_to_string(pieceBB[nKing]);
        pieceBB[color_] = pieceBB[color_] | oldrook;

        pieceBB[nRook] = pieceBB[nRook] | oldrook;

        castlings = m.castlings_get();
        special_moves = m.special_move_get();
    }

    void Board::revert_move(const Move &m, const enumPiece& color_)
    {
        half_move_count_ = m.half_move_get();
        if (color_ == nBlack)
            --turn_count_;
        // FIXME if move from a tower update castlings
        // if castling move both king and tower + revert back castling
        if (m.is_castling())
        {
            revert_castling(m, color_);
            return;
        }
        /// en passant
        else if (m.is_en_passant())
        {
            uint64 pos = m.to_get();
            if (color_ == nWhite) {
                pos >>= 8ull;
                pieceBB[nBlack] = pieceBB[nBlack] | pos;
                pieceBB[nPawn] = pieceBB[nPawn] | pos;
            }
            else
            {
                pos <<= 8ull;
                pieceBB[nWhite] = pieceBB[nWhite] | pos;
                pieceBB[nPawn] = pieceBB[nPawn] | pos;
            }
          //  special_moves = special_moves | m.to_get();
        }
        /// cpature
        else if (m.is_capture())
        {
            enumPiece c = other_color(color_);
            pieceBB[c] = pieceBB[c] | m.to_get();
            if (m.captured_piece_type_get() != m.piece_get())
                pieceBB[m.captured_piece_type_get()] = pieceBB[m.captured_piece_type_get()] | m.to_get();
            else
                pieceBB[m.captured_piece_type_get()] = pieceBB[m.captured_piece_type_get()] ^ m.to_get();
            if (m.captured_piece_type_get() == nRook)
                castlings = m.castlings_get();
        }
        /// pro;otions
        if (m.is_promotion()) {
            if (m.is_capture() && m.promotion_type_get() == m.captured_piece_type_get())
                pieceBB[m.promotion_type_get()] = pieceBB[m.promotion_type_get()] | m.to_get();
            else
                pieceBB[m.promotion_type_get()] = pieceBB[m.promotion_type_get()] ^ m.to_get();
        }
        else
            pieceBB[m.piece_get()] = pieceBB[m.piece_get()] ^ m.to_get();

        if (m.piece_get() == nRook || m.piece_get() == nKing)
            castlings = m.castlings_get();
        // FIXME is if needed here ?

        pieceBB[color_] = pieceBB[color_] ^ m.to_get();
        pieceBB[color_] = pieceBB[color_] | m.from_get();

        pieceBB[m.piece_get()] = pieceBB[m.piece_get()] | m.from_get();
        special_moves = m.special_move_get();
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
        castlings = (castlings | tab_pos[1]);
        castlings = (castlings | tab_pos[8]);
        castlings = (castlings | tab_pos[57]);
        castlings = (castlings | tab_pos[64]);
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

    int uint64_to_index(uint64 pos)
    {
        return ffsll(pos) - 1;
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
        if (m.is_promotion() == move.is_promotion())
        {
            if (m.is_promotion())
                return m.promotion_type_get() == move.promotion_type_get();
            return true;
        }
        return false;
    }

    // Returns true if move is valid (and applied)
    std::optional<Move> Board::check_apply_move(Move& move)
    {
        // FIXME check if our castlings are done the same way as pgn castlings
        // (first are pgn castlings always the same way, if yes is it the king that moves or the rook)

        if (!move.to_get() || !move.from_get())
            return std::nullopt;
        auto& moves = generate_moves(this->color);
        // FIXME set check mat pat
        if (moves.empty())
            return std::nullopt;
        for (auto& m : moves)
        {
            if (m.from_get() == move.from_get() && m.to_get() == move.to_get())
            {
                if (m.is_castling() && !move.is_castling())
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
        int index = ffsll(king);
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
        const int pos_magique_tower = ((tower_move[index] & (pieceBB[0] | pieceBB[1])) * magic_number_tower[index]) >> 52ull;
        if ((tower_output[index][pos_magique_tower] & tower_pos) != 0)
        {
            return true;
        }

        // check for bishop  + queen bishop
        const uint64 bishop_pos = (pieceBB[4] & pieceBB[no_color]) | (pieceBB[6] & pieceBB[no_color]);
        const int pos_magique_bishop = (((pieceBB[0] | pieceBB[1]) & bishop_move[index]) * magic_number_bishop[index]) >> 52ull;
        if ((bishop_output[index][pos_magique_bishop] & bishop_pos) != 0)
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
            uint64 pos, const chessBoard::enumPiece& type_piece) const
    {
        uint64 king = pos;
        int index = ffsll(king);

        uint64 i = 0;

        // Check for knight
        uint64 knight_mask = knight_move[index] & (pieceBB[3] & pieceBB[no_color]);
        while ((i = split_pos(knight_mask)) != 0)
        {
            add_move(no_color, moves,
                     { i, pos, nKnight, type_piece, std::nullopt, false, false,
                       special_moves, castlings, half_move_count_ });
        }
        // check for king
        uint64 king_mask = king_move[index] & (pieceBB[no_color] & pieceBB[7]);
        if ((i = split_pos(king_mask)) != 0)
        {
            add_move(no_color, moves,
                     { i, pos, nKing, type_piece, std::nullopt, false, false,
                       special_moves, castlings, half_move_count_ });
        }
        // check for tower + queen tower
        const uint64 tower_pos = (pieceBB[5] & pieceBB[no_color]);
        const int pos_magique_tower = ((tower_move[index] & (pieceBB[0] | pieceBB[1])) * magic_number_tower[index]) >> 52ull;
        uint64 tower_mask = tower_output[index][pos_magique_tower] & tower_pos;
        while ((i = split_pos(tower_mask)) != 0)
        {
            add_move(no_color, moves,
                     { i, pos, nRook, type_piece, std::nullopt, false, false,
                       special_moves, castlings, half_move_count_ });
        }
        // check for bishop  + queen bishop
        const uint64 bishop_pos = (pieceBB[4] & pieceBB[no_color]);
        const int pos_magique_bishop = (((pieceBB[0] | pieceBB[1]) & bishop_move[index]) * magic_number_bishop[index]) >> 52ull;
        uint64 bishop_mask = bishop_output[index][pos_magique_bishop] & bishop_pos;
        while ((i = split_pos(bishop_mask)) != 0)
        {
            add_move(no_color, moves,
                     { i, pos, nBishop, type_piece, std::nullopt, false, false,
                       special_moves, castlings, half_move_count_ });
        }
        // check queen_tower
        const uint64 queen_tower_pos = (pieceBB[6] & pieceBB[no_color]);
        const int queen_pos_magique_tower = ((tower_move[index] & (pieceBB[0] | pieceBB[1])) * magic_number_tower[index]) >> 52ull;
        uint64 queen_tower_mask = tower_output[index][queen_pos_magique_tower] & queen_tower_pos;
        while ((i = split_pos(queen_tower_mask)) != 0)
        {
            add_move(no_color, moves,
                     { i, pos, nQueen, type_piece, std::nullopt, false, false,
                       special_moves, castlings, half_move_count_ });
        }
        // check queen_bishop
        const uint64 queen_bishop_pos = (pieceBB[6] & pieceBB[no_color]);
        const int queen_pos_magique_bishop = (((pieceBB[0] | pieceBB[1]) & bishop_move[index]) * magic_number_bishop[index]) >> 52ull;
        uint64 queen_bishop_mask = bishop_output[index][queen_pos_magique_bishop] & queen_bishop_pos;
        while ((i = split_pos(queen_bishop_mask)) != 0)
        {
            add_move(no_color, moves,
                     { i, pos, nQueen, type_piece, std::nullopt, false, false,
                       special_moves, castlings, half_move_count_ });
        }

        // check pawn
        if (no_color == nBlack)
        {
            uint64  mask_black_pawn = white_pawn_attack[index] & (pieceBB[2] & pieceBB[no_color]);
            while ((i = split_pos(mask_black_pawn)) != 0)
            {
                if ((index - 1) / 8 == 7)
                {
                    add_move(no_color, moves,
                             { i, pos, nPawn, type_piece, nQueen, false, false,
                               special_moves, castlings, half_move_count_ });
                    add_move(no_color, moves,
                             { i, pos, nPawn, type_piece, nKnight, false, false,
                               special_moves, castlings, half_move_count_ });
                    add_move(no_color, moves,
                             { i, pos, nPawn, type_piece, nBishop, false, false,
                               special_moves, castlings, half_move_count_ });
                    add_move(no_color, moves,
                             { i, pos, nPawn, type_piece, nRook, false, false,
                               special_moves, castlings, half_move_count_ });
                }
                else
                {
                    add_move(no_color, moves,
                             { i, pos, nPawn, type_piece,
                               std::nullopt, false, false,
                               special_moves, castlings, half_move_count_ });
                }
            }
        }
        else
        {
            uint64 mask_white_pawn = black_pawn_attack[index] & (pieceBB[2] & pieceBB[no_color]);
            while ((i = split_pos(mask_white_pawn)) != 0)
            {
                if ((index - 1) / 8 == 0)
                {
                    add_move(no_color, moves,
                             { i, pos, nPawn, type_piece, nQueen, false, false,
                               special_moves, castlings, half_move_count_ });
                    add_move(no_color, moves,
                             { i, pos, nPawn, type_piece, nKnight, false, false,
                               special_moves, castlings, half_move_count_ });
                    add_move(no_color, moves,
                             { i, pos, nPawn, type_piece, nBishop, false, false,
                               special_moves, castlings, half_move_count_ });
                    add_move(no_color, moves,
                             { i, pos, nPawn, type_piece, nRook, false, false,
                               special_moves, castlings, half_move_count_ });
                }
                else
                {
                    add_move(no_color, moves,
                             { i, pos, nPawn, type_piece,
                               std::nullopt, false, false,
                               special_moves, castlings, half_move_count_ });
                }
            }
        }
    }

    bool Board::player_is_check(const enumPiece& color_) const
    {
        uint64 king = pieceBB[7] & pieceBB[color_];
        int index = ffsll(king);
        enumPiece no_color = other_color(color_);

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
        const int pos_magique_tower = ((tower_move[index] & (pieceBB[0] | pieceBB[1])) * magic_number_tower[index]) >> 52ull;
        if ((tower_output[index][pos_magique_tower] & tower_pos) != 0)
        {
            return true;
        }

        // check for bishop  + queen bishop
        const uint64 bishop_pos = (pieceBB[4] & pieceBB[no_color]) | (pieceBB[6] & pieceBB[no_color]);
        const int pos_magique_bishop = (((pieceBB[0] | pieceBB[1]) & bishop_move[index]) * magic_number_bishop[index]) >> 52ull;
        if ((bishop_output[index][pos_magique_bishop] & bishop_pos) != 0)
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
        return move.to_get();
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
            uint64 pos = split_pos(minib);
            while (pos) {
                if (pos & pieceBB[nWhite])
                    board[ffsll(pos) - 1] = epiece_to_char[i];
                else if (pos & pieceBB[nBlack])
                    board[ffsll(pos) - 1] = epiece_to_char[i] + 'a' - 'A';
                pos = split_pos(minib);
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

    /*
    void Board::test_board(std::vector<Move>& vec)
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
    }
    */

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
        if ((castlings & tab_pos[1]) != 0)
        {
            ret.push_back('K');
            tp = 1;
        }
        if ((castlings & tab_pos[8]) != 0)
        {
            ret.push_back('Q');
            tp = 1;
        }
        if ((castlings & tab_pos[57]) != 0)
        {
            ret.push_back('k');
            tp = 1;
        }
        if ((castlings & tab_pos[64]) != 0)
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

    int Board::get_current_pieces_count() const
    {
        return _popcnt64(pieceBB[nWhite] | pieceBB[nBlack]);
    }

    int Board::get_current_captures_count() const
    {
        return generate_capture_moves(this->color).size();
    }

    int Board::get_current_moves_count() const
    {
        return generate_moves(this->color).size();
    }

    Board Board::apply_many_moves(const MOVES_T& moves)
    {
        auto cur_color = this->color;
        uint64 h = 0;
        for (const auto& move : moves)
        {
            this->apply_move(move, cur_color, h);
            cur_color = other_color(cur_color);
        }
        return *this;
    }
}
