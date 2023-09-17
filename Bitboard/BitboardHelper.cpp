#include "Bitboardhelper.h"
#include "Extensions.h"
using namespace std;

BitBoardHelper::BitBoardHelper() {
    item = 0000;
    board = 0000000000000000000000000000000000000000000000000000000000000000;
}

void BitBoardHelper::SetFenAndPiece(string fen, short int value) {
    item = value;
    Extensions ext;
    fen = ext.Split(fen, ' ')[0];
    int square = 0;

    for(int i = 0; i < fen.length(); i++) {
        char curr = fen[i];
        if(curr != '/') {
            bool isInt = ext.TryParseCharToInt(curr);

            if(!isInt && item == FenCharToValue(curr)) {
                SetBit(square);
                square++;
            }

            if(isInt) {
                int value = ext.CharToInt(curr);
                square += value;
            } else {
                square++;
            }
        }
    }
}

void BitBoardHelper::SetBit(int square) {
    int mask = 1 << square;
    board = board | mask;
}

bool BitBoardHelper::GetBit(int square) {
    bool bit = board & (1 << square);
    return bit;
}

void BitBoardHelper::ClearBit(int square) {
    int mask = 1 << square;
    board = board & ~mask;
}


short int BitBoardHelper::FenCharToValue(char ch) {
    switch(ch) {
        case 'p':
            return 0001;
        case 'n':
            return 0010;
        case 'b':
            return 0011;
        case 'r':
            return 0100;
        case 'q':
            return 0101;
        case 'k':
            return 0110;
        case 'P':
            return 1001;
        case 'N':
            return 1010;
        case 'B':
            return 1011;
        case 'R':
            return 1100;
        case 'Q':
            return 1101;
        case 'K':
            return 1110;
    }
}