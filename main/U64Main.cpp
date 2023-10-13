#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    U64Bitboard u64;
    u64.LoadFen("r1bqkbnr/pPppppp1/2n5/8/8/5N2/P1PPPPpP/RNBQKB1R w Qkq - 0 1");

    
        string fen = u64.GetFen();
        cout << fen << endl;

        auto start = high_resolution_clock::now();
        U64 moves =  u64.GetMoves();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get all U64 moves: " + to_string(duration.count()) << endl;

        string move = "b7a8q";
        start = high_resolution_clock::now();
        bool moveMade =  u64.MakeMove(move);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get make a move moves: " + to_string(duration.count()) << endl;

        u64.LoadFen("rnbqkbnr/1ppp1ppp/p7/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 3");

        multimap<int, pair<int, char>> mapmoves;
        start = high_resolution_clock::now();
        u64.GetMapMoves(mapmoves);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get get stored moves: " + to_string(duration.count()) << endl;
        
        //Print(mapmoves, "mapmoves");

        move = "d5e6";
        start = high_resolution_clock::now();
        moveMade =  u64.MakeMove(move);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time to get make a move moves: " + to_string(duration.count()) << endl;

        //u64.PrintAllBoards();


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