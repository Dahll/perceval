//
// Created by adrien on 17/05/19.
//

#include "board.hh"
//#include "../magic/generate.hh"
#include "stock.hh"
#include <strings.h>

namespace chessBoard
{

    void Board::generate_moves(const enumPiece& color_ ,VMove& vec) const
    {
        //auto vec = std::vector<Move>();
	    //vec.reserve(60);
        get_pawn_move(color_, vec);
        get_knight_move(color_,vec);
        get_bishop_move(color_, vec);
        get_tower_move(color_, vec);
        get_queen_move(color_, vec);
        get_king_move(color_, vec);

        //return vec;
    }

    void Board::generate_captures(
            const enumPiece& color_, uint64& captures_pos,
            const INDEX_T& index, VMove& vec) const
    {
        uint64 j = 0;
        while (captures_pos != 0)
        {
            j = __builtin_ctzll(captures_pos);
            captures_pos &= captures_pos - 1;


            add_move(color_, vec,
                     { index, static_cast<uint16>(j), 4});
        }
    }

    void Board::generate_non_captures(
            const enumPiece& color_, uint64& destination_pos,
            const INDEX_T& index, VMove& vec) const
    {

        uint64 j = 0;
        while (destination_pos != 0)
        {
            j = __builtin_ctzll(destination_pos);
            destination_pos &= destination_pos - 1;

            add_move(color_, vec,
                     { index, static_cast<uint16>(j), 0});
        }

    }

    void Board::get_knight_move(const enumPiece& color_, VMove& vec) const
    {
        uint64 mask = pieceBB[3] & pieceBB[color_];
        int i = 0;
        while (mask != 0)
        {
            i = __builtin_ctzll(mask);
            mask &= mask - 1;
            const uint64& move = knight_move[i];
            const enumPiece notcolor = other_color(color_);
            const uint64 temp = ((move ^ pieceBB[color_]) & move);
         //   std::cout << std::endl <<  i << std::endl << int_to_string(temp) << std::endl;
            uint64 temp_cap = temp & pieceBB[notcolor];
            uint64 temp_norm = temp & (~pieceBB[notcolor]);

            //move capture
            generate_captures(color_, temp_cap, i, vec);
            //move normal
            generate_non_captures(color_, temp_norm, i, vec);
        }
    }

    void Board::get_tower_move(const enumPiece& color_, VMove& vec) const
    {
        uint64 mask = pieceBB[5] & pieceBB[color_];
        const uint64 occ = pieceBB[0] | pieceBB[1];
        int i = 0;
        while (mask != 0) {
            i = __builtin_ctzll(mask);
            mask &= mask - 1;
            uint64 temp = rookAttacks(occ, i);
            temp = (temp & pieceBB[color_]) ^ temp;
            const enumPiece notcolor = other_color(color_);
            uint64 temp_cap = temp & pieceBB[notcolor];
            uint64 temp_norm = temp & (~pieceBB[notcolor]);

            //move capture
            generate_captures(color_, temp_cap, i, vec);
            //move normal
            generate_non_captures(color_, temp_norm, i, vec);
        }
    }

    void Board::get_bishop_move(const enumPiece& color_, VMove& vec) const
    {
        uint64 mask = pieceBB[4] & pieceBB[color_];
        const uint64 occ = pieceBB[0] | pieceBB[1];
        int i = 0;
        //std::cout << std::endl << int_to_string(mask) << std::endl;
        while (mask != 0)
        {
            i = __builtin_ctzll(mask);
            mask &= mask - 1;
            uint64 temp = bishopAttacks(occ, i);
            temp = (temp & pieceBB[color_]) ^ temp;

            const enumPiece notcolor = other_color(color_);
            uint64 temp_cap = temp & pieceBB[notcolor];
            uint64 temp_norm = temp & (~pieceBB[notcolor]);

            //move capture
            generate_captures(color_, temp_cap, i, vec);
            //move normal
            generate_non_captures(color_, temp_norm, i, vec);
        }

    }

