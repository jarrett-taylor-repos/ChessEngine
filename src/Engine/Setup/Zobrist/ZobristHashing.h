#include "..\..\Utils\Extensions\Extensions.h"
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace Extensions;

namespace ZobristHashing {
    U64 rand64() {
        return rand() ^ ((U64)rand() << 15) ^ ((U64)rand() << 30) ^ ((U64)rand() << 45) ^ ((U64)rand() << 60);
    }

    //12 pieces for 64 squares  
    void GeneratePieceNumbers(ofstream &of) {
        of << "U64 zobristPieceSquareNumber[64][12] = {" << endl;
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
        of << "};" << endl;
    }

    //4 for castling rights
    void GenerateCastleNumbers(ofstream &of) {
        of << "U64 castlingNumbers[4] = {";
        for(int i = 0; i < 16; i++) {
            U64 n = rand64();
            if(i + 1 == 4) {
                of << n;
            } else {
                of << n << ",";
            }
        }
        of << "};" << endl;
    }


    //is white to move
    void GenerateColorNumbers(ofstream &of) {
        of << "U64 moveNumbers[2] = {";
        U64 n = rand64();
        U64 n2 = rand64();
        of << n  << ", " << n2 << "};" << endl;
    }

    //16 numbers for enpassant
    void GenerateEnpassantNumbers(ofstream &of) {
        of << "U64 enpassantNumbers[16] = {";
        for(int i = 0; i < 16; i++) {
            U64 n = rand64();
            if(i + 1 == 16) {
                of << n;
            } else {
                of << n << ",";
            }
        }
        of << "};" << endl;
    }

    void GenerateZobristNumbers() {
        ofstream of("zobristHash.txt");
        GeneratePieceNumbers(of);
        GenerateCastleNumbers(of);
        GenerateEnpassantNumbers(of);
        GenerateColorNumbers(of);
        of.close();
    }
}
