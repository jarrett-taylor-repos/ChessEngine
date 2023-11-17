#include "..\Extensions\U64Extentions.h"
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace U64Extensions;


namespace PrecomputtedMoveData {
    //knights
    U64 noNoEa(U64 b) {return (b >> 15) & notAFile; };  
    U64 noEaEa(U64 b) {return (b >> 6) & (notAFile & notBFile);};
    U64 soEaEa(U64 b) {return (b << 10) & (notAFile & notBFile);};
    U64 soSoEa(U64 b) {return (b << 17) & notAFile; };
    U64 soSoWe(U64 b) {return (b << 15) & notHFile; };
    U64 soWeWe(U64 b) {return (b << 6) & (notGFile & notHFile); };
    U64 noWeWe(U64 b) {return (b >> 10) & (notGFile & notHFile); };
    U64 noNoWe(U64 b) {return (b >> 17) & notHFile; };
    U64 KnightAttacks(U64 b) { return noNoEa(b) | noEaEa(b) | soEaEa(b) | soSoEa(b) | noNoWe(b) | noWeWe(b) | soWeWe(b) | soSoWe(b); };

    //kings
    U64 northOne(U64 b) { return b >> 8; };
    U64 southOne(U64 b) { return b << 8; };
    U64 eastOne(U64 b) { return b << 1 & notAFile; };
    U64 westOne(U64 b) { return b >> 1 & notHFile; };
    U64 northEastOne(U64 b) { return b >> 7 & notAFile; };
    U64 northWestOne(U64 b) { return b >> 9 & notHFile; };
    U64 southEastOne(U64 b) { return b << 9 & notAFile; };
    U64 southWestOne(U64 b) { return b << 7 & notHFile; };
    U64 OneInAllDirection(U64 b) { return (northOne(b) | southOne(b) | eastOne(b) | westOne(b) | northEastOne(b) | northWestOne(b) | southEastOne(b) | southWestOne(b)); };

    //pawns
    U64 wSinglePushTargets(U64 b) { return northOne(b); };
    U64 bSinglePushTargets(U64 b) { return southOne(b); };

    U64 wPawnWestAtt(U64 b) { return (b >> 9) & notHFile; };
    U64 wPawnEastAtt(U64 b) { return (b >> 7) & notAFile; };
    U64 wPawnAllAtt(U64 b) { return wPawnEastAtt(b) | wPawnWestAtt(b); };
    U64 bPawnWestAtt(U64 b) { return (b << 9) & notAFile; };
    U64 bPawnEastAtt(U64 b) { return (b << 7) & notHFile; };
    U64 bPawnAllAtt(U64 b) { return bPawnEastAtt(b) | bPawnWestAtt(b); };


