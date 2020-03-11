//
// Created by fouche_r on 5/16/19.
//

#pragma once

#include "../board/board.hh"
#include "listener.hh"
#include "pgn-move.hh"
#include "../board/move.hh"
#include "chessboard-interface.hh"

namespace adapter
{
    using namespace yaka;
    using namespace chessBoard;
    using Listeners = std::vector<listener::Listener *>;
    using Bvect = std::vector<std::pair<Board, int>>;
    class Adapter : public ChessboardInterface
    {
    private:

        Listeners listeners;
        chessBoard::Board boardM;

        void execute_moves(std::vector<PgnMove> &moves);
        bool listener_calls(chessBoard::Move &move, const yaka::PgnMove& ref, Bvect& vect);
        void init_listeners();
        bool disqualify_if_false(bool cond);
        bool player_is_mat(chessBoard::Move &move, const yaka::PgnMove& ref);

        /* Senders */
        void game_finished_sends();

    public:
        Adapter(const std::vector<std::string> &listeners,
                const std::string &path);


        opt_piece_t operator[](const Position &position) const override;
    };
}