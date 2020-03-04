#include "magic.hh"

using namespace std;



const uint64 RMagic[64] = {0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL, 0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL, 0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL, 0x1002100004082ULL, 0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL, 0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL, 0x800080204005ULL, 0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL, 0x9020010080800200ULL, 0x813241200148449ULL, 0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL, 0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL, 0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL, 0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL, 0xe0100040002240ULL, 0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL, 0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL, 0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL};

const uint64 BMagic[64] = {0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL, 0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL, 0x400492088408100ULL, 0x201c401040c0084ULL, 0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL, 0x8144042209100900ULL, 0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL, 0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL, 0x520040470104290ULL, 0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL, 0x188071040440a00ULL, 0x4800404002011c00ULL, 0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL, 0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL, 0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL, 0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL, 0x6803040141280a00ULL, 0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL, 0x822088220820214ULL, 0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL, 0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL};

int RBits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
};

int BBits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
};

uint64 attack_table_rook[102400] = {0ull};
uint64 attack_table_bishop[5248] = {0ull};

SMagic mBishopTbl[64];
SMagic mRookTbl[64];

void print_uint64(uint64 number)
{
    std::string str = "";
    int j = 1;
    while (j < 65)
    {
        if (number & (1ull << (j-1)))
        {
            str.insert(0,"1");
        }
        else
        {
            str.insert(0,"0");
        }
        if ((j) % 8 == 0)
            str.insert(0, "\n");
        j++;
    }
    std::cout << str << "\n";

}

int count_1s(uint64 b) {
    int r;
    for(r = 0; b; r++, b &= b - 1);
    return r;
}

const int BitTable[64] = {
    63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
    51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
    26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
    58, 20, 37, 17, 36, 8
};

int pop_1st_bit(uint64 *bb) {
    uint64 b = *bb ^ (*bb - 1);
    unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    *bb &= (*bb - 1);
    return BitTable[(fold * 0x783a9b23) >> 26];
}

uint64 index_to_uint64(int index, int bits, uint64 m) {
    int i, j;
    uint64 result = 0ULL;
    for(i = 0; i < bits; i++)
    {
        j = pop_1st_bit(&m);
        if(index & (1 << i)) result |= (1ULL << j);
    }
    return result;
}

uint64 rmask(int sq) {
    uint64 result = 0ULL;
    int rk = sq/8, fl = sq%8, r, f;
    for(r = rk+1; r <= 6; r++) result |= (1ULL << (fl + r*8));
    for(r = rk-1; r >= 1; r--) result |= (1ULL << (fl + r*8));
    for(f = fl+1; f <= 6; f++) result |= (1ULL << (f + rk*8));
    for(f = fl-1; f >= 1; f--) result |= (1ULL << (f + rk*8));
    return result;
}

uint64 bmask(int sq) {
    uint64 result = 0ULL;
    int rk = sq/8, fl = sq%8, r, f;
    for(r=rk+1, f=fl+1; r<=6 && f<=6; r++, f++) result |= (1ULL << (f + r*8));
    for(r=rk+1, f=fl-1; r<=6 && f>=1; r++, f--) result |= (1ULL << (f + r*8));
    for(r=rk-1, f=fl+1; r>=1 && f<=6; r--, f++) result |= (1ULL << (f + r*8));
    for(r=rk-1, f=fl-1; r>=1 && f>=1; r--, f--) result |= (1ULL << (f + r*8));
    return result;
}

uint64 ratt(int sq, uint64 block) {
    uint64 result = 0ULL;
    int rk = sq/8, fl = sq%8, r, f;
    for (r = rk+1; r <= 7; r++)
    {
        result |= (1ULL << (fl + r*8));
        if (block & (1ULL << (fl + r*8)))
            break;
    }
    for (r = rk-1; r >= 0; r--)
    {
        result |= (1ULL << (fl + r*8));
        if (block & (1ULL << (fl + r*8)))
            break;
    }
    for (f = fl+1; f <= 7; f++)
    {
        result |= (1ULL << (f + rk*8));
        if (block & (1ULL << (f + rk*8)))
            break;
    }
    for (f = fl-1; f >= 0; f--)
    {
        result |= (1ULL << (f + rk*8));
        if (block & (1ULL << (f + rk*8)))
            break;
    }
    return result;
}

uint64 batt(int sq, uint64 block) {
      uint64 result = 0ULL;
      int rk = sq/8, fl = sq%8, r, f;
      for (r = rk+1, f = fl+1; r <= 7 && f <= 7; r++, f++)
      {
        result |= (1ULL << (f + r*8));
        if(block & (1ULL << (f + r * 8)))
            break;
      }
      for (r = rk+1, f = fl-1; r <= 7 && f >= 0; r++, f--)
      {
        result |= (1ULL << (f + r*8));
        if(block & (1ULL << (f + r * 8)))
            break;
      }
      for (r = rk-1, f = fl+1; r >= 0 && f <= 7; r--, f++)
      {
        result |= (1ULL << (f + r*8));
        if(block & (1ULL << (f + r * 8)))
            break;
      }
      for (r = rk-1, f = fl-1; r >= 0 && f >= 0; r--, f--)
      {
        result |= (1ULL << (f + r*8));
        if(block & (1ULL << (f + r * 8)))
            break;
      }
      return result;
}


int transform(uint64 b, uint64 magic, int bits) {
  return (int)((b * magic) >> (64 - bits));
}

void find_magic(int sq, int m, int bishop, uint64* ptr) {
  uint64 mask, b[4096], a[4096], magic;
  int i, j, n;
  mask = bishop? bmask(sq) : rmask(sq);
  n = count_1s(mask);

  for(i = 0; i < (1 << n); i++) {
    b[i] = index_to_uint64(i, n, mask);
    a[i] = bishop? batt(sq, b[i]) : ratt(sq, b[i]);
  }

  magic = bishop? BMagic[sq] : RMagic[sq];
  
  for(i = 0; i < (1 << n); i++) {
    j = transform(b[i], magic, m);
    ptr[j] = a[i];
  }

  SMagic* tbl = bishop? mBishopTbl : mRookTbl;
  tbl[sq].ptr = ptr;
  tbl[sq].mask = mask;
  tbl[sq].magic = magic;
  tbl[sq].shift = 64 - m;
}

void fill_magic() {
  uint64* ptr = attack_table_rook;
  int square;
  for(square = 0; square < 64; square++)
  {
    find_magic(square, RBits[square], 0, ptr);
    ptr += 1ull << RBits[square];
  }

  ptr = attack_table_bishop;
  for(square = 0; square < 64; square++)
  {
    find_magic(square, BBits[square], 1, ptr);
    ptr += 1ull << BBits[square];
  }
}



/*int main()
{
  fill_magic();
  print_uint64(rookAttacks(2819400830217244800ull, 57));
  rookAttacks(2819400830217244800ull, 58);
  return 0;
}*/