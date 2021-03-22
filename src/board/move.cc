#include <utility>

//
// Created by fouche_r on 5/16/19.
//

#include <strings.h>
#include <stock.hh>
//#include "../adapter/pgn-parser.hh"
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>

//#include "sstream"
#include "move.hh"

namespace chessBoard
{
    std::string Move::to_str()  const
    {
        int indexfrom = ffsll(this->getFromPosition());
        int indexto = ffsll(this->getToPosition());
        std::string s;
        s.push_back(yaka::file_to_char[7 - ((indexfrom -1) % 8)]);
        s.push_back(yaka::rank_to_char[(indexfrom - 1) / 8]);
        s.push_back(yaka::file_to_char[7 - ((indexto -1) % 8)]);
        s.push_back(yaka::rank_to_char[(indexto - 1) / 8]);
        if (this->isPromotion())
        {
            if (this->isBishopPromotion() || this->isBishopPromotionCapture())
                s.push_back('b');
            else if (this->isRookPromotion() || this->isRookPromotionCapture())
                s.push_back('r');
            else if (this->isQueenPromotion() || this->isQueenPromotionCapture())
                s.push_back('q');
            else if (this->isKnightPromotion() || this->isKnightPromotionCapture())
                s.push_back('n');
        }
        return s;
    }

    void VMove::add_move(const Move &mv)
    {
        move_tab[len] = mv;
        len++;
    }
}

