#include "..\Extensions\Extentions.h"
#include <iostream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <bitset>
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
        char ch = PieceValueToChar(item);
        string str(1, ch);
        cout << str << endl;
        for(int i = 0; i < 64; i++) {
            if(i % 8 == 0 && i != 0) {
                cout << endl;
            }
            if(board[i] == 1) {
                cout << "1" << " ";
            } else {
                cout << "." <<  " ";
            }
        }
        cout << endl << endl;
    };

    bool GetBit(int square) { return board.test(square); };
    void ClearBoard() { board.reset(); };
    void ClearBit(int square) { board.reset(square); };
    void PrintBinaryValue() { cout << board << endl; };
    bitset<64> GetBoard() { return board; };
    short int GetItem() { return item; };
    long long BitsetToU64() {return board.to_ullong(); };

    short int FenCharToValue(char ch) {
        switch(ch) {
            case 'p': return 0001;
            case 'n': return 0010;
            case 'b': return 0011;
            case 'r': return 0100;
            case 'q': return 0101;
            case 'k': return 0110;
            case 'P': return 1001;
            case 'N': return 1010;
            case 'B': return 1011;
            case 'R': return 1100;
            case 'Q': return 1101;
            case 'K': return 1110;
            default: return 0000;
        }
    };

    char PieceValueToChar(short int item) {
        switch(item) { 
            case 0001: return 'p';
            case 0010: return 'n';
            case 0011: return 'b';
            case 0100: return 'r';
            case 0101: return 'q';
            case 0110: return 'k';
            case 1001: return 'P';
            case 1010: return 'N';
            case 1011: return 'B';
            case 1100: return 'R';
            case 1101: return 'Q';
            case 1110: return 'K';
            default: return ' ';
        }
    };
};