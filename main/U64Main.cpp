#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    U64Bitboard u64;
    u64.LoadFen("r1bqkbnr/pPppppp1/2n5/8/8/5N2/P1PPPPpP/RNBQKB1R w Qkq - 0 1");
    
    string fen = u64.GetFen();
    cout << fen << endl;

    string move = "b7a8q";
    auto start = high_resolution_clock::now();
    bool ucimoves =  u64.MakeMove(move);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Time to get all uci moves: " + to_string(duration.count()) << endl;

}