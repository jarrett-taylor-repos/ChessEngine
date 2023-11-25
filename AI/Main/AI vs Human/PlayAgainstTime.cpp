#include "../../PlayGame/PlayGame.h"
using namespace PlayGame;

void main() {
    InitAll();
    string fen = "2kr3B/p2p3p/1ppqp1N1/5p2/2P5/PP4PB/3NKn1P/R6Q w - - 0 21";
    playgame_against_settime(5, fen, false);
}
