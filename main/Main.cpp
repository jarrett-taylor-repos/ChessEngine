//#include "..\Bitboard\Bitboard.cpp"
#include "..\Bitboard\U64_Bitboard.cpp"
#include <chrono>
using namespace chrono;

int main () {
    PrecomputeData();
    U64_Bitboard board;
    board.PrintAllBoards();

    /*
        U64 test = 0;
        cout << test<< endl;
        test |= U64(1) << 63;
        cout << test<< endl;

        bool is63set = (test >> 63) & (U64)1;
        //PrintU64(test, "insert 63 ");
        cout << is63set << endl;

        test &= ~((U64)1 << 63);
        PrintU64(test, "remove 63");
        is63set = (test >> 63) & (U64)1;
        cout << is63set << endl;
    */

    /*
        board.LoadFen("rnbqkbn1/pppppppp/5r2/8/3B4/8/PPP3PP/RN1QKBNR w KQq - 0 1");

        auto start = high_resolution_clock::now();
        
        bitset<64> wprecomputtedBishopatt = board.wprecomputtedBishopMoves();
        bitset<64> bprecomputtedBishopatt = board.bprecomputtedBishopMoves();

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "BITSET: Time in microseconds for exploding bitboard implementation for bishops" << endl;
        cout << duration.count() << endl;

        start = high_resolution_clock::now();
        bitset<64> wBishopAttacks = board.wBishopAttacks();
        bitset<64> bBishopAttacks = board.bBishopAttacks();
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "BITSET: Time in microseconds for my implementation for bishops" << endl;
        cout << duration.count() << endl;

        start = high_resolution_clock::now();
        uint64_t wprecomputtedBishopMoves_u64 = board.wprecomputtedBishopMoves_u64();
        uint64_t bprecomputtedBishopMoves_u64 = board.bprecomputtedBishopMoves_u64();
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "U64: Time in microseconds for exploding bitboard implementation for bishops" << endl;
        cout << duration.count() << endl;



        ///////////////////////////
        board.LoadFen("1nbqkbn1/pppppppp/5r2/6R1/2r5/P7/1PPPPPPP/RNBQKBN1 w Q - 0 1");
        start = high_resolution_clock::now();

        bitset<64> wprecomputtedRookAtt = board.wprecomputtedRookMoves();
        bitset<64> bprecomputtedRookAtt = board.bprecomputtedRookMoves();

        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "BITSET: Time in microseconds for exploding bitboard implementation for rooks" << endl;
        cout << duration.count() << endl;

        start = high_resolution_clock::now();
        bitset<64> wRookAttacks = board.wRookAttacks();
        bitset<64> bRookAttacks = board.bRookAttacks();
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "BITSET: Time in microseconds for my implementation for rooks" << endl;
        cout << duration.count() << endl;

        start = high_resolution_clock::now();
        uint64_t wprecomputtedRookMoves_u64 = board.wprecomputtedRookMoves_u64();
        uint64_t bprecomputtedRookMoves_u64 = board.bprecomputtedRookMoves_u64();
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "U64: Time in microseconds for exploding bitboard implementation for bishops" << endl;
        cout << duration.count() << endl;

    */
    



    //////////U64 board
    /*
        U64_Bitboard U64_board;
        U64_board.LoadFen("rnbqkbn1/pppppppp/5r2/8/3B4/8/PPP3PP/RN1QKBNR w KQq - 0 1");

        auto start = high_resolution_clock::now();
        
        uint64_t wU64_boardBishopatt = U64_board.wprecomputtedBishopMoves();
        uint64_t bU64_boardBishopatt = U64_board.bprecomputtedBishopMoves();

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "U64_board: Time in microseconds for exploding bitboard implementation for bishops" << endl;
        cout << duration.count() << endl;

        ///////////////////////////
        U64_board.LoadFen("1nbqkbn1/pppppppp/5r2/6R1/2r5/P7/1PPPPPPP/RNBQKBN1 w Q - 0 1");
        start = high_resolution_clock::now();

        uint64_t wU64_boardRookAtt = U64_board.wprecomputtedRookMoves();
        uint64_t bU64_boardRookAtt = U64_board.bprecomputtedRookMoves();

        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "U64_board: Time in microseconds for exploding bitboard implementation for rooks" << endl;
        cout << duration.count() << endl;

        PrintU64(wU64_boardBishopatt, "wU64_boardBishopatt");
        PrintU64(bU64_boardBishopatt, "bU64_boardBishopatt");

        PrintU64(wU64_boardRookAtt, "wU64_boardRookAtt");
        PrintU64(bU64_boardRookAtt, "bU64_boardRookAtt");
        
        U64_board.LoadFen("rnbqkbn1/pppppppp/5r2/8/3B4/8/PPP3PP/RN1QKBNR w KQq - 0 1");

        auto start = high_resolution_clock::now();
        
        uint64_t wU64_boardBishopatt = U64_board.wprecomputtedBishopMoves();
        uint64_t bU64_boardBishopatt = U64_board.bprecomputtedBishopMoves();

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "U64_board: Time in microseconds for exploding bitboard implementation for bishops" << endl;
        cout << duration.count() << endl;

        ///////////////////////////
        U64_board.LoadFen("1nbqkbn1/pppppppp/5r2/6R1/2r5/P7/1PPPPPPP/RNBQKBN1 w Q - 0 1");
        start = high_resolution_clock::now();

        uint64_t wU64_boardRookAtt = U64_board.wprecomputtedRookMoves();
        uint64_t bU64_boardRookAtt = U64_board.bprecomputtedRookMoves();

        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "U64_board: Time in microseconds for exploding bitboard implementation for rooks" << endl;
        cout << duration.count() << endl;

        PrintU64(wU64_boardBishopatt, "wU64_boardBishopatt");
        PrintU64(bU64_boardBishopatt, "bU64_boardBishopatt");

        PrintU64(wU64_boardRookAtt, "wU64_boardRookAtt");
        PrintU64(bU64_boardRookAtt, "bU64_boardRookAtt");
    */

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