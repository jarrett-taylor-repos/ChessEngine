#ifndef BITBOARD_H
#define BITBOARD_H
#include "bitboardhelper.h"

class Bitboard {
    private:
    //represent color and piece
    BitBoardHelper Whitepawn;
    BitBoardHelper Whiteknight;
    BitBoardHelper Whitebishop;
    BitBoardHelper Whiterook;
    BitBoardHelper Whitequeen;
    BitBoardHelper Whiteking;

    BitBoardHelper Blackpawn;
    BitBoardHelper Blackknight;
    BitBoardHelper Blackbishop;
    BitBoardHelper Blackook;
    BitBoardHelper Blackqueen;
    BitBoardHelper Blackking;

    public:
    Bitboard::Bitboard(std::string);
};

#endif