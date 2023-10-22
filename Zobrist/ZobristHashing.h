#include "..\Extensions\U64Extentions.h"
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace U64Extensions;

namespace ZobristHashing {
    U64 rand64() {
        return rand() ^ ((U64)rand() << 15) ^ ((U64)rand() << 30) ^ ((U64)rand() << 45) ^ ((U64)rand() << 60);
    }

    //12 pieces for 64 squares  
    void GeneratePieceNumbers(ofstream &of) {
        of << "{" << endl;
        for(int i = 0; i < 64; i++) {
            of<<"  {";
            for(int j = 0; j < 12; j++) {
                U64 n = rand64();
                if(j + 1 == 12) {
                    of << n;
                } else {
                    of << n << ",";
                }
            }
            of << "}" << endl;
        }
        of << "}" << endl << endl;
    }

    //4 for castling rights
    void GenerateCastleNumbers(ofstream &of) {
        of << "{";
        for(int i = 0; i < 16; i++) {
            U64 n = rand64();
            if(i + 1 == 4) {
                of << n;
            } else {
                of << n << ",";
            }
        }
        of << "}" << endl << endl;
    }


    //is white to move
    void GenerateWhiteNumber(ofstream &of) {
        of << "{";
        U64 n = rand64();
        of << n << "}" << endl << endl;
    }

    //16 numbers for enpassant
    void GenerateEnpassantNumbers(ofstream &of) {
        of << "{";
        for(int i = 0; i < 16; i++) {
            U64 n = rand64();
            if(i + 1 == 16) {
                of << n;
            } else {
                of << n << ",";
            }
        }
        of << "}" << endl << endl;
    }

    void GenerateZobristNumbers() {
        ofstream of("zobristHash.txt");
        GeneratePieceNumbers(of);
        GenerateCastleNumbers(of);
        GenerateWhiteNumber(of);
        GenerateEnpassantNumbers(of);
        of.close();
    }

    void InitializeDefaults() {
        for(int i = 0; i < 64; i++) {
            for(int j = 0; j < 12; j ++) pieceNumbers[i][j] = C64(0);
        }

        for(int i = 0; i < 4; i++) castlingNumbers[i] = C64(0);
        for(int i = 0; i < 16; i++) enpassantNumbers[i] = C64(0);
        whiteMoveNumber = C64(0);
    }
}
