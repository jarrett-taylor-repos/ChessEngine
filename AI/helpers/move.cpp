#include "../evaluation.h"
class Move {
  public:
  int from;
  int to;
  char promo;
  int eval = -999999;
  bool hasBoard = false;
  U64Bitboard b;
  Move(int fromInput, int toInput, char promoInput) {
    from = fromInput;
    to=toInput;
    promo = promoInput;
  }

  void assignBoard(U64Bitboard &inputb) {
    b = inputb;
    hasBoard=true;
  }
};