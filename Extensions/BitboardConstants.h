#include <iostream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <bitset>
typedef unsigned long long U64;
using namespace std;

namespace BitboardConstants {
    string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    bitset<64> hFile("1000000010000000100000001000000010000000100000001000000010000000");
    bitset<64> gFile("0100000001000000010000000100000001000000010000000100000001000000");
    bitset<64> fFile("0010000000100000001000000010000000100000001000000010000000100000");
    bitset<64> eFile("0001000000010000000100000001000000010000000100000001000000010000");
    bitset<64> dFile("0000100000001000000010000000100000001000000010000000100000001000");
    bitset<64> cFile("0000010000000100000001000000010000000100000001000000010000000100");
    bitset<64> bFile("0000001000000010000000100000001000000010000000100000001000000010");
    bitset<64> aFile("0000000100000001000000010000000100000001000000010000000100000001");

    bitset<64> rank1("1111111100000000000000000000000000000000000000000000000000000000");
    bitset<64> rank2("0000000011111111000000000000000000000000000000000000000000000000");
    bitset<64> rank3("0000000000000000111111110000000000000000000000000000000000000000");
    bitset<64> rank4("0000000000000000000000001111111100000000000000000000000000000000");
    bitset<64> rank5("0000000000000000000000000000000011111111000000000000000000000000");
    bitset<64> rank6("0000000000000000000000000000000000000000111111110000000000000000");
    bitset<64> rank7("0000000000000000000000000000000000000000000000001111111100000000");
    bitset<64> rank8("0000000000000000000000000000000000000000000000000000000011111111");

    bitset<64> bMult("00000000000001010000000000000101");
    bitset<64> rMutl8("00000000000000010000000010000001");
    bitset<64> rMutl7("00000000000000010000001000000001");
    bitset<64> rMutl3456("00000000000000010000001010000001");

    U64 U64_hFile = hFile.to_ullong();
    U64 U64_gFile = gFile.to_ullong();
    U64 U64_fFile = fFile.to_ullong();
    U64 U64_eFile = eFile.to_ullong();
    U64 U64_dFile = dFile.to_ullong();
    U64 U64_cFile = cFile.to_ullong();
    U64 U64_bFile = bFile.to_ullong();
    U64 U64_aFile = aFile.to_ullong();

    U64 U64_notHFile = (~hFile).to_ullong();
    U64 U64_notGFile = (~gFile).to_ullong();
    U64 U64_notFFile = (~fFile).to_ullong();
    U64 U64_notEFile = (~eFile).to_ullong();
    U64 U64_notDFile = (~dFile).to_ullong();
    U64 U64_notCFile = (~cFile).to_ullong();
    U64 U64_notBFile = (~bFile).to_ullong();
    U64 U64_notAFile = (~aFile).to_ullong();

    U64 U64_rank1 = rank1.to_ullong();
    U64 U64_rank2 = rank2.to_ullong();
    U64 U64_rank3 = rank3.to_ullong();
    U64 U64_rank4 = rank4.to_ullong();
    U64 U64_rank5 = rank5.to_ullong();
    U64 U64_rank6 = rank6.to_ullong();
    U64 U64_rank7 = rank7.to_ullong();
    U64 U64_rank8 = rank8.to_ullong();

    U64 U64_notRank1 = (~rank1).to_ullong();
    U64 U64_notRank2 = (~rank2).to_ullong();
    U64 U64_notRank3 = (~rank3).to_ullong();
    U64 U64_notRank4 = (~rank4).to_ullong();
    U64 U64_notRank5 = (~rank5).to_ullong();
    U64 U64_notRank6 = (~rank6).to_ullong();
    U64 U64_notRank7 = (~rank7).to_ullong();
    U64 U64_notRank8 = (~rank8).to_ullong();

    U64 U64_bMult = bMult.to_ullong();
    U64 U64_rMutl8 = rMutl8.to_ullong();
    U64 U64_rMutl7 = rMutl7.to_ullong();
    U64 U64_rMutl3456 = rMutl3456.to_ullong();
}