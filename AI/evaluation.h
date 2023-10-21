#include "..\Bitboard\U64Bitboard.cpp"

using namespace std;

int getEval(U64Bitboard &b) {
  return b.GetMoveMultiplier()*(b.GetMaterialValue()+rand()%10);
}

// int gameoverEval(U64Bitboard &b) {

// }