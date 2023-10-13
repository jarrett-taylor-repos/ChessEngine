#include <iostream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <bitset>
#include <iterator>
#include <map>
#include <sstream>
#include <vector>
using namespace std;

typedef unsigned long long U64; 
#define C64(constantU64) constantU64##ULL

namespace U64BitboardConstants {
    pair<char, int> wKingCastle = pair<char, int> ('K', false);
    pair<char, int> wQueenCastle = pair<char, int> ('Q', false);
    pair<char, int> bKingCastle = pair<char, int> ('k', false);
    pair<char, int> bQueenCastle = pair<char, int> ('q', false);
    map<char, bool> castlingRightsDefault = {wKingCastle, wQueenCastle, bKingCastle, bQueenCastle};
    string promotionUci = "qrbn";
    string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    const U64 notHFile = 0x7F7F7F7F7F7F7F7F;
    const U64 notGFile = 0xBFBFBFBFBFBFBFBF;
    const U64 notFFile = 0xDFDFDFDFDFDFDFDF;
    const U64 notEFile = 0xEFEFEFEFEFEFEFEF;
    const U64 notDFile = 0xF7F7F7F7F7F7F7F7;
    const U64 notCFile = 0xFBFBFBFBFBFBFBFB;
    const U64 notBFile = 0xFDFDFDFDFDFDFDFD;
    const U64 notAFile = 0xFEFEFEFEFEFEFEFE;

    const U64 hFile = 0x8080808080808080;
    const U64 gFile = 0x4040404040404040;
    const U64 fFile = 0x2020202020202020;
    const U64 eFile = 0x1010101010101010;
    const U64 dFile = 0x0808080808080808;
    const U64 cFile = 0x0404040404040404;
    const U64 bFile = 0x0202020202020202;
    const U64 aFile = 0x0101010101010101;

    const U64 rank1 = 0xFF00000000000000;
    const U64 rank2 = 0x00FF000000000000;
    const U64 rank3 = 0x0000FF0000000000;
    const U64 rank4 = 0x000000FF00000000;
    const U64 rank5 = 0x00000000FF000000;
    const U64 rank6 = 0x0000000000FF0000;
    const U64 rank7 = 0x000000000000FF00;
    const U64 rank8 = 0x00000000000000FF;

    const U64 notRank1 = 0x00FFFFFFFFFFFFFF;
    const U64 notRank2 = 0xFF00FFFFFFFFFFFF;
    const U64 notRank3 = 0xFFFF00FFFFFFFFFF;
    const U64 notRank4 = 0xFFFFFF00FFFFFFFF;
    const U64 notRank5 = 0xFFFFFFFF00FFFFFF;
    const U64 notRank6 = 0xFFFFFFFFFF00FFFF;
    const U64 notRank7 = 0xFFFFFFFFFFFF00FF;
    const U64 notRank8 = 0xFFFFFFFFFFFFFF00;


    enum enumSquare {
        a1, b1, c1, d1, e1, f1, g1, h1,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a8, b8, c8, d8, e8, f8, g8, h8
    };

    map <int, string> IntToSquareMap = {
        {0, "a8"}, {1, "b8"}, {2, "c8"}, {3, "d8"}, {4, "e8"}, {5, "f8"}, {6, "g8"}, {7, "h8"},
        {8, "a7"}, {9, "b7"}, {10, "c7"}, {11, "d7"}, {12, "e7"}, {13, "f7"}, {14, "g7"}, {15, "h7"},
        {16, "a6"}, {17, "b6"}, {18, "c6"}, {19, "d6"}, {20, "e6"}, {21, "f6"}, {22, "g6"}, {23, "h6"},
        {24, "a5"}, {25, "b5"}, {26, "c5"}, {27, "d5"}, {28, "e5"}, {29, "f5"}, {30, "g5"}, {31, "h5"},
        {32, "a4"}, {33, "b4"}, {34, "c4"}, {35, "d4"}, {36, "e4"}, {37, "f4"}, {38, "g4"}, {39, "h4"},
        {40, "a3"}, {41, "b3"}, {42, "c3"}, {43, "d3"}, {44, "e3"}, {45, "f3"}, {46, "g3"}, {47, "h3"},
        {48, "a2"}, {49, "b2"}, {50, "c2"}, {51, "d2"}, {52, "e2"}, {53, "f2"}, {54, "g2"}, {55, "h2"},
        {56, "a1"}, {57, "b1"}, {58, "c1"}, {59, "d1"}, {60, "e1"}, {61, "f1"}, {62, "g1"}, {63, "h1"}
    };

    map <string, int> SquaretoIntMap = {
        {"a8", 0}, {"b8", 1}, {"c8", 2}, {"d8", 3}, {"e8", 4}, {"f8", 5}, {"g8", 6}, {"h8", 7},
        {"a7", 8}, {"b7", 9}, {"c7", 10}, {"d7", 11}, {"e7", 12}, {"f7", 13}, {"g7", 14}, {"h7", 15},
        {"a6", 16}, {"b6", 17}, {"c6", 18}, {"d6", 19}, {"e6", 20}, {"f6", 21}, {"g6", 22}, {"h6", 23},
        {"a5", 24}, {"b5", 25}, {"c5", 26}, {"d5", 27}, {"e5", 28}, {"f5", 29}, {"g5", 30}, {"h5", 31},
        {"a4", 32}, {"b4", 33}, {"c4", 34}, {"d4", 35}, {"e4", 36}, {"f4", 37}, {"g4", 38}, {"h4", 39},
        {"a3", 40}, {"b3", 41}, {"c3", 42}, {"d3", 43}, {"e3", 44}, {"f3", 45}, {"g3", 46}, {"h3", 47},
        {"a2", 48}, {"b2", 49}, {"c2", 50}, {"d2", 51}, {"e2", 52}, {"f2", 53}, {"g2", 54}, {"h2", 55},
        {"a1", 56}, {"b1", 57}, {"c1", 58}, {"d1", 59}, {"e1", 60}, {"f1", 61}, {"g1", 62}, {"h1", 63}
    };
}