#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    //U64Bitboard b(startFen);
    Moves movesList;
    b.GenerateMoves(movesList);
    int move = movesList.GetMove(13);
    cout << GetMoveUci(move);
    //PrintMoveListUci(movesList, true);
}