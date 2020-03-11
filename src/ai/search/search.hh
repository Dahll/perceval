//
// Created by fouche_r on 5/22/19.
//

#pragma once


#include <chrono>
#include <vector>
#include <algorithm>

#include "../transposition_table.hh"
#include "../evaluation/evaluation.hh"
#include "../../perft/perft.hh"
#include "stock.hh"
#include "../helpers/helper.hh"
#include "../uci.hh"
#include "../../board/board.hh"
#include "../metadata.hh"


using namespace std::chrono;
using namespace chessBoard;

namespace ai::search
{

    struct PV
    {
        uint8 length = 0;
        Move pv[256];
    };


    enum Bound : int {
        INF = INT32_MAX - 1,
    };

    enum Value : int {
        MAT = 3000000,
        MIN_MAT = 2999000
    };
    

    std::string PV_to_str(const PV& pv);
    void updatePV(Move best_move, PV& parent, PV& child);
    void mergePV(PV& from, PV& to);

    int alphabeta(const chessBoard::Board& b, const chessBoard::enumPiece& colo_act, int depth, int ply, int alpha,
                int beta, const chessBoard::Move& prev_move,
                PV& parent_PV,
                uint64 hash);

    chessBoard::Move caller_alphabeta(const chessBoard::Board& b, int depth, PV& parent_PV, uint64 hash, int alpha, int beta, int& score);

    int quiesce(const chessBoard::Board& b, const chessBoard::enumPiece& color_act, int alpha, int beta,
                const chessBoard::Move& prev_move, uint64 hash);
    void iterative_deepening();

    extern PV G_PV;


}
