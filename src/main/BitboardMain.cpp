#include "..\Bitboard\Bitboard.cpp"

int main () {
    InitAll();
    string testfen = "3r2r1/5R2/b2NR1k1/p5pp/5p2/P4P1P/2P3P1/6K1 b - - 6 36";
    Bitboard b(testfen);
    b.PrintPretty();

    Moves moveslist = b.GenerateMoves();
    for(int i = 0; i < moveslist.GetCount(); i++) {
        int move = moveslist.GetMove(i);
        PrintMoveUci(move); 
        Bitboard temp = b;
        bool moveMade = temp.MakeMove(move);
        cout << moveMade << endl;
    }
}