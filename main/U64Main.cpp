#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b("2k5/2p5/4P1K1/1p6/3P2p1/8/p7/8 b - - 1 45");
    //U64Bitboard b(startFen);
    Moves movesList;
    b.GenerateMoves(movesList);
    PrintMoveListUci(movesList, true);
}