#include "../evaluation.h"
class MoveEval {
  public:
  int move;
  int eval = -999999;
  bool hasBoard = false;
  // Bitboard b;
  MoveEval(int moveInput) {
    move = moveInput;
  }

  MoveEval(int moveInput, Bitboard &inputb, int inputeval) {
    move = moveInput;
    // b = inputb;
    hasBoard=true;
    eval=inputeval;
  }

    MoveEval(int moveInput, int inputeval) {
    move = moveInput;
    // b = inputb;
    hasBoard=true;
    eval=inputeval;
  }

  void assignBoard(Bitboard &inputb) {
    // b = inputb;
    hasBoard=true;
  };
};