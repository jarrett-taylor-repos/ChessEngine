#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b("rnbqkbnr/pppppppp/8/4r3/1b5q/4N3/PPPP1PPP/RNBQKB1R w KQkq - 0 1");

    U64 queenMoves = b.wQueenPsuedoMoves();
    U64 magicQueenMoves = b.wMagicQueenMoves();

    Print(queenMoves, "queenMoves");
    Print(magicQueenMoves, "magicQueenMoves");
}