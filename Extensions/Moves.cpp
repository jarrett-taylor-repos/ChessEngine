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