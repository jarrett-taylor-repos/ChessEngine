#include "../../PlayGame/PlayGame.h"
using namespace PlayGame;

void main() {
    InitAll();
    string fen = "rnb1k2r/1pq2ppp/p3pn2/3p4/1b1NP3/2N1BP2/PPPQ2PP/R3KB1R w KQkq d6 0 9";
    playgame_against_settime(3, fen, false);
}
