#include "Constants.h"
using namespace Constants;

namespace MagicExtensions {
    bool TestIndex(int index, int count) { return index & precomputtedSingleBit[count]; }; 

    int CountBits(U64 b) { 
        int count = 0;
        while(b != C64(0)) {
            count++;
            b &= b - 1;
        }
        return count;
    }

    int GetLSBIndex(U64 b) { 
        if(b == C64(0)) return -1;
        return CountBits((b & -b) - 1);
    }

    //Masks bishops
    U64 MaskBishop(int sq) {
        U64 attacks = C64(0);
        int file,rank;

        int targetRank = sq/8;
        int targetFile = sq%8;

        for(rank = targetRank + 1, file = targetFile + 1; rank <= 6 && file <= 6; rank++, file++) attacks |= (C64(1) << (rank*8 + file)); //south east 
        for(rank = targetRank - 1, file = targetFile + 1; rank >= 1 && file <= 6; rank--, file++) attacks |= (C64(1) << (rank*8 + file)); //north east 
        for(rank = targetRank + 1, file = targetFile - 1; rank <= 6 && file >= 1; rank++, file--) attacks |= (C64(1) << (rank*8 + file)); //south west 
        for(rank = targetRank - 1, file = targetFile - 1; rank >= 1 && file >= 1; rank--, file--) attacks |= (C64(1) << (rank*8 + file)); //north west 
        //Print(attacks, "attacks for " + to_string(sq));
        return attacks;
    }

    U64 BishopAttacks(int sq, U64 block) {
        U64 attacks = C64(0);
        int file,rank;

        int targetRank = sq/8;
        int targetFile = sq%8;

        //south east
        for(rank = targetRank + 1, file = targetFile + 1; rank <= 7 && file <= 7; rank++, file++) {
            attacks |= precomputtedSingleBit[rank*8 + file];
            if(precomputtedSingleBit[rank*8 + file] & block) break;
        }

        //north east 
        for(rank = targetRank - 1, file = targetFile + 1; rank >= 0 && file <= 7; rank--, file++) {
            attacks |= precomputtedSingleBit[rank*8 + file];
            if(precomputtedSingleBit[rank*8 + file] & block) break;
        }

        //south west 
        for(rank = targetRank + 1, file = targetFile - 1; rank <= 7 && file >= 0; rank++, file--) {
            attacks |= precomputtedSingleBit[rank*8 + file];
            if(precomputtedSingleBit[rank*8 + file] & block) break;
        }

        //north west 
        for(rank = targetRank - 1, file = targetFile - 1; rank >= 0 && file >= 0; rank--, file--){
            attacks |= precomputtedSingleBit[rank*8 + file];
            if(precomputtedSingleBit[rank*8 + file] & block) break;
        }
        return attacks;
    }

    //MASK rooks
    U64 MaskRook(int sq) {
        U64 attacks = C64(0);
        int file,rank;

        int targetRank = sq/8;
        int targetFile = sq%8;

        for(rank = targetRank + 1; rank <=6; rank++) attacks |= (C64(1) << (rank*8 + targetFile));
        for(rank = targetRank - 1; rank >=1; rank--) attacks |= (C64(1) << (rank*8 + targetFile));
        for(file = targetFile + 1; file <=6; file++) attacks |= (C64(1) << (targetRank*8 + file)); 
        for(file = targetFile - 1; file >=1; file--) attacks |= (C64(1) << (targetRank*8 + file));
        //Print(attacks, "attacks for " + to_string(sq));
        return attacks;
    }

    U64 RookAttacks(int sq, U64 block) {
        U64 attacks = C64(0);
        int file,rank;

        int targetRank = sq/8;
        int targetFile = sq%8;

        for(rank = targetRank + 1; rank <=7; rank++) {
            attacks |= precomputtedSingleBit[rank*8 + targetFile];
            if(precomputtedSingleBit[rank*8 + targetFile] & block) break;
        }

        for(rank = targetRank - 1; rank >=0; rank--){
            attacks |= precomputtedSingleBit[rank*8 + targetFile];
            if(precomputtedSingleBit[rank*8 + targetFile] & block) break;
        }

        for(file = targetFile + 1; file <=7; file++) {
            attacks |= precomputtedSingleBit[targetRank*8 + file];
            if(precomputtedSingleBit[targetRank*8 + file] & block) break;
        }

        for(file = targetFile - 1; file >=0; file--) {
            attacks |= precomputtedSingleBit[targetRank*8 + file];
            if(precomputtedSingleBit[targetRank*8 + file] & block) break;
        }
        //Print(attacks, "attacks for " + to_string(sq));
        return attacks;
    }

    U64 QueenAttacks(int sq, U64 block) { return BishopAttacks(sq, block) | RookAttacks(sq, block); }

