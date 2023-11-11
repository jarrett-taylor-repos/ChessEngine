#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b;
    b.LoadFen("5Q2/2kp4/5p2/1q3P1P/p3PK2/3r4/6B1/8 b - - 7 51");
    Moves move_list;
    b.GenerateMoves(move_list);
    b.MakeMove(move_list.GetMove(0));
    PrintMoveListUci(move_list, true);
    
    
}