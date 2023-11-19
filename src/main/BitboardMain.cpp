#include "..\Bitboard\Bitboard.cpp"

int main () {
    InitAll();
    Bitboard b(startFen);
    b.PrintPretty();
    b.MakeMoveFromUci("e2e4");
    b.PrintPretty();
}