#include "..\Bitboard\U64Bitboard.cpp"

using namespace std;

int getEval(U64Bitboard &b) {
  return b.GetMoveMultiplier()*(b.GetMaterialValue());
}