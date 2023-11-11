#include <vector>
#include <map>
#include "move.cpp"
#include <algorithm>
#include "ZTable.cpp"
#include <iostream>
using namespace std;

bool compareMoves(MoveEval move1, MoveEval move2) {
  return (move1.eval>move2.eval);
}

class MoveList {
  public:
    vector<MoveEval> movelist;
  
  void sortmoves() {
    sort(movelist.begin(), movelist.end(), compareMoves);
  }

  MoveList(Moves &moves, U64Bitboard b, int alpha, bool isAlphaBeta, int standPat, ZTable &ztable, bool logging, ofstream &log, string logtab) {
    if (logging) {log<<logtab<<"starting making move list, isAlphaBeta is "<<isAlphaBeta<<endl;}
    if (isAlphaBeta) movelist.reserve(moves.GetCount());
    for(int i = 0; i < moves.GetCount(); i++) {
      if (isAlphaBeta || b.isInCheck() || ( getMoveCapture(moves.GetMove(i)) && standPat+b.GetAbsValueAtIndex(getMoveTarget(moves.GetMove(i)))+200 > alpha ) ) {
        U64Bitboard bCopy = b;
        if (!bCopy.MakeMove(moves.GetMove(i))) {if (logging) {log<<logtab+'\t'<<"removing move "<<GetMoveUci(moves.GetMove(i))<<" for illegality"<<endl;}  continue;}
        
        int eval;
        unsigned long long zvalue = b.GetZobrist();
        ZTableEntry z = ztable.getEntry(zvalue);
        if (z.zvalue==zvalue && z.nodetype == 0) {
          eval = z.score;
        } else {
          eval = getEval(bCopy);
        }
        movelist.emplace_back(moves.GetMove(i), bCopy, eval);
      } else {
        if (logging) {log<<logtab+'\t'<<"removing move "<<GetMoveUci(moves.GetMove(i))<<" for pruning"<<endl;}
      }
    }

    sortmoves();
  };
};