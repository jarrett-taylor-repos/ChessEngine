#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    U64Bitboard b("r1b1k2r/ppppqpbp/8/P5P1/3P4/1n2K3/8/RNBQ1BNR w k - 0 21");
    vector<string> uci = MapMovesToUCI(b.GetMapMoves());
    Print(uci, "uci");


    /* Knight indexes speed update 
        auto start = high_resolution_clock::now();
        vector<int> indexes = GetTrueBits(b.GetwKnight());
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        cout << "Time to get GetTrueBits: " + to_string(duration.count()) << endl;

        start = high_resolution_clock::now();
        vector<int> windexes = b.GetwKnightIndexes();
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        cout << "Time to get all U64 moves: " + to_string(duration.count()) << endl;
    */

    /*  SINGLE check - king moves back and captures a piece 
        U64Bitboard b("7Q/3k2p1/1p3n2/8/8/8/3bKq2/1N5R w  - 0 19");
        multimap<int, pair<int, char>> mapmoves = b.GetMapMoves();
        Print(mapmoves, "mapmoves");
    */

    /* 3 move repition
        vector<string> moves = {"e2e4", "e7e5", "e1e2", "e8e7", "e2e1", "e7e8", "e1e2", "e8e7", "e2e1", "e7e8", "e1e2"};
        b.MakeMove(moves);

        cout << b.is3FoldRepition();
    */

    /*  ZOBRIST
        U64Bitboard b;
        U64 zobrist = b.GetZobrist();
        cout << zobrist << endl;

        U64Bitboard test("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
        U64 zobristtest = test.GetZobrist();
        cout << zobristtest << endl;
    */

    /* CASTLING
        U64Bitboard b("rn1qk1nr/pppppppp/8/8/2P5/1N1P2P1/PPb1PPbP/R3K2R b KQkq - 0 1");
        //castle both sides - "r3k2r/1ppbqppp/p1nbpn2/8/8/2NBPN2/PPPBQPPP/R3K2R b KQkq - 4 9" ,  b.MakeMove("e8c8"); - PASS 
        //castle but in check - "rnbqk1nr/pppppppp/8/8/1bP5/1N1P4/PP2PPPP/R3K2R w KQkq - 0 1" - PASS
        //castle but squares attacked - "rn1qk1nr/pppppppp/8/8/2P5/1N1P2P1/PPb1PPbP/R3K2R w KQkq - 0 1" - PASS
        //caste but after rook is captured = "rn1qk1nr/pppppppp/8/8/2P5/1N1P2P1/PPb1PPbP/R3K2R b KQkq - 0 1" b.MakeMove("g2h1"); - PASS
        string castling = b.GetCastlingRights();
        cout << castling << endl;
        b.MakeMove("g2h1");
        castling = b.GetCastlingRights();
        cout << castling << endl;
        vector<string> uci = MapMovesToUCI(b.GetMapMoves());
        Print(uci, "uci");
    */

    /* en passant
        U64Bitboard b("rnbqkbnr/1ppp1ppp/p7/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 3");
        multimap<int, pair<int, char>> mapmoves = b.GetMapMoves();
        //Print(mapmoves, "mapmoves");
        vector<string> uci = MapMovesToUCI(mapmoves);
        Print(uci, "uci");
    */

    /* Move reptiition and hash 
        b.MakeMove("e2e4");
        b.MakeMove("e7e5");
        b.MakeMove("e1e2");
        b.MakeMove("e8e7");
        b.MakeMove("e2e1");
        b.MakeMove("e7e8");
        b.MakeMove("e1e2");
        b.MakeMove("e8e7");
        b.MakeMove("e2e1");
        b.MakeMove("e7e8");
        b.MakeMove("e1e2");
        b.MakeMove("e8e7");
        b.MakeMove("e2e1");
        b.MakeMove("e7e8");

        map<string, int> hashfen = b.GetFenHash();
        Print(hashfen, "hashfen");
    */

    /* double check 
        /r3k1nr/p1pppppp/1p2qbb1/8/8/2Q2n2/PPPP1PPP/RNB1KBNR w q - 0 1")
        u64.LoadFen("r3k1nr/p1pppppp/1p2qbb1/8/5n2/3B1N2/PPPPKPPP/RNBQ1R2 w q - 0 1");
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