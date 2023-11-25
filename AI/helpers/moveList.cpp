#include "ZTable.cpp"
#include "MoveEval.cpp"
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

bool compareMoves(MoveEval move1, MoveEval move2) { return (move1.GetEval()<move2.GetEval()); }

class MoveList {
  public:
    vector<MoveEval> movelist;
    
    void sortmoves() { sort(movelist.begin(), movelist.end(), compareMoves); };

    MoveList(Moves &moves, Bitboard &b, int alpha, bool isAlphaBeta, int standPat, bool &isGameOver, ZTable &ztable, bool logging, ofstream &log, string logtab) {
        isGameOver = true;
        if (logging) {log<<logtab<<"starting making move list, isAlphaBeta is "<<isAlphaBeta<<" is in check is "<<b.isInCheck()<<endl;}
        if (isAlphaBeta) movelist.reserve(moves.GetCount());

        for(int i = 0; i < moves.GetCount(); i++) {
            if (isAlphaBeta || b.isInCheck() || ( getMoveCapture(moves.GetMove(i)) && standPat+b.GetAbsValueAtIndex(getMoveTarget(moves.GetMove(i)))+200 > alpha ) ) {
                Bitboard bCopy = b;
                if (!b.MakeMove(moves.GetMove(i))) {if (logging) {log<<logtab+'\t'<<"removing move "<<GetMoveUci(moves.GetMove(i))<<" for illegality"<<endl;}  continue;}
                if (b.IsDraw()) {if (logging) {log<<logtab+'\t'<<"removing move "<<GetMoveUci(moves.GetMove(i))<<" for draw move"<<endl;}  continue;}
                isGameOver=false;

                U64 zvalue = bCopy.GetZobrist();
                ZTableEntry z = ztable.GetEntry(zvalue);

                bool exactValue = z.isEqualToZvalue(zvalue) && z.isEqualToNodeType(0);
                int eval = exactValue ? z.GetScore() : bCopy.GetEvaluationWithMultiplier();
                MoveEval currEval(moves.GetMove(i), bCopy, eval);
                movelist.push_back(currEval);
                //movelist.emplace_back(moves.GetMove(i), eval);
                //need to includemove val with bcopy here 
            } else {
                isGameOver=false;
                if (logging) {log<<logtab+'\t'<<"removing move "<<GetMoveUci(moves.GetMove(i))<<" for pruning"<<endl;}
            }
        }

        sortmoves();
    };

    MoveList(Moves &moves) {
        for(int i = 0; i < moves.GetCount(); i++) {
            cout<<"i: "<<i<<" move:"<<GetMoveUci(moves.GetMove(i))<<endl;
            movelist.emplace_back(moves.GetMove(i), i);
        }

        sortmoves();
    };
};