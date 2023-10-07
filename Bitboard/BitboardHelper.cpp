#include "..\Extensions\Extentions.h"
#include <iostream>
#include <ctype.h>
#include <stdio.h>
using namespace std;
using namespace Extensions;
#pragma once


class BitBoardHelper {
    private:
    bitset<64> board;
    short int item;

    public:
    BitBoardHelper() {
        item = 0000;
        board = 0000000000000000000000000000000000000000000000000000000000000000;
    };

    void LoadFen(string fen, short int value) {
        item = value;
        fen = Split(fen, ' ')[0];
        int square = 0;

        for(int i = 0; i < fen.length(); i++) {
            char curr = fen[i];
            if(curr != '/') {
                bool isInt = isdigit(curr);
                short int convertValue = FenCharToValue(curr);

                if(!isInt && item == convertValue) { board.set(square); }

                if(isInt) {
                    int value = CharToInt(curr);
                    square += value;
                } else {
                    square++;
                }
            }
        }
    };

    void PrintBitBoard() {
        string str(1, PieceValueToChar(item));
        PrintBitSet(board, str);
    };

    bool GetBit(int square) { return board.test(square); };
    void ClearBoard() { board.reset(); };
    void ClearBit(int square) { board.reset(square); };
    void PrintBinaryValue() { cout << board << endl; };
    bitset<64> GetBoard() { return board; };
    short int GetItem() { return item; };
    long long ToU64() {return board.to_ullong(); };

    short int FenCharToValue(char ch) {
        switch(ch) {
            case 'p': return 0b0001;
            case 'n': return 0b0010;
            case 'b': return 0b0011;
            case 'r': return 0b0100;
            case 'q': return 0b0101;
            case 'k': return 0b0110;
            case 'P': return 0b1001;
            case 'N': return 0b1010;
            case 'B': return 0b1011;
            case 'R': return 0b1100;
            case 'Q': return 0b1101;
            case 'K': return 0b1110;
            default: return 0b0000;
        }
    };

    char PieceValueToChar(short int item) {
        switch(item) { 
            case 0b0001: return 'p';
            case 0b0010: return 'n';
            case 0b0011: return 'b';
            case 0b0100: return 'r';
            case 0b0101: return 'q';
            case 0b0110: return 'k';
            case 0b1001: return 'P';
            case 0b1010: return 'N';
            case 0b1011: return 'B';
            case 0b1100: return 'R';
            case 0b1101: return 'Q';
            case 0b1110: return 'K';
            default: return ' ';
        }
    };
};