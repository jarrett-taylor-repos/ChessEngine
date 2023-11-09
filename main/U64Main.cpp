#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b;
    b.LoadFen(startFen);
    Moves move_list;
    b.GenerateMoves(move_list); 

    U64Bitboard temp = b;
    bool tester = temp == b;
    cout << tester << endl;
    int move = move_list.GetMove(0);
    PrintMoveUci(move);
    temp.MakeMove(move);
    tester = temp == b;
    cout << tester;
}