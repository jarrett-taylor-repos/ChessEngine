#include "../../PlayGame/PlayGame.h"
using namespace PlayGame;

void main() {
    InitAll();
    string fen = "2kr4/p6p/1p1P2p1/2p2p2/QPP5/P4bP1/3qrN1P/1RR3K1 w - - 2 34";
    playgame_against_settime(5, fen, false);
}
