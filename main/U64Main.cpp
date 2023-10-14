#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    U64Bitboard u64;
    u64.LoadFen("1r2kbnr/p1p1pppp/p1p1b3/8/1n1q1P2/8/6PP/RNBQKBNR b KQk f3 0 12");
    multimap<int, pair<int, char>> mapmoves;
    u64.GetMapMoves(mapmoves);
    Print(mapmoves, "mapmoves");

    //u64.PrintAllBoards();


    /*
        u64.LoadFen("r3k2r/pp1bqppp/2n1pn2/2pp2B1/1bPP4/2N1PN2/PP1QBPPP/R3K2R w KQkq - 5 9");//castling

        auto start = high_resolution_clock::now();
        U64 moves =  u64.GetMoves();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get all U64 moves: " + to_string(duration.count()) << endl;

        start = high_resolution_clock::now();
        bool movemade = u64.MakeMove("e1c1");//e1g1
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get make move: " + to_string(duration.count()) << endl;

        u64.PrintAllBoards();

        string currfen = u64.GetFen();
        cout << currfen << endl;

        multimap<int, pair<int, char>> mapmoves;
        start = high_resolution_clock::now();
        u64.GetMapMoves(mapmoves);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get get stored moves: " + to_string(duration.count()) << endl;


    start = high_resolution_clock::now();
    U64Bitboard copy =  u64;
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Time to copy board: " + to_string(duration.count()) << endl;

    start = high_resolution_clock::now();
    int materialadv =  u64.GetMaterialValue();
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Time find material advantage of "<<materialadv<<": " + to_string(duration.count()) << endl;

    //Print(mapmoves, "mapmoves");

    // u64.PrintAllBoards();
        start = high_resolution_clock::now();
        movemade = u64.MakeMove("e8c8");//e8g8
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get make move: " + to_string(duration.count()) << endl;
        string fen = u64.GetFen();
        cout << fen << endl;

        auto start = high_resolution_clock::now();
        U64 moves =  u64.GetMoves();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get all U64 moves: " + to_string(duration.count()) << endl;

        u64.LoadFen("r1bqkbnr/pPppppp1/2n5/8/8/5N2/P1PPPPpP/RNBQKB1R w Qkq - 0 1");//promotion
        string move = "b7a8q";
        start = high_resolution_clock::now();
        bool moveMade =  u64.MakeMove(move);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get make a move moves: " + to_string(duration.count()) << endl;

        u64.LoadFen("rnbqkbnr/1ppp1ppp/p7/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 3"); //en passant 

        multimap<int, pair<int, char>> mapmoves;
        start = high_resolution_clock::now();
        u64.GetMapMoves(mapmoves);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get get stored moves: " + to_string(duration.count()) << endl;
        

        move = "d5e6";
        start = high_resolution_clock::now();
        moveMade =  u64.MakeMove(move);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get make a move moves: " + to_string(duration.count()) << endl;
    */


    /* 
        U64 allmoves = u64.GetMoves();
        U64 pawnMoves = u64.GetPawnMoves();
        U64 knightMoves = u64.GetKnightMoves();
        U64 bishopMoves = u64.GetBishopMoves();
        U64 rookMoves = u64.GetRookMoves();
        U64 queenMoves = u64.GetQueenMoves();
        U64 kingMoves = u64.GetKingMoves();

        Print(allmoves, "allmoves");
        Print(pawnMoves, "pawnMoves");
        Print(knightMoves, "knightMoves");
        Print(bishopMoves, "bishopMoves");
        Print(rookMoves, "rookMoves");
        Print(queenMoves, "queenMoves");
        Print(kingMoves, "kingMoves");
    */
}