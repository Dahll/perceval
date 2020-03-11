#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include "tiff.h"

struct SMagic {
   uint64* ptr;  // pointer to attack_table for each particular square
   uint64 mask;  // to mask relevant squares of both lines (no outer squares)
   uint64 magic; // magic 64-bit factor
   int shift; // shift right
};

extern uint64 attack_table_rook[102400];
extern uint64 attack_table_bishop[5248];

extern SMagic mBishopTbl[64];
extern SMagic mRookTbl[64];


//uint64 rookAttacks(uint64 occ, int sq);
//uint64 bishopAttacks(uint64 occ, int sq);

inline
uint64 bishopAttacks(uint64 occ, int sq) {
    uint64* aptr = mBishopTbl[sq].ptr;
    occ      &= mBishopTbl[sq].mask;
    occ      *= mBishopTbl[sq].magic;
    occ     >>= mBishopTbl[sq].shift;
    return aptr[occ];
}

inline
uint64 rookAttacks(uint64 occ, int sq) {
    uint64* aptr = mRookTbl[sq].ptr;
    occ &= mRookTbl[sq].mask;
    occ *= mRookTbl[sq].magic;
    occ >>= mRookTbl[sq].shift;
    return aptr[occ];
}



void fill_magic();

