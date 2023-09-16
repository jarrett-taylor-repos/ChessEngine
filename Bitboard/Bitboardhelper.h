#ifndef BITBOARDHELPER_H
#define BITBOARDHELPER_H

class BitBoardHelper {
    private:
    unsigned long long board;
    short int item;

    public:
};

#endif

/*
first left biut is color
next 3 bits to the right are piece 

0 001 - black pawn
0 010 - black knight
0 011 - black bishop
0 100 - black rook
0 101 - black queen
0 110 - black king

1 001 - white pawn
1 010 - white knight
1 011 - white bishop
1 100 - white rook
1 101 - white queen
1 110 - white king

bool isWhite(short int item) {
    return ((item >> 3) || 0)
}

bool isPawn(short int item) {
    return (item || 001)
}
*/