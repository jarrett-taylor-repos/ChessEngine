#include "..\Bitboard\U64Bitboard.cpp"
#include <chrono>
using namespace chrono;

int main () {
    U64Bitboard u64;
    u64.LoadFen("r1bqkbnr/pPppppp1/2n5/8/8/5N2/P1PPPPpP/RNBQKB1R w Qkq - 0 1");
    string fen = u64.GetFen();
    cout << fen << endl;

    auto start = high_resolution_clock::now();
    vector<string> ucimoves =  u64.GetUciMoves();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time to get all uci moves: " << endl;
    cout << duration.count() << endl;

    start = high_resolution_clock::now();
    U64 boardmoves =  u64.wMoves();
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Time to get all biset moves: " << endl;
    cout << duration.count() << endl;
}