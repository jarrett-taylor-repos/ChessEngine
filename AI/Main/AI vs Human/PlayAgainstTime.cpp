#include "../../PlayGame/PlayGame.h"
using namespace PlayGame;

void main() {
    string fen = "2kr3r/pbppq1bp/1pn1p1p1/5p2/2PPP1n1/PP3NP1/1B1NKP1P/R2Q1B1R w - - 0 13";
    playgame_against_settime(5, fen);
}
