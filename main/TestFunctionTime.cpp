#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b, temp;
    U64 occboard = b.GetOccBoard();

    auto start = high_resolution_clock::now();
    Moves move_list;
    b.GenerateMoves(move_list);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Get moves old: " + to_string(duration.count()) << endl;


    start = high_resolution_clock::now();
    Moves move_list2;
    b.GeneratebbMoves(move_list2);
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);
    cout << "get move new: " + to_string(duration.count()) << endl;


    start = high_resolution_clock::now();
    b.CopyBoard();
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);
    cout << "CopyBoard: " + to_string(duration.count()) << endl;

    int move = move_list.GetMove(1);
    start = high_resolution_clock::now();
    int source = getMoveSource(move);
    int target = getMoveTarget(move);
    int piece = getMovePiece(move);
    int promoted = getMovePromoted(move);
    int capture = getMoveCapture(move);
    int doublePush = getMoveDouble(move);
    int enpass = getMoveEnpassant(move);
    int castling = getMoveCastling(move);
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);
    cout << "Get move info: " + to_string(duration.count()) << endl;

}