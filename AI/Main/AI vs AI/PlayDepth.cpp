#include "../../PlayGame/PlayGame.h"
using namespace PlayGame;

void main() {
    InitAll();
    string fen = "2kr3r/pbpp2bp/1p2p1p1/2q1Pp2/2P3n1/PP3NP1/1B1NKP1P/R2Q1B1R w - - 1 15";
    playgame_setdepth(2, fen, false);
}
