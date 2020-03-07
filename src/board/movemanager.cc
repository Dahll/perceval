//
// Created by adrien on 17/05/19.
//

#include "board.hh"
//#include "../magic/generate.hh"
#include "stock.hh"
#include <strings.h>

namespace chessBoard
{

    const MOVES_T Board::generate_moves(const enumPiece& color_) const
    {
        auto vec = std::vector<Move>();
        get_pawn_move(color_, vec);
        get_knight_move(color_,vec);
        get_bishop_move(color_, vec);
        get_tower_move(color_, vec);
        get_queen_move(color_, vec);
        get_king_move(color_, vec);

        return vec;
    }

    void Board::generate_captures(const enumPiece& type,
            const enumPiece& color_, uint64& captures_pos,
            const INDEX_T& index, std::vector<Move>& vec) const
    {
        uint64 j = 0;
        while (captures_pos != 0)
        {
            j = 1ull << (__builtin_ctzll(captures_pos));
            captures_pos &= captures_pos - 1;
            enumPiece enup;
            if ((j & pieceBB[2]) != 0)
            {
                enup = enumPiece::nPawn;
            }
            else if ((j & pieceBB[3]) != 0)
            {
                enup = enumPiece::nKnight;
            }
            else if ((j & pieceBB[4]) != 0)
            {
                enup = enumPiece::nBishop;
            }
            else if ((j & pieceBB[5]) != 0)
            {
                enup = enumPiece::nRook;
            }
            else if ((j & pieceBB[6]) != 0)
            {
                enup = enumPiece::nQueen;
            }
            else
            {
                enup = enumPiece::nKing;
            }
            add_move(color_, vec,
                     { tab_pos[index], j, type, enup, std::nullopt,
                       false, false,
                       special_moves, castlings, half_move_count_ });
        }
    }

    void Board::generate_non_captures(const enumPiece& type,
            const enumPiece& color_, uint64& destination_pos,
            const INDEX_T& index, MOVES_T& vec) const
    {

        uint64 j = 0;
        while (destination_pos != 0)
        {
            j = 1ull << (__builtin_ctzll(destination_pos));
            destination_pos &= destination_pos - 1;
            add_move(color_, vec,
                     { tab_pos[index], j, type, std::nullopt, std::nullopt,
                       false, false,
                       special_moves, castlings, half_move_count_ });
        }

    }


    void Board::generate_captures_casts(const enumPiece& type,
            const enumPiece& color_, uint64& captures_pos,
            const INDEX_T& index, MOVES_T& vec) const
    {
        uint64 j = 0;
        while (captures_pos != 0)
        {
            j = 1ull << (__builtin_ctzll(captures_pos));
            captures_pos &= captures_pos - 1;
            enumPiece enup;
            if ((j & pieceBB[2]) != 0)
            {
                enup = enumPiece::nPawn;
            }
            else if ((j & pieceBB[3]) != 0)
            {
                enup = enumPiece::nKnight;
            }
            else if ((j & pieceBB[4]) != 0)
            {
                enup = enumPiece::nBishop;
            }
            else if ((j & pieceBB[5]) != 0)
            {
                enup = enumPiece::nRook;
            }
            else if ((j & pieceBB[6]) != 0)
            {
                enup = enumPiece::nQueen;
            }
            else
            {
                enup = enumPiece::nKing;
            }

            const auto move = Move(tab_pos[index], j,
                                   type,
                                   enup,
                                   std::nullopt,
                                   false, false,
                                   special_moves,
                                   castlings,
                                   half_move_count_);
            //this->apply_move(move, color_);
            //bool ret = this->player_is_check(color_);
            //this->revert_move(move, color_);
            uint64 h = 0;
            auto b = this->copy_board();
            b.apply_move(move, color_, h);
            bool ret = b.player_is_check(color_);
            if (!ret)
                vec.emplace_back(move);
        }
    }

