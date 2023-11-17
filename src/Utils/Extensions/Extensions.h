#include "MagicExtensions.h"
#include "ZobristExtensions.h"
#include "CastlingExtensions.h"
#include "Moves.cpp"
#include <chrono>
using namespace chrono;
using namespace MagicExtensions;
using namespace ZobristExtensions;
using namespace MoveExtensions;
using namespace CastlingExtensions;

namespace Extensions { 
    //common U64 
    U64 GetLSB(U64 b) { return b & -b; }
    void RemoveLSB(U64 &b) { b ^= GetLSB(b); }

    //move insert
    void FindAndInsertMoves(Moves &movesList, int source, U64 moves, int piece, bool isWhite, bool isPawnCap, int enpassantTarget, U64 possibleCaptures) {
        int move = 0;
        while(moves != Empty) {
            int target = GetLSBIndex(moves);
            bool isPawn = (piece == P) || (piece == p);
            bool isPromo = (isWhite && isPawn && (target < a7)) || (!isWhite && isPawn && (target > h2));
            bool isDoublePawnPush = isPawn && abs(target-source) == 16;
            bool isCastle = ((piece == K) || (piece == k)) && abs(target-source) == 2;
            bool isEnpassant = isPawn && target == enpassantTarget;
            bool isCap = isPawnCap || TestBit(possibleCaptures, target);

            if(isPromo) {
                move = encodeMove(source, target, piece, 1, isCap, 0, 0, 0);
                movesList.AddMove(move);
                move = encodeMove(source, target, piece, 2, isCap, 0, 0, 0);
                movesList.AddMove(move);
                move = encodeMove(source, target, piece, 4, isCap, 0, 0, 0);
                movesList.AddMove(move);
                move = encodeMove(source, target, piece, 8, isCap, 0, 0, 0);
                movesList.AddMove(move);
            } else {
                move = encodeMove(source, target, piece, 0, isCap, isDoublePawnPush, isEnpassant, isCastle);
                movesList.AddMove(move);
            }

            PopBit(moves, target);
        }
    }

    //enpassant 
    string EnpassantTargetToString(int index) { 
        if(index == 0) return "-";
        return squares_to_coordinates[index];
    }

    //common helpers 
    vector<string> Split(string str, const char token = ' '){
        string tmp; 
        stringstream ss(str);
        vector<string> words;

        while(getline(ss, tmp, token))  {
            words.push_back(tmp);
        }
        return words;
    }
    short int CharToInt(char ch) { string str(1, ch); return stoi(str); };
    int StringtoIndex(string str) {return SquaretoIntMap.at(str); }
    int CharToPiece(char c) {
        switch(c) {
            case 'p': return p;
            case 'b': return b;
            case 'n': return n;
            case 'r': return r;
            case 'q': return q;
            case 'k': return k;
            case 'P': return P;
            case 'B': return B;
            case 'N': return N;
            case 'R': return R;
            case 'Q': return Q;
            case 'K': return K;
        }
    }

    //PRINT methods
    void Print(U64 b, string name = "") {
        if(name != "") cout << name << endl;

        for(int i = 0; i < 64; i++) {

            if(i != 0 && (i % 8) == 0) { cout << "   " << to_string(9 - i/8) <<  endl; }
            TestBit(b, i) ? cout << "1 " : cout << ". ";
        }
        cout << "   " << 1;
        cout << endl << endl << "A B C D E F G H " << endl;
        cout << endl << endl;
    }
}