    void GenerateSingleBitBoards(ofstream &of) {
        of << "U64 precomputtedSingleBit[64] = {";
        for(int i = 0; i < 64; i++) {
            U64 single = SingleBitBoard(i);
            of << single;
            if(i != 63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedKnights(ofstream &of) {
        of << "U64 precomputtedKnights[64] = {";
        for(int i = 0; i < 64; i++) {
            U64 knightAtt = KnightAttacks(SingleBitBoard(i));
            of << knightAtt;
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedKings(ofstream &of) {
        of << "U64 precomputtedKings[64] = {";
        for(int i = 0; i < 64; i++) {
            U64 knightAtt = OneInAllDirection(SingleBitBoard(i));
            of << knightAtt;
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedWhitePawnAttacks(ofstream &of) {
        of << "U64 precomputtedWhitePawnAttacks[64] = {";
        for(int i = 0; i < 64; i++) {
            U64 pawnAtt = wPawnAllAtt(SingleBitBoard(i));
            of << pawnAtt;
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedBlackPawnAttacks(ofstream &of) {
        of << "U64 precomputtedBlackPawnAttacks[64] = {";
        for(int i = 0; i < 64; i++) {
            U64 pawnAtt = bPawnAllAtt(SingleBitBoard(i));
            of << pawnAtt;
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedWhitePawnSinglePush(ofstream &of) {
        of << "U64 precomputtedWhitePawnSinglePush[64] = {";
        for(int i = 0; i < 64; i++) {
            U64 pawnPush = wSinglePushTargets(SingleBitBoard(i));
            of << pawnPush;
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedBlackPawnSinglePush(ofstream &of) {
        of << "U64 precomputtedBlackPawnSinglePush[64] = {";
        for(int i = 0; i < 64; i++) {
            U64 pawnPush = bSinglePushTargets(SingleBitBoard(i));
            of << pawnPush;
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedWhitePawnAttackMap(ofstream &of) {
        of << "map<U64, U64> precomputtedWhitePawnAttackMap = {";
        for(int i = 0; i < 64; i++) {
            U64 key = SingleBitBoard(i);
            U64 pawnPush = wPawnAllAtt(SingleBitBoard(i));
            of << "{" + to_string(key) + ", " + to_string(pawnPush) + "}";
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedBlackPawnAttackMap(ofstream &of) {
        of << "map<U64, U64> precomputtedBlackPawnAttackMap = {";
        for(int i = 0; i < 64; i++) {
            U64 key = SingleBitBoard(i);
            U64 pawnPush = bPawnAllAtt(SingleBitBoard(i));
            of << "{" + to_string(key) + ", " + to_string(pawnPush) + "}";
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedKnightMap(ofstream &of) {
        of << "map<U64, U64> precomputtedKnightMap = {";
        for(int i = 0; i < 64; i++) {
            U64 key = SingleBitBoard(i);
            U64 knightAtt = KnightAttacks(SingleBitBoard(i));
            of << "{" + to_string(key) + ", " + to_string(knightAtt) + "}";
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedKingMap(ofstream &of) {
        of << "map<U64, U64> precomputtedKingMap = {";
        for(int i = 0; i < 64; i++) {
            U64 key = SingleBitBoard(i);
            U64 kingAtt = OneInAllDirection(SingleBitBoard(i));
            of << "{" + to_string(key) + ", " + to_string(kingAtt) + "}";
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedBishopMask(ofstream &of) {
        of << "U64 precomputtedBishopMask[64] = {";
        for(int i = 0; i < 64; i++) {
            U64 attacks = MaskBishop(i);
            of << attacks;
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputtedRookMask(ofstream &of) {
        of << "U64 precomputtedRookMask[64] = {";
        for(int i = 0; i < 64; i++) {
            U64 attacks = MaskRook(i);
            of << attacks;
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GenerateBishopOccupanyBitLookupTables(ofstream &of) {
        of << "const int BishopOccupanyBitCount[64] = {" << endl;
        for(int rank = 0; rank < 8; rank++) {
            for(int file = 0; file < 8; file++) {
                int sq = rank * 8 + file;
                if(sq % 8 == 0) of << "    ";
                of << CountBits(MaskBishop(sq));
                if(sq != 63) of << ", ";
            }
            of << endl;
        }
        of << "};" << endl;
    }

    void GenerateRookOccupanyBitLookupTables(ofstream &of) {
        of << "const int RookOccupanyBitCount[64] = {" << endl;
        for(int rank = 0; rank < 8; rank++) {
            for(int file = 0; file < 8; file++) {
                int sq = rank * 8 + file;
                if(sq % 8 == 0) of << "    ";
                of << CountBits(MaskRook(sq));
                if(sq != 63) of << ", ";
            }
            of << endl;
        }
        of << "};" << endl;
    }

    void GenerateRookMagicNumbers(ofstream &of) {
        of << "U64 rookMagicNumbers[64] = {";
        for(int i = 0; i < 64; i++) {
            U64 attacks = FindMagicNumber(i, CountBits(MaskRook(i)), false);
            of << attacks;
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GenerateBishopMagicNumbers(ofstream &of) {
        of << "U64 bishopMagicNumbers[64] = {";
        for(int i = 0; i < 64; i++) {
            U64 attacks = FindMagicNumber(i, CountBits(MaskBishop(i)), true);
            of << attacks;
            if(i !=63) of << ", ";
        }
        of << "};" << endl;
    }

    void GeneratePrecomputted() {
        ofstream of("PrecomputtedMoveData.txt");
        GenerateSingleBitBoards(of);
        GeneratePrecomputtedKnights(of);
        GeneratePrecomputtedKings(of);

        GeneratePrecomputtedWhitePawnAttacks(of);
        GeneratePrecomputtedBlackPawnAttacks(of);
        GeneratePrecomputtedWhitePawnSinglePush(of);
        GeneratePrecomputtedBlackPawnSinglePush(of);
        GeneratePrecomputtedWhitePawnAttackMap(of);
        GeneratePrecomputtedBlackPawnAttackMap(of);

        GeneratePrecomputtedKnightMap(of);
        GeneratePrecomputtedKingMap(of);

        GeneratePrecomputtedBishopMask(of);
        GeneratePrecomputtedRookMask(of);

        GenerateBishopOccupanyBitLookupTables(of);
        GenerateRookOccupanyBitLookupTables(of);

        GenerateRookMagicNumbers(of);
        GenerateBishopMagicNumbers(of);
        of.close();
    }
}