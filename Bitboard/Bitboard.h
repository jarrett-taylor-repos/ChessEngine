#ifndef BITBOARD_H
#define BITBOARD_H
#include "bitboardhelper.h"

class Bitboard {
    private:
    //represent color and piece
    BitBoardHelper whitepawn;
    BitBoardHelper whiteknight;
    BitBoardHelper whitebishop;
    BitBoardHelper whiterook;
    BitBoardHelper whitequeen;
    BitBoardHelper whiteking;

    BitBoardHelper blackpawn;
    BitBoardHelper blackknight;
    BitBoardHelper blackbishop;
    BitBoardHelper blackook;
    BitBoardHelper blackqueen;
    BitBoardHelper blackking;

    public:

};

#endif