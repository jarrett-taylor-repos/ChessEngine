#include "..\Bitboard\Bitboard.cpp"

int main () {
    InitAll();
    Bitboard b(startFen);
    b.PrintPretty();
    b.PrintAllBoards();
}