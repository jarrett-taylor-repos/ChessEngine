#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b(startFen);
    
    auto start = high_resolution_clock::now();
    Moves move_list;
    b.GenerateMoves(move_list);
    auto stop = high_resolution_clock::now();
    auto duration_nano = duration_cast<nanoseconds>(stop - start);
    auto duration_micro = duration_cast<microseconds>(stop - start);
    cout << "Get moves: " + to_string(duration_nano.count()) << endl;
    cout << "Get moves: " + to_string(duration_micro.count()) << endl;


    start = high_resolution_clock::now();
    int move = move_list.GetMove(0);
    b.MakeMove(move);
    stop = high_resolution_clock::now();
    duration_nano = duration_cast<nanoseconds>(stop - start);
    duration_micro = duration_cast<microseconds>(stop - start);
    cout << "make move: " + to_string(duration_nano.count()) << endl;
    cout << "make move: " + to_string(duration_micro.count()) << endl;

}