#include "..\Bitboard\Bitboard.cpp"
#include "..\Bitboard\BitboardHelper.cpp"
#include <chrono>
using namespace chrono;

int main () {
    Bitboard board; 
    board.LoadFen("1nb1kbnr/ppppqppp/3B4/8/4R3/2r5/PPPPPPPP/1NBQK1NR w Kk - 0 1");
    vector<string> ucimoves = board.GetUciMoves();
    string move = ucimoves[0];
    board.MakeMove(move);
    board.PrintAllBoards();


    /* UCI MOVE GENERATION TIMED
        auto start = high_resolution_clock::now();
                
        vector<string> wUciMoves = board.wUciMoves();

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "WHITE UCI MOVE GENERATION" << endl;
        cout << duration.count() << endl;

        start = high_resolution_clock::now();
            
        vector<string> bUciMoves = board.bUciMoves();


        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "BLACK UCI MOVE GENERATION" << endl;
        cout << duration.count() << endl;
    */

    /* UCI MOVES
        board.LoadFen("1nb1kbnr/ppppqppp/3B4/8/4R3/2r5/PPPPPPPP/1NBQK1NR w Kk - 0 1");
        vector<string> wpawnuci = board.wPawnMovesUCI();
        PrintVector(wpawnuci, "wpawnuci");
        vector<string> bpawnuci = board.bPawnMovesUCI();
        PrintVector(bpawnuci, "bpawnuci");

        vector<string> wKnightMovesUCI = board.wKnightMovesUCI();
        PrintVector(wKnightMovesUCI, "wKnightMovesUCI");
        vector<string> bKnightMovesUCI = board.bKnightMovesUCI();
        PrintVector(bKnightMovesUCI, "bKnightMovesUCI");

        vector<string> wKingMovesUCI = board.wKingMovesUCI();
        PrintVector(wKingMovesUCI, "wKingMovesUCI");
        vector<string> bKingMovesUCI = board.bKingMovesUCI();
        PrintVector(bKingMovesUCI, "bKingMovesUCI");

        vector<string> wBishopMovesUCI = board.wBishopMovesUCI();
        PrintVector(wBishopMovesUCI, "wBishopMovesUCI");
        vector<string> bBishopMovesUCI = board.bBishopMovesUCI();
        PrintVector(bBishopMovesUCI, "bBishopMovesUCI");

        vector<string> wRookMovesUCI = board.wRookMovesUCI();
        PrintVector(wRookMovesUCI, "wRookMovesUCI");
        vector<string> bRookMovesUCI = board.bRookMovesUCI();
        PrintVector(bRookMovesUCI, "bRookMovesUCI");

        vector<string> wQueenMovesUCI = board.wQueenMovesUCI();
        PrintVector(wQueenMovesUCI, "wQueenMovesUCI");
        vector<string> bQueenMovesUCI = board.bQueenMovesUCI();
        PrintVector(bQueenMovesUCI, "bQueenMovesUCI");

        vector<string> wUciMoves = board.wUciMoves();
        PrintVector(wUciMoves, "wUciMoves");
        vector<string> bUciMoves = board.bUciMoves();
        PrintVector(bUciMoves, "bUciMoves");
    */

    /* XRAY
        board.LoadFen("rnb1kbnr/ppppqppp/8/8/4R3/8/PPPPPPPP/1NBQKBNR w Kkq - 0 1");
        bitset<64> xRaywRookAttacks = board.xRaywRookAttacks();
        PrintBitSet(xRaywRookAttacks, "xRaywRookAttacks");

        board.LoadFen("rnb1kbnr/ppppqppp/3B4/8/4R3/8/PPPPPPPP/1NBQK1NR w Kkq - 0 1");
        bitset<64> xRaywBishopAttacks = board.xRaywBishopAttacks();
        PrintBitSet(xRaywBishopAttacks, "xRaywBishopAttacks");
    */

    /* TIME
        auto start = high_resolution_clock::now();
            
        bitset<64> wBishopMoves = board.wBishopMoves();
        bitset<64> bBishopMoves = board.bBishopMoves();


        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "BITSET: bishop moves in microseconds" << endl;
        cout << duration.count() << endl;

        start = high_resolution_clock::now();
            
        bitset<64> wRookMoves = board.wRookMoves();
        bitset<64> bRookMoves = board.bRookMoves();


        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "BITSET: bishop moves in microseconds" << endl;
        cout << duration.count() << endl;
    */

    /* MOVE GENERATION
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

        board.LoadFen("rnb1kbnr/pppppp1p/2q3p1/8/3Q4/P7/1PPPPPPP/RNB1KBNR w Q - 0 1");
        bitset<64> wQueenMoves = board.wQueenMoves();
        PrintBitSet(wQueenMoves, "wQueenMoves");
        bitset<64> bQueenMoves = board.bQueenMoves();
        PrintBitSet(bQueenMoves, "bQueenMoves");
    */  

}