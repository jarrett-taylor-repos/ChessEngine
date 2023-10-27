#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    U64Bitboard b;
    U64 pawns = b.GetwPawn();
    //move generation time 
    U64 test = aFile;
    U64 temp = aFile;

    auto start = high_resolution_clock::now();
    PopBit(test, a1);
    PopBit(test, d4);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Get traditional way rook attacks: " + to_string(duration.count()) << endl;


    start = high_resolution_clock::now();
    ResetBit(temp, a1);
    ResetBit(temp, d4);
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);
    cout << "Get on the fly rok attacks: " + to_string(duration.count()) << endl;

}