    //occupancy 
    U64 SetOccupancy(int index, int bitCount, U64 attacks) {
        U64 occupancy = C64(0);

        for(int count = 0; count < bitCount; count++) {
            int sq = GetLSBIndex(attacks);
            PopBit(attacks, sq);

            if(TestIndex(index, count)) SetBit(occupancy, sq);
        }
        //Print(occupancy, "occupancy " + to_string(index));
        return occupancy;
    }

    //Magic numbers
    unsigned int GetU32RandomNumber() {
        unsigned int num = state;
        num ^= num << 13; num ^= num >> 17; num ^= num << 5;
        state = num;
        return num;
    }

    U64 GetU64RandomNumber() {
        U64 n1 = (U64)(GetU32RandomNumber() & 0xFFFF);
        U64 n2 = (U64)(GetU32RandomNumber() & 0xFFFF);
        U64 n3 = (U64)(GetU32RandomNumber() & 0xFFFF);
        U64 n4 = (U64)(GetU32RandomNumber() & 0xFFFF);

        return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
    }

    U64 GenerateMagicNumber() { return GetU64RandomNumber() & GetU64RandomNumber() & GetU64RandomNumber(); }

    //magic numbers 
    U64 FindMagicNumber(int sq, int countBits, bool isBishop) {
        U64 occupancies[4096];
        U64 attacks[4096];
        U64 usedAttacks[4096];

        U64 attackMask = isBishop ? precomputtedBishopMask[sq] : precomputtedRookMask[sq];

        int occupancyIndexes = precomputtedSingleBit[countBits];

        for(int i = 0; i < occupancyIndexes; i++) {
            occupancies[i] = SetOccupancy(i, countBits, attackMask);
            attacks[i] = isBishop ? BishopAttacks(sq, occupancies[i]) : RookAttacks(sq, occupancies[i]);
        }

        for(int count = 0; count < 100000000; count++) {
            U64 magic = GenerateMagicNumber();
            if(CountBits((attackMask * magic) & 0xFF00000000000000) < 6) continue;

            memset(usedAttacks, 0, sizeof(usedAttacks));

            int idx, fail;
            for(idx = 0, fail = 0; !fail && idx < occupancyIndexes; idx++) {
                int magicIndex = (int)( (occupancies[idx] * magic) >> (64 - countBits) );

                if(usedAttacks[magicIndex] == C64(0)) {
                    usedAttacks[magicIndex] = attacks[idx];
                } else if (usedAttacks[magicIndex] != attacks[idx]) {
                    fail = 1;
                }
            }

            if(!fail) return magic;
        }
        cerr << "Magic number not found." << endl;
        return C64(0);
    }

    void InitMagicNumbers(U64 bitCount[64], bool isBishop) {
        for(int sq = 0; sq < 64; sq++) {
            cout << FindMagicNumber(sq, bitCount[sq], isBishop) << endl;
        }
    }

    void InitMagicNumbers() {
        for(int sq = 0; sq < 64; sq++) {
            cout << FindMagicNumber(sq, BishopOccupanyBitCount[sq], true) << endl;
        }
    }

    void InitSliderPiecesAttacks(bool isBishop) {
        for(int sq = 0; sq < 64; sq++) {
            U64 attackMask = isBishop ? precomputtedBishopMask[sq] : precomputtedRookMask[sq];
            int countBits = CountBits(attackMask);
            int occupancyIndexes = 1 << countBits;

            for(int idx = 0; idx < occupancyIndexes; idx++) {
                U64 occupancy = SetOccupancy(idx, countBits, attackMask);
                if(isBishop) {
                    int magicIndex = (occupancy * bishopMagicNumbers[sq]) >> (64 - BishopOccupanyBitCount[sq]);
                    magicBishopAttacks[sq][magicIndex] = BishopAttacks(sq, occupancy);
                } else {
                    int magicIndex = (occupancy * rookMagicNumbers[sq]) >> (64 - RookOccupanyBitCount[sq]);
                    magicRookAttacks[sq][magicIndex] = RookAttacks(sq, occupancy);
                }
            }
        }
    }

    void InitAll() {
        InitSliderPiecesAttacks(true);
        InitSliderPiecesAttacks(false);
    }

    //get attacks
    U64 GetBishopAttacks(int sq, U64 occupancy) {
        occupancy &= precomputtedBishopMask[sq];
        occupancy *= bishopMagicNumbers[sq];
        occupancy >>= 64 - BishopOccupanyBitCount[sq];
        return magicBishopAttacks[sq][occupancy];
    }

    U64 GetRookAttacks(int sq, U64 occupancy) {
        occupancy &= precomputtedRookMask[sq];
        occupancy *= rookMagicNumbers[sq];
        occupancy >>= 64 - RookOccupanyBitCount[sq];
        return magicRookAttacks[sq][occupancy];
    }

    U64 GetQueenAttacks(int sq, U64 occupancy) { return GetRookAttacks(sq, occupancy) | GetBishopAttacks(sq, occupancy); }
}