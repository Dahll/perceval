//
// Created by adrien on 21/05/19.
//

#pragma once
#include "tiff.h"
#include <string>
#include <vector>
#include <array>


namespace
{
    using array_zobrist = std::array<uint64, 911>;
    using array_optional = std::array<uint64, 4096>;
    using array_array = std::array<array_optional, 65>;
    using array_64 = std::array<uint64, 64>;
    using array_65 = std::array<uint64, 65>;

}

    std::string print_list_zobrist(const array_zobrist& list);

    constexpr array_65 magic_number_tower = {0u,612498416294952992u,369295994084400144u,2364390908712980484u,72064476991496960u,144116571189809160u,9367491625158181656u,1315070884565745728u,2341872355992940800u,4755941946144342016u,22027815096320u,8804684534792u,72127997146170368u,1143526453676298u,1152939663729758464u,145423465447488u,583884690249777168u,126101064716918784u,36077184129204256u,36134487616389160u,1275605425328256u,937876821859894280u,1441187099624686080u,36034363449671744u,40166536384772u,4972149996913721346u,2553203474334080u,21409726252195841u,18051790830862368u,577609200789225473u,2306971109217665281u,18049592142667784u,864704877182353473u,5192687208427422016u,72128031908430096u,153439046747554304u,246359642866944u,1143578009012256u,72656003249882128u,1801448715862409296u,288301298947223585u,4611756396178866176u,4556513629712386u,12106243110080512u,864867058939461700u,9587775825258496u,1196406895804486u,4611686709951221776u,2200165163009u,9259402208265375776u,72207127757717808u,4435174621200u,2377918341603132577u,1729663741295067392u,13778557077632u,14269673014764247048u,38579938940657668u,649789798399213586u,18454619791851585u,72063092806136129u,9296001394421010498u,1152939251446874113u,282033461133313u,2305843318485943060u,3315797254563971170u};

    constexpr array_65 magic_number_bishop = {0u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,1083023177046531879u,10915881610578495130u,10915881610578495130u,10915881610578495130u,10915881610578495130u,10915881610578495130u,10915881610578495130u,10915881610578495130u,10915881610578495130u,5258989156066218480u,58583225069519136u,577623485884543264u,796329560572638562u,796329560572638562u,796329560572638562u,796329560572638562u,796329560572638562u,796329560572638562u,796329560572638562u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u,6474786582482038824u};

    std::string int_to_string(uint64 index);

    std::string print_list(const array_optional &list);

    std::string print_tab(const array_array &tab);

    std::string print_magique(const array_65 &list);

    void convertToBinary(uint64 n, std::string &str);

    uint64 ZeroBitBiasedRandom();


    constexpr bool is_occupied(uint64 pos, uint64 index, std::array<uint64, 64> &list);

    constexpr uint64 add_byte(uint64 b, uint64 param);

    constexpr array_64 generate_list();

    array_array find_magique_tower();

    array_array find_magique_bishop();


#include "generate.hxx"



