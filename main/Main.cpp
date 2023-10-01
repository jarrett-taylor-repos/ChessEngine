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

    bitset<64> wkingmoves = board.wKingMoves();
    bitset<64> bkingmoves = board.bKingMoves();

    bitset<64> notbboard = board.NotbBoard();
    bitset<64> notwboard = board.NotwBoard();

    PrintBitSet(wpawnmoves, "wpawnmoves");
    PrintBitSet(bpawnmoves, "bpawnmoves");
    PrintBitSet(wKnightmoves, "wKnightmoves");
    PrintBitSet(bKnightmoves, "bKnightmoves");
    PrintBitSet(wkingmoves, "wkingmoves");
    PrintBitSet(bkingmoves, "bkingmoves");
}