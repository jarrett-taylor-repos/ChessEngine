#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    //U64Bitboard b("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    U64Bitboard b(startFen);
    Moves move_list;
    b.GeneratebbMoves(move_list);

    int move = move_list.GetMove(18);
    PrintMoveUci(move);
    b.MakeMove(move);

    b.GeneratebbMoves(move_list);
    PrintMoveListUci(move_list);


}