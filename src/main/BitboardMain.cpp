#include "..\Bitboard\Bitboard.cpp"

int main () {
    InitAll();
    Bitboard b(startFen);
    b.PrintPretty();
    cout << b.GetEvaluation() << endl;

    Bitboard temp("rnbqkbnr/pppppppp/8/8/3PP3/2NBBN2/PPP2PPP/R2Q1RK1 w kq - 0 1");
    cout << temp.GetEvaluation() << endl;
}