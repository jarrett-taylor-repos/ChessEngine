#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b;
    b.LoadFen("rnbq1k1r/1B1Pbp1p/p1p5/6p1/8/8/PPP1NnPP/RNBQK2R w KQ - 0 10");
    Moves move_list;
    b.GenerateMoves(move_list); 
    PrintMoveListUci(move_list);
}