#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b(startFen);

    auto start = high_resolution_clock::now();
    Moves move_list;
    b.GenerateMoves(move_list);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Get moves: " + to_string(duration.count()) << endl;


    start = high_resolution_clock::now();
    int move = move_list.GetMove(0);
    b.MakeMove(move);
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);
    cout << "make move: " + to_string(duration.count()) << endl;

    start = high_resolution_clock::now();
    U64 *occ = b.GetOccupancies();
    U64 *bb = b.GetBitBoards();

    memset(occ, 0, 24);
    for (int bbPiece = P; bbPiece <= K; bbPiece++) occ[WHITE] |= bb[bbPiece];
    for (int bbPiece = p; bbPiece <= k; bbPiece++) occ[BLACK] |= bb[bbPiece];

    occ[BOTH] |= occ[WHITE];
    occ[BOTH] |= occ[BLACK];
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);
    cout << "set occupancies: " + to_string(duration.count()) << endl;

}