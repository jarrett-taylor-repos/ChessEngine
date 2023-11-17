#include "..\src\Bitboard\Bitboard.cpp"

using namespace std;



int getEval(Bitboard &b) {
  return b.GetMoveMultiplier()*(b.GetEvaluation());
}