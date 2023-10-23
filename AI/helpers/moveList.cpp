#include <vector>
#include <map>
#include "move.cpp"
#include <algorithm>
#include "ZTable.cpp"
#include <iostream>
using namespace std;

bool compareMoves(Move move1, Move move2) {
  return (move1.eval<move2.eval);
}

class MoveList {
  public:
    vector<Move> movelist;
  
  MoveList(multimap <int, pair<int,char> > &moves, bool logging, ofstream &log, string logtab) {
    if (logging) {log<<logtab<<"starting standard making move list - ";}
    movelist.reserve(moves.size());
    for(multimap<int, pair<int, char>>::const_iterator it = moves.begin(); it != moves.end(); ++it) {
      movelist.emplace_back(it->first, it->second.first, it->second.second);
    }
    if (logging) {log<<"done"<<endl;}
  }

  MoveList(multimap <int, pair<int,char> > &moves, U64Bitboard &b, int alpha, int standPat, bool logging, ofstream &log, string logtab) {
    if (logging) {log<<logtab<<"starting quiesce making move list - ";}
    for(multimap<int, pair<int, char>>::const_iterator it = moves.begin(); it != moves.end(); ++it) {
      if (b.isInCheck() || (b.isCapture(it->second.first) && standPat+b.GetAbsValueAtIndex(it->second.first)+200 > alpha)) { // || b.isMoveCheck(it->first, it->second.first, it->second.second)
        movelist.emplace_back(it->first, it->second.first, it->second.second);
      }
    }
    if (logging) {log<<"done"<<endl;}
  }

  void setEvals(U64Bitboard &b, ZTable &ztable){
    for(auto & move : movelist) {
      unsigned long long zvalue = b.GetZobrist();
      ZTableEntry z = ztable.getEntry(zvalue);
      if (z.zvalue==zvalue && z.nodetype == 0) {
        move.eval = z.score;
      } else {
        U64Bitboard bCopy = b;
        bCopy.MakeMove(move.from, move.to, move.promo);
        move.eval = getEval(bCopy);
        move.assignBoard(bCopy);
      }
    }
  }

    void setEvals(U64Bitboard &b){
    for(auto & move : movelist) {

      U64Bitboard bCopy = b;
      bCopy.MakeMove(move.from, move.to, move.promo);
      move.eval = getEval(bCopy);
      move.assignBoard(bCopy);
    }
  }

  void sortmoves() {
    sort(movelist.begin(), movelist.end(), compareMoves);
  }

};