#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    U64Bitboard b;
    U64 occboard = b.GetOccBoard();
    //move generation time 
    U64 test = aFile;
    U64 temp = aFile;

    auto start = high_resolution_clock::now();
    b.GetMapMoves();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Get map moves: " + to_string(duration.count()) << endl;


    start = high_resolution_clock::now();
    b.wBishopPsuedoMoves();
    b.wRookPsuedoMoves();
    b.wQueenPsuedoMoves();

    b.wBishopPsuedoMoves();
    b.wRookPsuedoMoves();
    b.wQueenPsuedoMoves();

    b.wBishopPsuedoMoves();
    b.wRookPsuedoMoves();
    b.wQueenPsuedoMoves();

    b.wBishopPsuedoMoves();
    b.wRookPsuedoMoves();
    b.wQueenPsuedoMoves();

    b.wBishopPsuedoMoves();
    b.wRookPsuedoMoves();
    b.wQueenPsuedoMoves();

    b.wBishopPsuedoMoves();
    b.wRookPsuedoMoves();
    U64 queen = b.wQueenPsuedoMoves();
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);
    cout << "Get psuedo moves: " + to_string(duration.count()) << endl;

    start = high_resolution_clock::now();
    b.wMagicRookMoves();
    b.wMagicBishopMoves();
    U64 magicq = b.wMagicQueenMoves();

    b.wMagicRookMoves();
    b.wMagicBishopMoves();
    b.wMagicQueenMoves();

    b.wMagicRookMoves();
    b.wMagicBishopMoves();
    b.wMagicQueenMoves();

    b.wMagicRookMoves();
    b.wMagicBishopMoves();
    b.wMagicQueenMoves();

    b.wMagicRookMoves();
    b.wMagicBishopMoves();
    b.wMagicQueenMoves();

    b.wMagicRookMoves();
    b.wMagicBishopMoves();
    b.wMagicQueenMoves();

    b.wMagicRookMoves();
    b.wMagicBishopMoves();
    b.wMagicQueenMoves();
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);
    cout << "Get magic moves: " + to_string(duration.count()) << endl;

    Print(queen);
    Print(magicq);



}