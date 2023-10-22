#include "..\Extensions\U64Extentions.h"
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace U64Extensions;


namespace PrecomputtedMoveData {

    U64 noNoEa(U64 b) {return (b >> 15) & notAFile; };
    U64 noEaEa(U64 b) {return (b >> 6) & (notAFile & notBFile);};
    U64 soEaEa(U64 b) {return (b << 10) & (notAFile & notBFile);};
    U64 soSoEa(U64 b) {return (b << 17) & notAFile; };
    U64 soSoWe(U64 b) {return (b << 15) & notHFile; };
    U64 soWeWe(U64 b) {return (b << 6) & (notGFile & notHFile); };
    U64 noWeWe(U64 b) {return (b >> 10) & (notGFile & notHFile); };
    U64 noNoWe(U64 b) {return (b >> 17) & notHFile; };
    U64 KnightAttacks(U64 b) { return noNoEa(b) | noEaEa(b) | soEaEa(b) | soSoEa(b) | noNoWe(b) | noWeWe(b) | soWeWe(b) | soSoWe(b); };

    void GeneratePrecomputtedKnights(ofstream &of) {
        of << "{";
        for(int i = 0; i < 64; i++) {
            U64 knightAtt = KnightAttacks(SingleBitBoard(i));
            of << knightAtt;
            if(i !=63) of << ", ";
        }
        of << "}" << endl << endl;
    }

    void GeneratePrecomputted() {
        ofstream of("PrecomputtedMoveData.txt");
        GeneratePrecomputtedKnights(of);

        of.close();
    }
}