#include "..\Bitboard\Bitboard.cpp"
#include "..\Bitboard\BitboardHelper.cpp"

int main () {
    PrecomputeData();
    Bitboard board;
    board.LoadFen("rnbqkbn1/pppppppp/5r2/8/3B4/8/PPP3PP/RN1QKBNR w KQq - 0 1");

    bitset<64> black = board.bBoard();
    bitset<64> white = board.wBoard();
    bitset<64> emptyboard = board.EmptyBoard();
    bitset<64> wbish = board.GetwBishop().GetBoard();
    vector<int> indexes = BitSetTrueIndexes(wbish);
    PrintBitSet(wbish, "wbish");
    //PrintVectorInt(indexes, "w bishops indexes");

    for(int sq : indexes) {
        bitset<64> diag = diagonals[sq];
        bitset<64> mask = maskBishop[sq];
        //PrintBitSet(diag, "diag " + to_string(sq));
        //PrintBitSet(mask, "mask " + to_string(sq));
        bitset<64> bishopattcks = mask & ~white | (mask & black);
        PrintBitSet((mask & black), "mask & black");
        PrintBitSet(bishopattcks, "bishopattcks");

    }
    //PrintBitSet(black, "black");
    //PrintBitSet(white, "white");
    //PrintBitSet(emptyboard, "emptyboard");

    
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