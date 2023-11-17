#include "..\Constants\Constants.h"
using namespace Constants;

#include <stdio.h>
#include <string.h>
/*
    0000 0000 0000 0000 0011 1111  source square       0x3f
    0000 0000 0000 1111 1100 0000  target sqaure       0xfc0
    0000 0000 1111 0000 0000 0000  piece               0xf000
    0000 1111 0000 0000 0000 0000  promoted piece      0xf0000
    0001 0000 0000 0000 0000 0000  capture flag        0x100000
    0010 0000 0000 0000 0000 0000  double push flag    0x200000
    0100 0000 0000 0000 0000 0000  enpassant flag      0x400000
    1000 0000 0000 0000 0000 0000  castling flag       0x800000
*/
#define encodeMove(startSq, targetSq, piece, promoted, capture, doublePawnPush, enpassant, castling) \
    startSq | (targetSq << 6) | (piece << 12) | (promoted << 16) | (capture << 20) | (doublePawnPush << 21) | (enpassant << 22) | (castling << 23)

#define getMoveSource(move) (move & 0x3f)
#define getMoveTarget(move) ((move & 0xfc0) >> 6)
#define getMovePiece(move) ((move & 0xf000) >> 12)
#define getMovePromoted(move) ((move & 0xf0000) >> 16)
#define getMoveCapture(move) (move & 0x100000)
#define getMoveDouble(move) (move & 0x200000)
#define getMoveEnpassant(move) (move & 0x400000)
#define getMoveCastling(move) (move & 0x800000)

class Moves {
    int moves[256];
    int count;

    public:
    Moves() {
        count = 0;
        memset(moves, 0, sizeof(moves));
    };

    void AddMove(int move) {
        moves[count] = move;
        count++;
    };

    void Clear() {
        count = 0;
        memset(moves, 0, sizeof(moves));
    };

    int GetCount() { return count; };
    int GetMove(int index) {
        if(index > count) return 0;
        return moves[index];
    };

    void SetCount(int value) { count = value; };
};

namespace MoveExtensions {
    int GetPromoPiece(int promoP, bool isWhiteMove) {
        if(!promoP) return 0;

        switch(promoP) {
            case 1: return isWhiteMove ? Q : q;
            case 2: return isWhiteMove ? R : r; 
            case 4: return isWhiteMove ? B : b; 
            case 8: return isWhiteMove ? N : n; 
            default: return 0;
        }
        return 0;
    }

    char GetPromoPieceChar(int move) {
        int promoM = getMovePromoted(move);
        if(!promoM) return ' ';

        switch (promoM) {
            case 1: return 'q';
            case 2: return 'r';
            case 4: return 'b';
            case 8: return 'n';
            default: return ' ';
        }
    }

    string GetMoveUci(int move) {
        string start = squares_to_coordinates[getMoveSource(move)];
        string end = squares_to_coordinates[getMoveTarget(move)];
        string str = start + end + GetPromoPieceChar(move);
        return str;
    }

    int GetMoveByUci(Moves movesList, string uci) {
        for(int i = 0; i < movesList.GetCount(); i++) {
            int move = movesList.GetMove(i);
            string ucimove = GetMoveUci(move);
            if(ucimove == uci) return move;
        }
        return 0;
    }

    void PrintMoveUci(int move, int nodes = 0) {
        cout << squares_to_coordinates[getMoveSource(move)] 
            << squares_to_coordinates[getMoveTarget(move)] 
            << GetPromoPieceChar(move);

        if(nodes) cout << ": " << nodes;
        cout << endl;
    }

    void PrintMoveListUci(Moves movesList, bool withNumbers = false) {
        cout << "Move Count: " << movesList.GetCount() << endl;
        for(int i = 0; i < movesList.GetCount(); i++){
            if(withNumbers) cout << i << " - ";
            PrintMoveUci(movesList.GetMove(i));
        }
    }

    void PrintMove(int move) {
        int source = getMoveSource(move);
        int target = getMoveTarget(move);
        int piece = getMovePiece(move);
        int promoted = getMovePromoted(move);
        int capture = getMoveCapture(move);
        int doublePush = getMoveDouble(move);
        int enpassant = getMoveEnpassant(move);
        int castling = getMoveCastling(move);


        string str_start = squares_to_coordinates[source];
        string str_target = squares_to_coordinates[target];
        char str_piece = *ascii_pieces[piece];
        char str_promoted = GetPromoPieceChar(move);
        char str_capture = capture ? '1' : '0';
        char str_double = doublePush ? '1' : '0';
        char str_enpassant = enpassant ? '1' : '0';
        char str_castling = castling ? '1' : '0';

        cout << str_start 
            << str_target 
            << str_piece
            << str_promoted
            << str_capture
            << str_double
            << str_enpassant
            << str_castling
            << endl;
    }
}