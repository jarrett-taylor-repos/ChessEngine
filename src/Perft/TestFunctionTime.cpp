#include "..\Bitboard\Bitboard.cpp"

int main () {
    InitAll();
    Bitboard b(startFen);
    
    auto start = high_resolution_clock::now();
    Bitboard copytemp = b;
    auto stop = high_resolution_clock::now();
    auto duration_nano = duration_cast<nanoseconds>(stop - start);
    auto duration_micro = duration_cast<microseconds>(stop - start);
    cout << "Copy board: " + to_string(duration_nano.count()) << endl;
    cout << "Copy board: " + to_string(duration_micro.count()) << endl;

    Moves movesList;
    b.GenerateMoves(movesList);
    int move0 = movesList.GetMove(0);
    start = high_resolution_clock::now();
    b.MakeMove(move0);
    stop = high_resolution_clock::now();
    duration_nano = duration_cast<nanoseconds>(stop - start);
    duration_micro = duration_cast<microseconds>(stop - start);
    cout << "make move: " + to_string(duration_nano.count()) << endl;
    cout << "make move: " + to_string(duration_micro.count()) << endl;

}