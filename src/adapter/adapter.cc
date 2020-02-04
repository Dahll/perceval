//
// Created by fouche_r on 5/16/19.
//

#include "adapter.hh"
#include "convertion.hh"
#include "listenerCalls.hh"
#include <dlfcn.h>
#include "pgn-parser.hh"
#include <optional>

namespace adapter
{
    ChessboardInterface::opt_piece_t
    Adapter::operator[](const Position &position) const
    {
        auto index = position_to_index(position);
        return CBopt_to_CIopt(boardM[index]);
    }

    void Adapter::execute_moves(std::vector<PgnMove> &moves)
    {
  //      std::cout << boardM.to_perft() << std::endl;
//        boardM.print();
        Bvect vect;
        for (const auto &PGNmove : moves)
        {
       //     std::cout << file_to_char[utils::utype(PGNmove.start_get().file_get())]
     //               << rank_to_char[utils::utype(PGNmove.start_get().rank_get())]
   //                 << file_to_char[utils::utype(PGNmove.end_get().file_get()) ]
 //                   << rank_to_char[utils::utype(PGNmove.end_get().rank_get())] << std::endl;
            auto move = PGNmove_to_CBmove(PGNmove);
            bool finished = listener_calls(move, PGNmove, vect);
            if (finished) {
                game_finished_sends();
                break;
            }
            boardM.switch_color();
         //   boardM.print();
        }
        //boardM.print();
        //game_finished_sends();
    }

    /* Constructor */
    Adapter::Adapter(const std::vector<std::string> &listeners_s,
                     const std::string &path)
    {
        std::vector<void *>dl_opens;
        for (auto &l : listeners_s)
        {
            auto so_open = dlopen(l.c_str(), RTLD_LAZY);
            if (!so_open)
            {
                fprintf(stderr, "%s\n", dlerror());
                exit(EXIT_FAILURE);
            }
            dl_opens.emplace_back(so_open);
            auto symbol = dlsym(so_open, "listener_create");
            listener::Listener *listener = reinterpret_cast<listener::Listener *(*)()>(symbol)();
            listeners.emplace_back(listener);
        }

        init_listeners();

        auto moves = pgn_parser::parse_pgn(path);
        execute_moves(moves);

        for (auto& so : dl_opens)
            dlclose(so);
    }
}