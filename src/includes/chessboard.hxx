//
// Created by romain on 6/4/19.
//

#include <strings.h>
#include <iostream>


namespace chessBoard
{
    inline bool pawn_will_promote(INDEX_T index, enumPiece color)
    {
        if (color == nWhite)
        {
            if (index >= 48)
                return true;
        }
        else
        {
            if (index <= 15)
                return true;
        }
        return false;
    }
    inline INDEX_T split_index(uint64& pBB)
    {
        int index = __builtin_ffsll(pBB);
        //uint64 pos = index_to_uint64(index);
        //uint64 pos = 1 << (index);
        //pBB ^= pos;
        pBB &= pBB - 1;
        //// check if dereferencing an array is faster
        return index;
    }

    inline POSITION_T split_pos(uint64& pBB)
    {
        int index = __builtin_ffsll(pBB);
        //uint64 pos = index_to_uint64(index);
        //       pos = pos << (index);
        //uint64 pos = (1ull << index);
        pBB &= pBB - 1;
        if (index == 0)
            return 0;
        index -= 1;
        return 1ull << index;
    }

    inline POSITION_T pos_from_index(const INDEX_T& index)
    {
        uint64 l = 1ull;
        POSITION_T pos = l << (index);
        return pos;
    }


    inline int number_piece(uint64 num)
    {
        int i = 0;
        while (split_index(num) != 0)
            ++i;
        return i;
    }
}