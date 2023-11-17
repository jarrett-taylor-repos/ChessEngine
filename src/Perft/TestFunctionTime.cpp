#include "..\Bitboard\Bitboard.cpp"

int main () {
    InitAll();
    Bitboard b(startFen);
    
    auto start = high_resolution_clock::now();
    Moves move_list;
    b.GenerateMoves(move_list);
    b.GenerateMoves(move_list);
    b.GenerateMoves(move_list);
    b.GenerateMoves(move_list);
    b.GenerateMoves(move_list);
    auto stop = high_resolution_clock::now();
    auto duration_nano = duration_cast<nanoseconds>(stop - start);
    auto duration_micro = duration_cast<microseconds>(stop - start);
    cout << "Get moves: " + to_string(duration_nano.count()) << endl;
    cout << "Get moves: " + to_string(duration_micro.count()) << endl;


    start = high_resolution_clock::now();
    int move0 = move_list.GetMove(0);
    int move1 = move_list.GetMove(1);
    int move2 = move_list.GetMove(2);
    int move3 = move_list.GetMove(3);
    int move4 = move_list.GetMove(4);
    b.MakeMove(move0);
    b.MakeMove(move1);
    b.MakeMove(move2);
    b.MakeMove(move3);
    b.MakeMove(move4);
    stop = high_resolution_clock::now();
    duration_nano = duration_cast<nanoseconds>(stop - start);
    duration_micro = duration_cast<microseconds>(stop - start);
    cout << "make move: " + to_string(duration_nano.count()) << endl;
    cout << "make move: " + to_string(duration_micro.count()) << endl;

    start = high_resolution_clock::now();
    b.wPsuedoMoves();
    b.wPsuedoMoves();
    b.wPsuedoMoves();
    b.wPsuedoMoves();
    b.wPsuedoMoves();
    stop = high_resolution_clock::now();
    duration_nano = duration_cast<nanoseconds>(stop - start);
    duration_micro = duration_cast<microseconds>(stop - start);
    cout << "PsuedoMoves: " + to_string(duration_nano.count()) << endl;
    cout << "PsuedoMoves: " + to_string(duration_micro.count()) << endl;

}