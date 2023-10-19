#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    U64Bitboard u64("rn2k1nr/p1pppppp/1p2qbb1/8/3K4/8/PPPPPPPP/RNBQ1BNR w q - 0 1");
    U64 attacks = u64.GetAttacks();
    multimap<int, pair<int, char>> moves = u64.GetMapMoves();
    Print(moves, "moves");


    /* other 
        //r3k1nr/p1pppppp/1p2qbb1/8/5n2/3B1N2/PPPPKPPP/RNBQ1R2 w q - 0 1
        u64.LoadFen("r3k1nr/p1pppppp/1p2qbb1/8/5n2/3B1N2/PPPPKPPP/RNBQ1R2 w q - 0 1");//r3k1nr/p1pppppp/1p2qbb1/8/8/2Q2n2/PPPP1PPP/RNB1KBNR w q - 0 1")
        multimap<int, pair<int, char>> moves = u64.GetMapMoves();
        Print(moves, "moves");
    */

    /* SINGLE CHECK - king moves back across diagonal 
        u64.LoadFen("rn2k1nr/p1pppppp/1p2qbb1/8/3K4/8/PPPPPPPP/RNBQ1BNR w q - 0 1");
        multimap<int, pair<int, char>> moves = u64.GetMapMoves();
        Print(moves, "moves");
    */

    /* SINGLE CHECK 
        u64.LoadFen("rnbqkbnr/ppppp1pp/5p2/7Q/4P3/8/PPPP1PPP/RNB1KBNR b KQkq - 1 2"); //check

        multimap<int, pair<int, char>> mapmoves;
        u64.GetMapMoves(mapmoves);
        Print(mapmoves, "mapmoves");
    */

    /* Pinned pieces moves - no checks
        u64.LoadFen("rnb1k1n1/pppppppp/8/4r3/1b5q/4N3/PPPP1PPP/RNBQKB1R w KQq - 0 1");
        map<int, U64> pins = u64.GetBlockerPinnedToKingMovesMap();
        Print(pins, "pins");

        multimap<int, pair<int, char>> mapmoves;
        u64.GetMapMoves(mapmoves);
        Print(mapmoves, "mapmoves");
    */

    /* 
        U64 moves = u64.bRookMoves();
        U64 pieceBoard = u64.GetbRook();
        U64 colorBoard = u64.wBoard();
        U64 pieceColorBoard = u64.bBoard();

        U64 occ = u64.AllBoard();
        U64 blockers = moves & occ;
        Print(occ, "occ");
        Print(blockers, "blockers");
        U64 occAttacks = u64.RookAttacks(pieceBoard, occ ^ blockers);
        U64 movesXORoccatt = moves ^ occAttacks;
        Print(occAttacks, "occAttacks");
        Print(movesXORoccatt, "movesXORoccatt");
        Print(colorBoard, "colorBoard");
        U64 xrayattcks = (movesXORoccatt) & colorBoard;

        U64 test = occAttacks & ~occ;
        Print(test, "test");
        U64 pinnedToKingPotentialMoves = (( C64(0)) |(movesXORoccatt & ~pieceColorBoard) )^ xrayattcks;
        Print(pinnedToKingPotentialMoves, "pinnedToKingPotentialMoves");
        return xrayattcks;

        U64 xrayatt = u64.xRayAttacks();
        Print(xrayatt, "xrayatt");
    */

    /* XRAY ROOK
        u64.LoadFen("rnb1kbnr/ppppqppp/8/8/4R3/8/PPPPPPPP/1NBQKBNR w Kkq - 0 1"); //x ray rook
        U64 wrookmoves = u64.wRookMoves();
        U64 occ = u64.bBoard();
        U64 blockers = wrookmoves & occ;
        U64 occXORblockers = occ ^ blockers;
        //blockers &= wrookatt;
        Print(blockers, "blockers");
        Print(occXORblockers, "occ ^ blockers");
        U64 occAttacks = u64.RookAttacks(u64.GetwRook(), occXORblockers);
        U64 xrayattcks = (wrookmoves ^ occAttacks) & u64.bBoard();

        Print(occAttacks, "occAttacks");
        Print(xrayattcks, "xrayattcks");

        U64 xrayrookatt = u64.xRaywRookAttacks();
        Print(xrayrookatt, "xrayrookatt");
    */

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