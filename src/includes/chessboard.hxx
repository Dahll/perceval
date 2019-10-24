//
// Created by romain on 6/4/19.
//

#include <strings.h>


namespace chessBoard
{
    inline bool pawn_will_promote(INDEX_T index, enumPiece color)
    {
        if (color == nWhite)
        {
            if (index >= 49)
                return true;
        }
        else
        {
            if (index <= 16)
                return true;
        }
        return false;
    }
    inline INDEX_T split_index(uint64& pBB)
    {
        int index = ffsll(pBB);
        uint64 pos = index_to_uint64(index);
//        pos = pos << (index);
        pBB ^= pos;
        //// check if dereferencing an array is faster
        return index;
    }

    inline POSITION_T split_pos(uint64& pBB)
    {
        int index = ffsll(pBB);
        uint64 pos = index_to_uint64(index);
        //       pos = pos << (index);
        pBB = pBB ^ pos;
        return pos;
    }

    inline POSITION_T pos_from_index(const INDEX_T& index)
    {
        uint64 l = 1;
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