#include "..\Bitboard\Bitboard.cpp"
#include "..\Bitboard\BitboardHelper.cpp"

int main () {
    PrecomputeData();
    Bitboard board;
    board.LoadFen("rnbqkbn1/pppppppp/5r2/8/3B4/8/PPP3PP/RN1QKBNR w KQq - 0 1");

    bitset<64> wBishopMoves = board.wBishopMoves();
    PrintBitSet(wBishopMoves, "wBishopMoves");
    bitset<64> bBishopMoves = board.bBishopMoves();
    PrintBitSet(bBishopMoves, "bBishopMoves");

    board.LoadFen("1nbqkbn1/pppppppp/5r2/6R1/2r5/P7/1PPPPPPP/RNBQKBN1 w Q - 0 1");
    bitset<64> wRookMoves = board.wRookMoves();
    PrintBitSet(wRookMoves, "wRookMoves");
    bitset<64> bRookMoves = board.bRookMoves();
    PrintBitSet(bRookMoves, "bRookMoves");

    
    /*
        bitset<64> bKnightmoves = board.bKnightMoves();
        bitset<64> wKnightmoves = board.wKnightMoves();

        bitset<64> wpawnmoves = board.wPawnMoves();
        bitset<64> bpawnmoves = board.bPawnMoves();

        bitset<64> wkingmoves = board.wKingMoves();
        bitset<64> bkingmoves = board.bKingMoves();

        bitset<64> notbboard = board.NotbBoard();
        bitset<64> notwboard = board.NotwBoard();

        PrintBitSet(wpawnmoves, "wpawnmoves");
        PrintBitSet(bpawnmoves, "bpawnmoves");
        PrintBitSet(wKnightmoves, "wKnightmoves");
        PrintBitSet(bKnightmoves, "bKnightmoves");
        PrintBitSet(wkingmoves, "wkingmoves");
        PrintBitSet(bkingmoves, "bkingmoves");     
    */  

}