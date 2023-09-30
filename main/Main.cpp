#include "..\Bitboard\Bitboard.cpp"
#include "..\Bitboard\BitboardHelper.cpp"
#include "..\MoveEngine\MoveCoordinates.h"
using namespace MoveCoordinates;

int main () {
    Bitboard board;
    bitset<64> bKnightmoves = board.bKnightMoves();
    bitset<64> wKnightmoves = board.wKnightMoves();

    bitset<64> wpawnmoves = board.wPawnMoves();
    bitset<64> bpawnmoves = board.bPawnMoves();

    cout << "wpawnmoves" << endl;
    PrintBitSet(wpawnmoves);
    cout << "bpawnmoves" << endl;
    PrintBitSet(bpawnmoves);
    cout << "wKnightmoves" << endl;
    PrintBitSet(wKnightmoves);
    cout << "bKnightmoves" << endl;
    PrintBitSet(bKnightmoves);
}