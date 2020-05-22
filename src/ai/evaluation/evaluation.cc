//
// Created by adrien on 30/05/19.
//

#include "evaluation.hh"
namespace ai::evaluation
{
    namespace
    {
        using chessBoard::enumPiece;
    }
    uint32 values[2][8][64] = {{{0}}};

    int calc_phase(const chessBoard::Board& b)
    {
        int KnightPhase = 1;
        int BishopPhase = 1;
        int RookPhase = 2;
        int QueenPhase = 4;
        int TotalPhase = KnightPhase*4 + BishopPhase*4 + RookPhase*4 + QueenPhase*2;

        int phase = TotalPhase;

        phase -= __builtin_popcountll(b.pieceBB[3]) * KnightPhase;
        phase -= __builtin_popcountll(b.pieceBB[4]) * BishopPhase;
        phase -= __builtin_popcountll(b.pieceBB[5]) * RookPhase;
        phase -= __builtin_popcountll(b.pieceBB[6]) * QueenPhase;

        phase = (phase * 256 + (TotalPhase / 2)) / TotalPhase;
        return phase;
    }


    void init_values()
    {
        for (int i = 2; i < 8; i++)
        {
            int piece_val_mg = pieces_values_mg[i - 2];
            int piece_val_eg = pieces_values_eg[i - 2];
            for (int j = 0; j < 64; j++)
            {
                int psqt_val_mg = psqt_values_mg[i - 2][j];
                int psqt_val_eg = psqt_values_eg[i - 2][j];
                //std::cout << psqt_val_mg << std::endl;
                //std::cout << psqt_val_eg << std::endl;
                int mg = piece_val_mg + psqt_val_mg;
                int eg = piece_val_eg + psqt_val_eg;
                uint32 score = (((static_cast<uint32>(eg)) << 16u) | (static_cast<uint32>(mg) & 0x0000FFFFu));
                //std::cout << score << std::endl;
                values[0][i][j] = score;
                values[1][i][(7-(j / 8)) * 8 + j % 8] = score;
            }
        }
    }

    void set_positional_values(const chessBoard::Board& b, const chessBoard::enumPiece& color,
            const chessBoard::enumPiece& not_color, MG& mg, EG& eg)
    {
        for (int i = 2; i < 8; i++)
        {
            uint64 mask = b.pieceBB[i] & b.pieceBB[color];
            uint8 index = 0;
            while (mask != 0)
            {
                index = __builtin_ctzll(mask);
                mask &= mask - 1;
                uint32 tmp = values[color][i][index];
                mg.positional_value += (tmp & 0x0000FFFFu);
                eg.positional_value += (tmp >> 16u);
            }
            uint64 mask_no = b.pieceBB[i] & b.pieceBB[not_color];
            while (mask_no != 0)
            {
                index = __builtin_ctzll(mask_no);
                mask_no &= mask_no - 1;
                uint32 tmp = values[not_color][i][index];
                mg.positional_value -= (tmp & 0x0000FFFFu);
                eg.positional_value -= (tmp >> 16u);
            }
        }
    }

    void set_eval_values(const chessBoard::Board& b, const chessBoard::enumPiece& color,
            const chessBoard::enumPiece& not_color, MG& mg, EG& eg)
    {
        set_positional_values(b, color, not_color, mg, eg);
    }

    int evaluate(const chessBoard::Board& b, const chessBoard::enumPiece& color_)
    {
        const chessBoard::enumPiece& not_color = b.other_color(color_);
        MG mg = MG();
        EG eg = EG();
        set_eval_values(b, color_, not_color, mg, eg);
        int ret = 0;
        int mid = middle_game_evaluation(mg);
        int end = end_game_evaluation(eg);
        int phase = calc_phase(b);
        ret = ((mid * (256 - phase)) + (end * phase)) / 256;
        return ret;
    }
}