    void Board::generate_non_captures_casts(const enumPiece& type,
            const enumPiece& color_, uint64& destination_pos,
            const INDEX_T& index, std::vector<Move>& vec) const
    {
        uint64 j = 0;
        while (destination_pos != 0)
        {
            j = 1ull << (__builtin_ctzll(destination_pos));
            destination_pos &= destination_pos - 1;
            const auto move = Move(tab_pos[index], j,
                                   type,
                                   std::nullopt,
                                   std::nullopt,
                                   false, false,
                                   special_moves,
                                   castlings,
                                   half_move_count_);
            //this->apply_move(move, color_);
            //bool ret = player_is_check(color_);
            //this->revert_move(move, color_);

            auto b = this->copy_board();
            uint64 h = 0;
            b.apply_move(move, color_, h);
            bool ret = b.player_is_check(color_);
            if (!ret)
                vec.emplace_back(move);
        }

    }

    void Board::get_knight_move(const enumPiece& color_, std::vector<Move>& vec) const
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
            generate_captures(nKnight, color_, temp_cap, i, vec);
            //move normal
            generate_non_captures(nKnight, color_, temp_norm, i, vec);
        }
    }

    void Board::get_tower_move(const enumPiece& color_, std::vector<Move>& vec) const
    {
        uint64 mask = pieceBB[5] & pieceBB[color_];
        const uint64 occ = pieceBB[0] | pieceBB[1];
        int i = 0;
        while (mask != 0) {
            i = __builtin_ctzll(mask);
            mask &= mask - 1;
            //const uint64 &relevant_mask = tower_move[i] & (pieceBB[0] | pieceBB[1]);
            //const uint64 &index = (magic_number_tower[i] * relevant_mask) >> 52ull;
            //uint64 temp = tower_output[i][index];
            uint64 temp = rookAttacks(occ, i);
            temp = (temp & pieceBB[color_]) ^ temp;
            //print();
            //std::cout << std::endl << int_to_string(temp) << std::endl;
            //          std::cout << i << std::endl << int_to_string(relevant_mask)<< std::endl <<index << std::endl << int_to_string(temp) << std::endl;
            const enumPiece notcolor = other_color(color_);
            uint64 temp_cap = temp & pieceBB[notcolor];
            uint64 temp_norm = temp & (~pieceBB[notcolor]);

            //move capture
            generate_captures_casts(nRook, color_, temp_cap, i, vec);
            //move normal
            generate_non_captures_casts(nRook, color_, temp_norm, i, vec);
        }
    }

    void Board::get_bishop_move(const enumPiece& color_, std::vector<Move>& vec) const
    {
        uint64 mask = pieceBB[4] & pieceBB[color_];
        const uint64 occ = pieceBB[0] | pieceBB[1];
        int i = 0;
        //std::cout << std::endl << int_to_string(mask) << std::endl;
        while (mask != 0)
        {
            i = __builtin_ctzll(mask);
            mask &= mask - 1;
            //const uint64& relevant_mask = bishop_move[i] & (pieceBB[0] | pieceBB[1]);
            //const uint64& index = (magic_number_bishop[i] * relevant_mask) >> 52ull;
            //uint64 temp = bishop_output[i][index];
            uint64 temp = bishopAttacks(occ, i);
            //std::cout << std::endl << int_to_string(temp) << std::endl <<i << std::endl << index << std::endl;
            temp = (temp & pieceBB[color_]) ^ temp;
            //std::cout << std::endl << int_to_string(relevant_mask) << std::endl << relevant_mask << std::endl;
            //std::cout << std::endl << int_to_string(temp) << std::endl << temp << std::endl;

            const enumPiece notcolor = other_color(color_);
            uint64 temp_cap = temp & pieceBB[notcolor];
            uint64 temp_norm = temp & (~pieceBB[notcolor]);

            //move capture
            generate_captures(nBishop, color_, temp_cap, i, vec);
            //move normal
            generate_non_captures(nBishop, color_, temp_norm, i, vec);
        }

    }

    void Board::gen_castlings_move(const enumPiece& color_, uint64 destination_pos,
            POSITION_T start, std::vector<Move>& vec) const
    {
        add_move(color_, vec,
                 { start, destination_pos, nKing, std::nullopt, std::nullopt,
                   true, false,
                   special_moves, castlings, half_move_count_ });
         //   std::cout << std::endl << "castling" << std::endl << int_to_string(destination_pos);
    }

    void Board::generate_castlings(const enumPiece& color_,
            std::vector<Move>& vec) const
    {

        if (color_ == nWhite)
        {
            if (castlings & tab_pos[7])
            {
                if (!((pieceBB[1] | pieceBB[0]) & (tab_pos[6] | (tab_pos[5] | tab_pos[4]))))
                    if (!square_is_check(color_, tab_pos[4]))
                        gen_castlings_move(color_, tab_pos[5], tab_pos[3], vec);
            }
            if (castlings & tab_pos[0])
            {
                if (!((pieceBB[1] | pieceBB[0]) & (tab_pos[2] | tab_pos[1]))) {
                    if (!square_is_check(color_, tab_pos[2]))
                        gen_castlings_move(color_, tab_pos[1], tab_pos[3], vec);
                }
            }
        }
        else
        {
            if (castlings & tab_pos[63])
            {
                if (!((pieceBB[1] | pieceBB[0]) & (tab_pos[62] | (tab_pos[61] | tab_pos[60])))) {
                    if (!square_is_check(color_, tab_pos[60]))
                        gen_castlings_move(color_, tab_pos[61], tab_pos[59], vec);
                }
            }
            if (castlings & tab_pos[56])
            {
                if (!((pieceBB[1] | pieceBB[0]) & (tab_pos[58] | tab_pos[57]))) {
                    if (!square_is_check(color_, tab_pos[58]))
                        gen_castlings_move(color_, tab_pos[57], tab_pos[59], vec);
                }
            }
        }

    }

    void Board::get_king_move(const enumPiece& color_, std::vector<Move>& vec) const
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
        generate_captures_casts(nKing, color_, temp_cap, i, vec);
        //move normal
        generate_non_captures_casts(nKing, color_, temp_norm, i, vec);
        if (!player_is_check(color_))
            generate_castlings(color_, vec);
    }

    void Board::get_queen_move(const enumPiece& color_, std::vector<Move>& vec) const
    {
        uint64 queen = pieceBB[6] & pieceBB[color_];
        const uint64 occ = pieceBB[0] | pieceBB[1];
        int i = 0;
        while (queen != 0) {
            i = __builtin_ctzll(queen);
            queen &= queen - 1;
            /*const uint64 &relevant_diagonals = bishop_move[i] & (pieceBB[0] | pieceBB[1]);
            const uint64 &index_diagonals = (magic_number_bishop[i] * relevant_diagonals) >> 52ull;

            const uint64 &relevant_lignes = tower_move[i] & (pieceBB[0] | pieceBB[1]);
            const uint64 &index_lignes = (magic_number_tower[i] * relevant_lignes) >> 52ull;

            uint64 temp = tower_output[i][index_lignes] | bishop_output[i][index_diagonals];*/
            //std::cout << std::endl << int_to_string(relevant_lignes) << std::endl;

            uint64 temp = rookAttacks(occ, i) | bishopAttacks(occ, i);

            temp = (temp & pieceBB[color_]) ^ temp;
            //std::cout << std::endl << int_to_string(temp) << std::endl;
            const enumPiece notcolor = other_color(color_);
            uint64 temp_cap = temp & pieceBB[notcolor];
            uint64 temp_norm = temp & (~pieceBB[notcolor]);


            generate_captures(nQueen, color_, temp_cap, i, vec);
            //move normal
            generate_non_captures(nQueen, color_, temp_norm, i, vec);
        }
    }


    void Board::add_move(const enumPiece& color_, MOVES_T& vec,
            const Move& move) const
            /*const uint64& from, const uint64& to, const enumPiece& type,
            const std::optional<enumPiece>& capture,
            const std::optional<enumPiece>& promotion,
            const bool& is_castling, const bool& is_en_passant,
            const uint64& report) const*/
    {
        /*const auto move = Move(from, to,
                           type,
                           capture,
                           promotion,
                           is_castling, is_en_passant,
                           report, castlings, half_move_count_);*/
     //   if (to_perft() == "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/P2P1RPP/n2Q2K1 w kq - 0 0\n" || to_perft() == "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/P2P1RPP/b2Q2K1 w kq - 0 0\n")
       //     std::cout << "pute" << std::endl;
        //this->apply_move(move, color_);
        //bool ret = this->player_is_check(color_);
        //this->revert_move(move, color_);

        auto b = this->copy_board();
        uint64 h = 0;
        b.apply_move(move, color_, h);
        bool ret = b.player_is_check(color_);
        if (!ret)
            vec.emplace_back(move);
    }

    void Board::generate_attack_move_pawn(const enumPiece &color_,
            std::vector<Move> &vec, int index, const uint64 (&attack)[64]) const
    {
        //normal
        uint64 mask = attack[index] & pieceBB[other_color(color_)];
        bool is_promotion = pawn_will_promote(index, color_);
        uint64 i = 0;
        while (mask != 0)
        {
            i = 1ull << (__builtin_ctzll(mask));
            mask &= mask - 1;
            enumPiece enup;
            if ((i & pieceBB[nPawn]) != 0)
            {
                enup = enumPiece::nPawn;
            }
            else if ((i & pieceBB[nKnight]) != 0)
            {
                enup = enumPiece::nKnight;
            }
            else if ((i & pieceBB[nBishop]) != 0)
            {
                enup = enumPiece::nBishop;
            }
            else if ((i & pieceBB[nRook]) != 0)
            {
                enup = enumPiece::nRook;
            }
            else if ((i & pieceBB[nQueen]) != 0)
            {
                enup = enumPiece::nQueen;
            }
            else
            {
                enup = enumPiece::nKing;
            }


            if (is_promotion)
            {
                add_move(color_, vec,
                        { tab_pos[index], i, nPawn, enup, nQueen, false, false,
                          special_moves, castlings, half_move_count_ });
                add_move(color_, vec,
                        { tab_pos[index], i, nPawn, enup, nRook, false, false,
                          special_moves, castlings, half_move_count_ });
                add_move(color_, vec,
                        { tab_pos[index], i, nPawn, enup, nKnight, false, false,
                          special_moves, castlings, half_move_count_});
                add_move(color_, vec,
                        { tab_pos[index], i, nPawn, enup, nBishop, false, false,
                          special_moves, castlings, half_move_count_});
            }
            else
            {
                add_move(color_, vec,
                         { tab_pos[index], i, nPawn, enup, std::nullopt,
                           false, false,
                           special_moves, castlings, half_move_count_});
            }
        }

        //en passant
        uint64 oui_passant = tab_pos[index]; // pos du pion
        oui_passant = (oui_passant << 1ull) | (oui_passant >> 1ull); // les cases juste a cote, la ou se trouve le possible pion mange
        oui_passant = oui_passant & (pieceBB[other_color(color_)] & pieceBB[nPawn]); // on enleve les endroit sans pions adverse
       // oui_passant = oui_passant; // on garde ceux ou le en passant est possible
        if (!oui_passant)
            return;
        uint64 mask_en_passant = attack[index] & special_moves; // on compare avec les attaques possible
//        if (index == 31 && special_moves) {
           //std::cout << "hello" << std::endl << int_to_string(special_moves) << std::endl;
           //std::cout << int_to_string(tab_pos[index]) << std::endl << int_to_string(oui_passant) << std::endl;
        //}
        uint64 j = 0;
        while (mask_en_passant != 0)
        {
            j = 1ull << (__builtin_ctzll(mask_en_passant));
            mask_en_passant &= mask_en_passant - 1;
            add_move(color_,vec,
                     { tab_pos[index], j, nPawn, nPawn, std::nullopt,
                       false, true,
                       special_moves, castlings, half_move_count_ });
       //     std::cout << "12" << std::endl;
        }

    }

    void Board::generate_classic_move_pawn(const enumPiece& color_,
            std::vector<Move>& vec, const int& index,
            const uint64 (&normal)[64], const uint64 (&jump)[64]) const
    {
        const uint64 mask = normal[index] & (~(pieceBB[0] | pieceBB[1]));
        if (mask != 0)
        {
            bool is_promotion = pawn_will_promote(index, color_);
            if (is_promotion)
            {
                add_move(color_, vec,
                         { tab_pos[index], mask, nPawn, std::nullopt, nQueen,
                           false, false,
                           special_moves, castlings, half_move_count_ });
                add_move(color_, vec,
                         { tab_pos[index], mask, nPawn, std::nullopt, nRook,
                           false, false,
                           special_moves, castlings, half_move_count_ });
                add_move(color_, vec,
                         { tab_pos[index], mask, nPawn, std::nullopt, nBishop,
                           false, false,
                           special_moves, castlings, half_move_count_ });
                add_move(color_, vec,
                         { tab_pos[index], mask, nPawn, std::nullopt, nKnight,
                           false, false,
                           special_moves, castlings, half_move_count_ });
            }
            else
            {
                add_move(color_, vec,
                         { tab_pos[index], mask, nPawn,
                           std::nullopt, std::nullopt, false, false,
                           special_moves, castlings, half_move_count_ });
            }
            const uint64 mask_jump = jump[index] & (~(pieceBB[0] | pieceBB[1]));
            if (mask_jump != 0)
            {
                add_move(color_, vec,
                         { tab_pos[index], mask_jump, nPawn,
                           std::nullopt, std::nullopt, false, false,
                           special_moves, castlings, half_move_count_ });
            }
        }

    }


    void Board::generate_pawn_move(const enumPiece &color_, std::vector<Move> &vec,
            uint64& mask, const uint64 (&attack)[64],
            const uint64 (&normal)[64], const uint64 (&jump)[64]) const
    {
        int i = 0;
        //std::cout << std::endl << int_to_string(mask) << std::endl;
        while (mask != 0)
        {
            i = __builtin_ctzll(mask);
            mask &= mask - 1;
            generate_attack_move_pawn(color_, vec, i, attack);
            generate_classic_move_pawn(color_, vec, i, normal, jump);
        }
    }

    void Board::get_pawn_move(const enumPiece &color_, std::vector<Move> &vec) const
    {
        uint64 mask = pieceBB[2] & pieceBB[color_];
        if (color_ == nWhite)
            generate_pawn_move(color_, vec, mask, white_pawn_attack, white_pawn_normal_move, white_pawn_jump);
        else
            generate_pawn_move(color_, vec, mask, black_pawn_attack, black_pawn_normal_move, black_pawn_jump);
    }

    void Board::get_all_move_square_check(std::vector<chessBoard::Move>& moves,
            const enumPiece& color_, Board& board) const
    {
        uint64 i = 0;

        // check for king
        uint64 king_mask = board.pieceBB[color_] & board.pieceBB[7];
        if (king_mask != 0)
        {
            i = 1ull << (__builtin_ctzll(king_mask));
            board.move_attack_square(moves, color_, i, nKing);
        }
        // Check for knight
        uint64 knight_mask = board.pieceBB[3] & board.pieceBB[color_];
        while (knight_mask != 0)
        {
            i = 1ull << (__builtin_ctzll(knight_mask));
            knight_mask &= knight_mask - 1;
            board.move_attack_square(moves, color_, i, nKnight);
        }
        // check for tower
        uint64 tower_pos = (board.pieceBB[5] & board.pieceBB[color_]);
        while (tower_pos != 0)
        {
            i = 1ull << (__builtin_ctzll(tower_pos));
            tower_pos &= tower_pos - 1;
            board.move_attack_square(moves, color_, i, nRook);
        }
        // check for bishop
        uint64 bishop_pos = (board.pieceBB[4] & board.pieceBB[color_]);
        while (bishop_pos != 0)
        {
            i = 1ull << (__builtin_ctzll(bishop_pos));
            bishop_pos &= bishop_pos - 1;
            board.move_attack_square(moves, color_, i, nBishop);
        }
        // check queen
        uint64 queen_tower_pos = (board.pieceBB[6] & board.pieceBB[color_]);
        while (queen_tower_pos != 0)
        {
            i = 1ull << (__builtin_ctzll(queen_tower_pos));
            queen_tower_pos &= queen_tower_pos - 1;
            board.move_attack_square(moves, color_, i, nQueen);
        }
        // check pawn

        uint64 mask_black_pawn = (board.pieceBB[2] & board.pieceBB[color_]);
        while (mask_black_pawn != 0)
        {
            i = 1ull << (__builtin_ctzll(mask_black_pawn));
            mask_black_pawn &= mask_black_pawn - 1;
            board.move_attack_square(moves, color_, i, nPawn);
        }
    }

    std::vector<Move> remove_non_capture_move(std::vector<Move>& vec)
    {
        auto ret = std::vector<Move>();
        for (auto &move : vec)
        {
            if (move.is_capture())
                ret.push_back(move);
        }
        return ret;
    }
}

