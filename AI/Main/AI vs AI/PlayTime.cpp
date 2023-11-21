#include "../../PlayGame/PlayGame.h"
using namespace PlayGame;

void main() {
    string fen = "r3k2r/pbppq1bp/1pn1ppp1/8/2PPP1n1/1P3NP1/PB1N1P1P/R2QKB1R w KQkq - 1 11";
    playgame_settime(5, fen);
}
