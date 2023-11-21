#include "../../PlayGame/PlayGame.h"
using namespace PlayGame;

void main() {
    string fen = "2kr3r/pbppq1bp/1pn1ppp1/8/2PPP1n1/PP3NP1/1B1N1P1P/R2QKB1R w KQ - 1 12";
    playgame_against_settime(10, fen);
}
