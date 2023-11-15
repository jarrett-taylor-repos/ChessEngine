#include "MagicExtensions.h"
#include "Moves.cpp"
#include <chrono>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unordered_set>
using namespace chrono;
using namespace std;
using namespace MagicExtensions;
using namespace MoveExtensions;

namespace Extensions { 
    //common U64 
    U64 GetLSB(U64 b) { return b & -b; }
    void RemoveLSB(U64 &b) { b ^= GetLSB(b); }

    //castling helper
    int SetCastlingRightsHelper(char ch) {
        switch (ch) {
            case 'K': return wk; 
            case 'Q': return wq;
            case 'k': return bk; 
            case 'q': return bq;
            default: return 0;
        }
    }

    int SetCastlingRights(string str) {
        int result = 0;
        if(str == "" || str == "-") return 0;
        for(int i = 0; i < str.size(); i++) {
            char ch = str[i];
            result |= SetCastlingRightsHelper(ch);
        }
        return result;
    }
    
    string CastlingRightsString(int castling) {
        if(castling == 0) return "-";

        string temp = "";
        if(castling & wk) temp += "K";
        if(castling & wq) temp += "Q";
        if(castling & bk) temp += "k";
        if(castling & bq) temp += "Q";
        return temp;
    }

    //enpassant 
    string EnpassantTargetToString(int index) { 
        if(index == 0) return "-";
        return squares_to_coordinates[index];
    }

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

    void Print(vector<int> vect, string name = "") {
        if(name.length() != 0) { cout << name << endl; }
        for(int i = 0; i < vect.size(); i++) {
            cout << to_string(vect[i]) << " ";
        }
        cout << endl << endl;
    }

    void Print(vector<string> uci, string name = "") {
        if(name.length() != 0) { cout << name << endl;}
        
        for(string move : uci) {
            cout << move << endl;
        }
        cout << endl << endl;
    }

    //zobrist 
    void SetZobristHash(U64 &zobrist, int boardSq, int piece) { zobrist ^= pieceNumbers[boardSq][piece]; }

    int EnpassantZobristIndex(int enPassantTarget) {
        if(enPassantTarget < 16 || enPassantTarget > 47) return -1;
        if(enPassantTarget < 24) return enPassantTarget - 16;
        return enPassantTarget - 32;
    }

    void SetZobristHash(U64 &zobrist, int enpassantTarget) {
        int index = EnpassantZobristIndex(enpassantTarget);
        if(index == -1) return;
        zobrist ^= enpassantNumbers[index];
    }

    void SetZobristHash(U64 &zobrist, bool isWhiteMove) {
        if(!isWhiteMove) zobrist ^= whiteMoveNumber;
    }

    int CastlingRightsToZobristIndex(int piece) {
        if(piece == K) return 0;
        if(piece == Q) return 1;
        if(piece == k) return 2;
        if(piece == q) return 3;
        return -1;
    }

    void SetCastlingZobrist(int piece, U64 &zobrist) {
        int index = CastlingRightsToZobristIndex(piece);
        if(index == -1) return;
        zobrist ^= castlingNumbers[index];
    }

    void SetCastlingZobrist(U64 &zobrist, int castlingRights) {
        if(!castlingRights) return;

        if(castlingRights & wk) zobrist ^= castlingNumbers[0];
        if(castlingRights & wq) zobrist ^= castlingNumbers[1];
        if(castlingRights & bk) zobrist ^= castlingNumbers[2];
        if(castlingRights & bq) zobrist ^= castlingNumbers[3];
    }
}