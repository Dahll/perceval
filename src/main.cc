//
// Created by fouche_r on 5/16/19.
//

#include "ia/ia.hh"
#include "ia/ia_env.hh"
#include "perft.hh"
#include <boost/program_options.hpp>
#include <iostream>
#include <adapter.hh>

using namespace boost::program_options;
using Listeners = std::vector<std::string>;




std::vector<chessBoard::Move> ai::env::input_vect = std::vector<chessBoard::Move>();
std::vector<chessBoard::Move> ai::env::input_vect_quiescence = std::vector<chessBoard::Move>();




int main(int argc, const char *argv[])
{
    // FIXME tester etat board

    // FIXME en passant/pion avancer de 2 et revert tout ca
    // FIXME draws 50 half move rule/3 times sqme board
    try
    {
        Listeners listeners;

        options_description desc{"Options"};
        desc.add_options()
                ("help,h", "help")
                ("pgn", value<std::string>(), "pgn")
                ("listeners,l", value<Listeners >(&listeners)->multitoken(), "listeners")
                ("perft", value<std::string>(), "perft");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);
        if (vm.count("help"))
            std::cout << desc << '\n';
        else if (vm.count("pgn"))
        {
            std::string path = vm["pgn"].as<std::string>();
            for (auto& l : listeners)
                if (l[0] != '/')
                    l.insert(0, "./");
            adapter::Adapter(listeners, path);

        }
        else if (vm.count("perft"))
        {
            std::string path = vm["perft"].as<std::string>();
            std::cout << Perft::perft(path) << std::endl;
        }
        else
        {
            uci::loop();
        }
    }
    catch (const error &ex)
    {
        std::cerr << ex.what() << '\n';
    }
}
