#include "..\Extensions\Extentions.h"
#include <iostream>
#include <ctype.h>
#include <stdio.h>
using namespace Extensions;
#pragma once


class U64_BitBoardHelper {
    private:
    U64 board;
    short int item;

    public:
    U64_BitBoardHelper() {
        item = 0000;
        board = U64(0);
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

                if(!isInt && item == convertValue) { board |= U64(1) << square; }

                if(isInt) {
                    int value = CharToInt(curr);
                    square += value;
                } else {
                    square++;
                }
            }
        }
    };

    void PrintU64() {
        char ch = PieceValueToChar(item);
        string str(1, ch);
        cout << str << endl;
        for(int i = 0; i < 64; i++) {
            if(i % 8 == 0 && i != 0) cout << endl;
            if(get_bit(board, i)) {
                cout << "1" << " ";
            } else {
                cout << "." << " ";
            }
        }
        cout << endl << endl;
    };

    bool GetBit(int square) { return get_bit(board, square); };
    void ClearBoard() { board = 0; };
    void ClearBit(int square) { bit_clear(board, square); };
    void PrintBinaryValue() { cout << board << endl; };
    U64 GetBoard() { return board; };
    short int GetItem() { return item; };

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