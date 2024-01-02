#include "..\Bitboard\Bitboard.cpp"

int main () {
    InitAll();
    string fen = "rnb1k2r/1pq2ppp/p3pn2/3p4/1b1NP3/P1N1BP2/1PPQ2PP/R3KB1R b KQkq - 0 9";
    Bitboard b(fen);
    b.PrintPretty();
    cout << b.GetFen();
    //b.PrintAllBoards();
}