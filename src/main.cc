//
// Created by fouche_r on 5/16/19.
//

#include "ia.hh"
#include "ia_env.hh"
#include "perft.hh"
#include <boost/program_options.hpp>
#include <iostream>
#include <adapter.hh>

using namespace boost::program_options;
using Listeners = std::vector<std::string>;


chessBoard::Board ai::env::boardM = chessBoard::Board();

std::vector<chessBoard::Move> ai::env::input_vect = std::vector<chessBoard::Move>();
std::vector<chessBoard::Move> ai::env::input_vect_quiescence = std::vector<chessBoard::Move>();

std::unordered_map<uint64, transposition_table::Data>* ai::env::transposition_table = nullptr;
std::unordered_map<uint64, transposition_table::Data>* ai::env::transposition_table_quiescence = nullptr;

TimePoint ai::env::act_start = system_clock::now();
int ai::env::start_depth = 0;

const std::string ai::env::my_name = "Perceval";
bool ai::env::is_opening = true;

std::vector<std::pair<uint64, int>> ai::env::vectBoard = std::vector<std::pair<uint64, int>>{};



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
            ai::IA perceval_le_con;
            perceval_le_con.play_chess();
        }
    }
    catch (const error &ex)
    {
        std::cerr << ex.what() << '\n';
    }
}
