#include "..\Extensions\BitboardConstants.h"
using namespace BitboardConstants;
typedef unsigned long long U64;

bitset<64> diagonals[64];
bitset<64> horizVert[64];
bitset<64> maskBishop[64];
bitset<64> maskRook[64];
int rankR[8][128];

const int repsB[64] =
{
    6, 5, 4, 3, 3, 4, 5, 6,
    5, 5, 4, 3, 3, 4, 5, 5,
    4, 4, 4, 3, 3, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 3, 3, 4, 4, 4,
    5, 5, 4, 3, 3, 4, 5, 5,
    6, 5, 4, 3, 3, 4, 5, 6,
};

const int repsR[64] =
{
    7, 6, 6, 6, 6, 6, 6, 8,
    7, 5, 5, 5, 5, 5, 5, 8,
    7, 5, 4, 4, 4, 4, 5, 8,
    7, 5, 4, 3, 3, 4, 5, 8,
    7, 5, 4, 3, 3, 4, 5, 8,
    7, 5, 4, 4, 4, 4, 5, 8,
    7, 5, 5, 5, 5, 5, 5, 8,
    7, 6, 6, 6, 6, 6, 6, 8,
};

namespace PrecomputtedMoves  {

    void DiagonalsOffsets(bitset<64>* temp) {
        for(int sq = 0; sq < 64; ++sq) {
            bitset<64> bb;
            bb.set(sq);
            temp[sq]  = (bb >> 7) & ~aFile & ~rank1;
            temp[sq] |= (bb >> 9) & ~hFile & ~rank1;
            temp[sq] |= (bb << 7) & ~hFile & ~rank8;
            temp[sq] |= (bb << 9) & ~aFile & ~rank8;
            //PrintBitSet(diagonals[sq], to_string(sq));
        }
    }

    void RookOffsets(bitset<64>* temp) {
        for (int sq = 0; sq < 64; ++sq ) {
            bitset<64> bb;
            bb.set(sq);
            temp[sq]  = (bb >> 8);
            temp[sq] |= (bb >> 1) & ~hFile;
            temp[sq] |= (bb << 8);
            temp[sq] |= (bb << 1) & ~aFile;
            //PrintBitSet(horizVert[sq], to_string(sq));
        }
    }

    void MaskBishop(bitset<64>* temp) {
        bitset<64> diagonals[64];
        for(int sq = 0; sq < 64; ++sq) {
            bitset<64> bb;
            bb.set(sq);
            diagonals[sq]  = (bb >> 7) & ~aFile & ~rank1;
            diagonals[sq] |= (bb >> 9) & ~hFile & ~rank1;
            diagonals[sq] |= (bb << 7) & ~hFile & ~rank8;
            diagonals[sq] |= (bb << 9) & ~aFile & ~rank8;
            //PrintBitSet(diagonals[sq], to_string(sq));
        }

        for (int sq = 0; sq < 64; ++sq ) {
            temp[sq] = 0;
            int i;
            for ( i = sq - 9; i >= 0 && i % 8 != 7; i -= 9 )
                temp[sq] |= bitset<64>(1) << i;
            for ( i = sq - 7; i >= 0 && i % 8 != 0; i -= 7 )
                temp[sq] |= bitset<64>(1) << i;
            for ( i = sq + 9; i < 64 && i % 8 != 0; i += 9 )
                temp[sq] |= bitset<64>(1) << i;
            for ( i = sq + 7; i < 64 && i % 8 != 7; i += 7 )
                temp[sq] |= bitset<64>(1) << i;
            //PrintBitSet(maskBishop[sq], to_string(sq));
        }
    }

    void MaskRook(bitset<64>* temp) {
        for (int sq = 0; sq < 64; ++sq ) {
            temp[sq] = 0;
            int i;
            for ( i = sq - 8; i >= 0; i -= 8 )
                temp[sq] |= bitset<64>(1) << i;
            for ( i = sq - 1; i >= 0 && (i & 7) != 7; --i )
                temp[sq] |= bitset<64>(1) << i;
            for ( i = sq + 1; i < 64 && (i & 7) != 0; ++i )
                temp[sq] |= bitset<64>(1) << i;
            for ( i = sq + 8; i < 64; i += 8 )
                temp[sq] |= bitset<64>(1) << i;
            //PrintBitSet(maskRook[sq], to_string(sq));
        }
    }

    void makeRankR(int temp[8][128]) {
        for (int sq = 0; sq < 8; ++sq) {
            for (int i = 0; i < 128; i += 2) {
                int rr = 0;
                int j;
                for (j=sq - 1; j >= 0; --j) {
                    rr |= (1 << j);
                    if (!(i & (1 << j)))  // the 1 bits are the free squares
                        break;
                }
                for (j=sq + 1; j < 8; ++j) {
                    rr |= (1 << j);
                    if (!(i & (1 << j)))  // the 1 bits are the free squares
                        break;
                }
                temp[sq][i  ] = rr;
                temp[sq][i+1] = rr;
            }
        }
    }

    void PrecomputeData() {
        DiagonalsOffsets(diagonals);
        RookOffsets(horizVert);
        MaskBishop(maskBishop);
        MaskRook(maskRook);
        makeRankR(rankR);
    }
}