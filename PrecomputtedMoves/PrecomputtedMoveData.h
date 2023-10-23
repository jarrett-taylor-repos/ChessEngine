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
    U64 wPawnWestAtt(U64 b) { return (b >> 9) & notHFile; };
    U64 wPawnEastAtt(U64 b) { return (b >> 7) & notAFile; };
    U64 wPawnAllAtt(U64 b) { return wPawnEastAtt(b) | wPawnWestAtt(b); };
    U64 bPawnWestAtt(U64 b) { return (b << 9) & notAFile; };
    U64 bPawnEastAtt(U64 b) { return (b << 7) & notHFile; };
    U64 bPawnAllAtt(U64 b) { return bPawnEastAtt(b) | bPawnWestAtt(b); };

    void GenerateSingleBitBoards(ofstream &of) {
        of << "{";
        for(int i = 0; i < 64; i++) {
            U64 single = SingleBitBoard(i);
            of << single;
            if(i != 63) of << ", ";
        }
        of << "}" << endl << endl;
    }

    void GeneratePrecomputtedKnights(ofstream &of) {
        of << "{";
        for(int i = 0; i < 64; i++) {
            U64 knightAtt = KnightAttacks(SingleBitBoard(i));
            of << knightAtt;
            if(i !=63) of << ", ";
        }
        of << "}" << endl << endl;
    }

    void GeneratePrecomputtedKings(ofstream &of) {
        of << "{";
        for(int i = 0; i < 64; i++) {
            U64 knightAtt = OneInAllDirection(SingleBitBoard(i));
            of << knightAtt;
            if(i !=63) of << ", ";
        }
        of << "}" << endl << endl;
    }

    void PrecomputtedWhitePawnAttacks(ofstream &of) {
        of << "{";
        for(int i = 0; i < 64; i++) {
            U64 pawnAtt = wPawnAllAtt(SingleBitBoard(i));
            of << pawnAtt;
            if(i !=63) of << ", ";
        }
        of << "}" << endl << endl;
    }

    void PrecomputtedBlackPawnAttacks(ofstream &of) {
        of << "{";
        for(int i = 0; i < 64; i++) {
            U64 pawnAtt = bPawnAllAtt(SingleBitBoard(i));
            of << pawnAtt;
            if(i !=63) of << ", ";
        }
        of << "}" << endl << endl;
    }

    void GeneratePrecomputted() {
        ofstream of("PrecomputtedMoveData.txt");
        GenerateSingleBitBoards(of);
        GeneratePrecomputtedKnights(of);
        GeneratePrecomputtedKings(of);
        PrecomputtedWhitePawnAttacks(of);
        PrecomputtedBlackPawnAttacks(of);
        of.close();
    }
}