    void Board::generate_castlings(const enumPiece& color_,
            VMove& vec) const
    {

        if (color_ == nWhite)
        {
            if (castlings & tab_pos[7])
            {
                if (!((pieceBB[1] | pieceBB[0]) & (tab_pos[6] | (tab_pos[5] | tab_pos[4]))))
                    if (!square_is_check(color_, tab_pos[4]))
                        add_move(color_, vec, {3, 5, 3});
                        //gen_castlings_move(color_, tab_pos[5], tab_pos[3], vec);

            }
            if (castlings & tab_pos[0])
            {
                if (!((pieceBB[1] | pieceBB[0]) & (tab_pos[2] | tab_pos[1]))) {
                    if (!square_is_check(color_, tab_pos[2]))
                        add_move(color_, vec, {3, 1, 2});
                        //gen_castlings_move(color_, tab_pos[1], tab_pos[3], vec);
                }
            }
        }
        else
        {
            if (castlings & tab_pos[63])
            {
                if (!((pieceBB[1] | pieceBB[0]) & (tab_pos[62] | (tab_pos[61] | tab_pos[60])))) {
                    if (!square_is_check(color_, tab_pos[60]))
                        add_move(color_, vec, {59, 61, 3});
                        //gen_castlings_move(color_, tab_pos[61], tab_pos[59], vec);
                }
            }
            if (castlings & tab_pos[56])
            {
                if (!((pieceBB[1] | pieceBB[0]) & (tab_pos[58] | tab_pos[57]))) {
                    if (!square_is_check(color_, tab_pos[58]))
                        add_move(color_, vec, {59, 57, 2});
                        //gen_castlings_move(color_, tab_pos[57], tab_pos[59], vec);
                }
            }
        }

    }

    void Board::get_king_move(const enumPiece& color_, VMove& vec) const
    {
        // FIXME castlings
        uint64 king = pieceBB[7] & pieceBB[color_];
        int i = __builtin_ctzll(king);

        const uint64& move = king_move[i];
        const enumPiece notcolor = other_color(color_);
        const uint64 temp = ((move ^ pieceBB[color_]) & move);
        uint64 temp_cap = temp & pieceBB[notcolor];
        uint64 temp_norm = (temp & (~pieceBB[notcolor]));

        //move capture
        generate_captures(color_, temp_cap, i, vec);
        //move normal
        generate_non_captures(color_, temp_norm, i, vec);
        if (!player_is_check(color_))
            generate_castlings(color_, vec);
    }

    void Board::get_queen_move(const enumPiece& color_, VMove& vec) const
    {
        uint64 queen = pieceBB[6] & pieceBB[color_];
        const uint64 occ = pieceBB[0] | pieceBB[1];
        int i = 0;
        while (queen != 0) {
            i = __builtin_ctzll(queen);
            queen &= queen - 1;

            uint64 temp = rookAttacks(occ, i) | bishopAttacks(occ, i);

            temp = (temp & pieceBB[color_]) ^ temp;
            const enumPiece notcolor = other_color(color_);
            uint64 temp_cap = temp & pieceBB[notcolor];
            uint64 temp_norm = temp & (~pieceBB[notcolor]);

            generate_captures(color_, temp_cap, i, vec);
            generate_non_captures(color_, temp_norm, i, vec);
        }
    }


    void Board::add_move(const enumPiece& color_, VMove& vec,
            const Move& move) const
            /*const uint64& from, const uint64& to, const enumPiece& type,
            const std::optional<enumPiece>& capture,
            const std::optional<enumPiece>& promotion,
            const bool& is_castling, const bool& is_en_passant,
            const uint64& report) const*/
    {
        auto b = this->copy_board();
        uint64 h = 0;
        b.apply_move(move, color_, h);
        bool ret = b.player_is_check(color_);
        if (!ret)
            vec.add_move(move);
    }

    void Board::generate_attack_move_pawn(const enumPiece &color_,
            VMove& vec, uint16 index, const uint64 (&attack)[64]) const
    {
        //normal
        uint64 mask = attack[index] & pieceBB[other_color(color_)];
        bool is_promotion = pawn_will_promote(index, color_);
        uint16 i = 0;
        while (mask != 0)
        {
            i = static_cast<uint16 >(__builtin_ctzll(mask));
            mask &= mask - 1;
            if (is_promotion)
            {
                add_move(color_, vec,
                        { index, i, 15});
                add_move(color_, vec,
                         { index, i, 14});
                add_move(color_, vec,
                         { index, i, 13});
                add_move(color_, vec,
                         { index, i, 12});
            }
            else
            {
                add_move(color_, vec,
                         { index, i, 4});
            }
        }

        //en passant
        uint64 oui_passant = tab_pos[index]; // pos du pion
        oui_passant = (oui_passant << 1ull) | (oui_passant >> 1ull); // les cases juste a cote, la ou se trouve le possible pion mange
        oui_passant = oui_passant & (pieceBB[other_color(color_)] & pieceBB[nPawn]); // on enleve les endroit sans pions adverse
        if (!oui_passant)
            return;
        uint64 mask_en_passant = attack[index] & special_moves; // on compare avec les attaques possible
        uint64 j = 0;
        while (mask_en_passant != 0)
        {
            j = __builtin_ctzll(mask_en_passant);
            mask_en_passant &= mask_en_passant - 1;
            add_move(color_,vec,
                     { index, static_cast<uint16 >(j), 5});
        }

    }

