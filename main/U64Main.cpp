#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    //U64Bitboard b("1R6/1P1bq1bk/p2p2pp/1r1Pp3/8/6NP/1B2QPP1/6K1 w - - 1 35");
    U64Bitboard b(startFen);
    Moves movesList;
    b.PrintPretty();
    b.GenerateMoves(movesList);
    //PrintMoveListUci(movesList, true);
}