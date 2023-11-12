#include "../evaluation.h"
class MoveEval {
  public:
  int move;
  int eval = -999999;
  bool hasBoard = false;
  // U64Bitboard b;
  MoveEval(int moveInput) {
    move = moveInput;
  }

  MoveEval(int moveInput, U64Bitboard &inputb, int inputeval) {
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

  void assignBoard(U64Bitboard &inputb) {
    // b = inputb;
    hasBoard=true;
  };
};