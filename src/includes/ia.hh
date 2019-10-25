//
// Created by fouche_r on 5/22/19.
//

#pragma once

#include "perft.hh"
#include "stock.hh"
#include <helper.hh>
#include <uci.hh>
#include <board.hh>
#include <map>
#include <chrono>
#include <vector>
#include <algorithm>
#include <unordered_map>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
using namespace std::chrono;

using INT_MOVE_PAIR = std::pair<int, chessBoard::Move>;
using VECTOR_PAIR = std::vector<INT_MOVE_PAIR >;

namespace ai
{
    #define QUEEN 900
    #define ROOKS 500
    #define BISHOP 330
    #define KNIGHT 320
    #define PAWN 100
    #define KING 20000


    static const std::map<std::string, int> map_test_white {
            {"6k1/6p1/6Pp/7P/p7/Pp6/1P6/1K6 w - - 0 0\n", 0},
            {"6k1/6p1/6Pp/7P/p7/Pp6/1P6/3K4 w - - 0 0\n", 1},
            {"6k1/6p1/6Pp/7P/p7/Pp6/1P1K4/8 w - - 0 0\n", 2},

            {"4k3/6p1/6Pp/7P/p7/Pp6/1P6/1K6 w - - 0 0\n", -1},
            {"4k3/6p1/6Pp/7P/p7/Pp6/1P6/3K4 w - - 0 0\n", 0},
            {"4k3/6p1/6Pp/7P/p7/Pp6/1P1K4/8 w - - 0 0\n", 1},

            {"8/4k1p1/6Pp/7P/p7/Pp6/1P6/1K6 w - - 0 0\n", -2},
            {"8/4k1p1/6Pp/7P/p7/Pp6/1P6/3K4 w - - 0 0\n", -1},
            {"8/4k1p1/6Pp/7P/p7/Pp6/1P1K4/8 w - - 0 0\n", 0},
    };

    static const std::map<std::string, int> map_test_black {
            {"6k1/6p1/6Pp/7P/p7/Pp6/1P6/1K6 b - - 0 0\n", 0},
            {"6k1/6p1/6Pp/7P/p7/Pp6/1P6/3K4 b - - 0 0\n", -1},
            {"6k1/6p1/6Pp/7P/p7/Pp6/1P1K4/8 b - - 0 0\n", -2},

            {"4k3/6p1/6Pp/7P/p7/Pp6/1P6/1K6 b - - 0 0\n", 1},
            {"4k3/6p1/6Pp/7P/p7/Pp6/1P6/3K4 b - - 0 0\n", 0},
            {"4k3/6p1/6Pp/7P/p7/Pp6/1P1K4/8 b - - 0 0\n", -1},

            {"8/4k1p1/6Pp/7P/p7/Pp6/1P6/1K6 b - - 0 0\n", 2},
            {"8/4k1p1/6Pp/7P/p7/Pp6/1P6/3K4 b - - 0 0\n", 1},
            {"8/4k1p1/6Pp/7P/p7/Pp6/1P1K4/8 b - - 0 0\n", 0},
    };

    class Data
    {
    public:
        Data(const std::optional<chessBoard::Move> move, int score, int depth, int is_cut_off) : move_(move), score_(score), depth_(depth), is_cut_off_(is_cut_off)
        {}
        int score_get();
        void score_set(int score);

        chessBoard::Move move_get();
        void move_set(std::optional<chessBoard::Move> move);

        int depth_get();
        void depth_set(int depth);

        int is_cut_off_get();
        void is_cut_off_set(int is_cut_off);

        bool move_has_value();





    private:
        std::optional<chessBoard::Move> move_;
        int score_;
        int depth_;
        int is_cut_off_;
    };

    class IA
    {
    private:
        chessBoard::Board boardM;

        std::vector<chessBoard::Move> input_vect;
        std::vector<chessBoard::Move> input_vect_quiescence;

        std::unordered_map<uint64, Data>* transposition_table;
        std::unordered_map<uint64, Data>* transposition_table_quiescence;

        TimePoint act_start;
        int start_depth;

        const std::string my_name = "Perceval";
        bool is_opening = true;
    public:
        std::vector<std::pair<uint64, int>> vectBoard;
        void play_chess();
        int test_mini_max() const;
        int eval_queen(const chessBoard::enumPiece& color,
                const chessBoard::enumPiece& not_color_) const;
        int eval_rook(const chessBoard::enumPiece& color,
                const chessBoard::enumPiece& not_color_) const;
        int eval_bishop(const chessBoard::enumPiece& color,
                const chessBoard::enumPiece& not_color_) const;
        int eval_knight(const chessBoard::enumPiece& color,
                const chessBoard::enumPiece& not_color_) const;
        int eval_pawn(const chessBoard::enumPiece& color,
                const chessBoard::enumPiece& not_color_) const;
        int eval_king(const chessBoard::enumPiece& color,
                const chessBoard::enumPiece& not_color_) const;
        int evaluate(const chessBoard::enumPiece& color_) const;


        int alphabeta(const chessBoard::enumPiece& colo_act, int depth, int alpha,
                int beta, const chessBoard::Move& prev_move,
                chessBoard::MOVES_T& prev_vect_move,
                chessBoard::MOVES_T& prev_vect_move_quiescence, uint64 hash);

        int caller_alphabeta(int depth, std::vector<chessBoard::Move>& output_vect, std::vector<chessBoard::Move>& output_vect_quiescence, uint64 hash);

        int quiesce(const chessBoard::enumPiece& color_act, int alpha, int beta,
                const chessBoard::Move& prev_move, int depth,
                chessBoard::MOVES_T& prev_vect_move_quiescence, uint64 hash);
        std::optional<chessBoard::Move> iterative_deepening(int max_size, uint64 hash);
        VECTOR_PAIR moves_set_values(const std::vector<chessBoard::Move>& vect,
                const std::optional<chessBoard::Move>& prev_move, int depth, uint64 hash);
        VECTOR_PAIR moves_set_values_quiescence(const std::vector<chessBoard::Move>& vect,
                const std::optional<chessBoard::Move>& prev_move, int depth, uint64 hash);

        /* Time management */
        int get_boost_factor(int nb_turns) const;
        int approx_next_time(const chessBoard::Board& cur_b, int depth) const;
        bool approx_enough_time(const TimePoint& act_start,
                const TimePoint& start, int max_time, size_t depth) const;
        void update_transposition_table(const std::optional<chessBoard::Move>& move, int score, int depth, uint64 hash, int is_cut_off);
        void update_transposition_table_quiescence(const std::optional<chessBoard::Move>& move, int score, uint64 hash, int is_cut_off);
        int val_max_depth();
        int quiescence_calc_move(const chessBoard::Move& move,
                const std::optional<chessBoard::Move>& prev_move, size_t depth, uint64 hash);
        int calc_move(const chessBoard::Move& move, size_t act_depth,
                const std::optional<chessBoard::Move>& prev_move, uint64 hash);
        int give_time(int time_left);
    };
    void merge_vect(std::vector<chessBoard::Move>& vect1, std::vector<chessBoard::Move>& vect2);
    bool threefoldd2(std::vector<std::pair<uint64, int>>& vect, const uint64& hash, int half_move_count, int& index);
    uint64 zobrist(const chessBoard::Board& b);
}