    void Board::generate_classic_move_pawn(const enumPiece& color_,
            VMove& vec, const uint16 & index,
            const uint64 (&normal)[64], const uint64 (&jump)[64]) const
    {
        const uint64 mask = normal[index] & (~(pieceBB[0] | pieceBB[1]));
        if (mask != 0)
        {
            auto i = static_cast<uint16 >(__builtin_ctzll(mask));
            bool is_promotion = pawn_will_promote(index, color_);
            if (is_promotion)
            {
                add_move(color_, vec,
                         { index, i, 11});
                add_move(color_, vec,
                         { index, i, 10});
                add_move(color_, vec,
                         { index, i, 9});
                add_move(color_, vec,
                         { index, i, 8});
            }
            else
            {
                add_move(color_, vec,
                         { index, i, 0});

            }
            const uint64 mask_jump = jump[index] & (~(pieceBB[0] | pieceBB[1]));
            if (mask_jump != 0)
            {
                add_move(color_, vec,
                         { index, static_cast<uint16 >(__builtin_ctzll(mask_jump)), 1});

            }
        }

    }


    void Board::generate_pawn_move(const enumPiece &color_, VMove& vec,
            uint64& mask, const uint64 (&attack)[64],
            const uint64 (&normal)[64], const uint64 (&jump)[64]) const
    {
        uint16 i = 0;
        //std::cout << std::endl << int_to_string(mask) << std::endl;
        while (mask != 0)
        {
            i = __builtin_ctzll(mask);
            mask &= mask - 1;
            generate_attack_move_pawn(color_, vec, i, attack);
            generate_classic_move_pawn(color_, vec, i, normal, jump);
        }
    }

    void Board::get_pawn_move(const enumPiece &color_, VMove& vec) const
    {
        uint64 mask = pieceBB[2] & pieceBB[color_];
        if (color_ == nWhite)
            generate_pawn_move(color_, vec, mask, white_pawn_attack, white_pawn_normal_move, white_pawn_jump);
        else
            generate_pawn_move(color_, vec, mask, black_pawn_attack, black_pawn_normal_move, black_pawn_jump);
    }

    void Board::get_all_move_square_check(VMove& moves,
            const enumPiece& color_, Board& board) const
    {
        uint64 i = 0;

        // check for king
        uint64 king_mask = board.pieceBB[color_] & board.pieceBB[7];
        if (king_mask != 0)
        {
            i = 1ull << (__builtin_ctzll(king_mask));
            board.move_attack_square(moves, color_, i);
        }
        // Check for knight
        uint64 knight_mask = board.pieceBB[3] & board.pieceBB[color_];
        while (knight_mask != 0)
        {
            i = 1ull << (__builtin_ctzll(knight_mask));
            knight_mask &= knight_mask - 1;
            board.move_attack_square(moves, color_, i);
        }
        // check for tower
        uint64 tower_pos = (board.pieceBB[5] & board.pieceBB[color_]);
        while (tower_pos != 0)
        {
            i = 1ull << (__builtin_ctzll(tower_pos));
            tower_pos &= tower_pos - 1;
            board.move_attack_square(moves, color_, i);
        }
        // check for bishop
        uint64 bishop_pos = (board.pieceBB[4] & board.pieceBB[color_]);
        while (bishop_pos != 0)
        {
            i = 1ull << (__builtin_ctzll(bishop_pos));
            bishop_pos &= bishop_pos - 1;
            board.move_attack_square(moves, color_, i);
        }
        // check queen
        uint64 queen_tower_pos = (board.pieceBB[6] & board.pieceBB[color_]);
        while (queen_tower_pos != 0)
        {
            i = 1ull << (__builtin_ctzll(queen_tower_pos));
            queen_tower_pos &= queen_tower_pos - 1;
            board.move_attack_square(moves, color_, i);
        }
        // check pawn

        uint64 mask_black_pawn = (board.pieceBB[2] & board.pieceBB[color_]);
        while (mask_black_pawn != 0)
        {
            i = 1ull << (__builtin_ctzll(mask_black_pawn));
            mask_black_pawn &= mask_black_pawn - 1;
            board.move_attack_square(moves, color_, i);
        }
    }

}

