#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b;
    U64 occboard = b.GetOccBoard();
    //move generation time 
    U64 test = aFile;
    U64 temp = aFile;

    auto start = high_resolution_clock::now();
    for(int i = 0; i < 64; i++) {
        b.QueenAttacks(precomputtedSingleBit[i], occboard);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Get traditional way rook attacks: " + to_string(duration.count()) << endl;


    start = high_resolution_clock::now();
    for(int i = 0; i < 64; i++) {
        QueenAttacks(i, occboard);
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);
    cout << "Get on the fly rok attacks: " + to_string(duration.count()) << endl;



}