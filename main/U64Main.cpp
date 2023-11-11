#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b(startFen);
    //b.LoadFen("1k1q2K1/8/8/8/8/8/8/8 w - - 1 107");
    Moves move_list;
    b.GenerateMoves(move_list);
    cout << b.IsDraw() << endl;

    int move = move_list.GetMove(4);
    b.MakeMove(move);
    b.GenerateMoves(move_list);
    cout << b.IsDraw() << endl;

    move = move_list.GetMove(20);
    b.MakeMove(move);
    b.GenerateMoves(move_list);
    cout << b.IsDraw() << endl;

    move = move_list.GetMove(0);
    b.MakeMove(move);
    b.GenerateMoves(move_list);
    cout << b.IsDraw() << endl;
    
    
}