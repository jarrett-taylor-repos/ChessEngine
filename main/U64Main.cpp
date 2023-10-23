#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    U64Bitboard b;

    /* Time test
        auto start = high_resolution_clock::now();
        b.GetPsuedoMoves();
        b.SetAttacks();
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        cout << "Time to get get psuedo moves and set attacks: " + to_string(duration.count()) << endl;


        start = high_resolution_clock::now();
        b.GetMapMoves();
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        cout << "Time to get map moves: " + to_string(duration.count()) << endl;
    */
    
}