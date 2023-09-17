#include "bitboard.h"
#include <string>
using namespace std;

Bitboard::Bitboard(string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") {
    Whitepawn.SetFenAndPiece(fen, 1001);
    Whiteknight.SetFenAndPiece(fen, 1010);
    Whitebishop.SetFenAndPiece(fen, 1011);
    Whiterook.SetFenAndPiece(fen, 1100);
    Whitequeen.SetFenAndPiece(fen, 1101);
    Whiteking.SetFenAndPiece(fen, 1110);

    Blackpawn.SetFenAndPiece(fen, 0001);
    Blackknight.SetFenAndPiece(fen, 0010);
    Blackbishop.SetFenAndPiece(fen, 0011);
    Blackook.SetFenAndPiece(fen, 0100);
    Blackqueen.SetFenAndPiece(fen, 0101);
    Blackking.SetFenAndPiece(fen, 0110);
}