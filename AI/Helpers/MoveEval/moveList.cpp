#include "../ZTable/ZTable.cpp"
#include "MoveEval.cpp"
#include <algorithm>
#include <vector>
#include <map>

bool CompareMoves(MoveEval move1, MoveEval move2) { return (move1.GetEval()<move2.GetEval()); }

class MoveList {
    public:
    vector<MoveEval> movelist;

    void SortMoves() { sort(movelist.begin(), movelist.end(), CompareMoves); };

    MoveList(Moves &moves, Bitboard &b, int alpha, bool isAlphaBeta, int standPat, bool &isGameOver, ZTable &ztable, bool logging, ofstream &log, string logtab) {
        isGameOver = true;
        if (logging) {log<<logtab<<"starting making move list, isAlphaBeta is "<<isAlphaBeta<<" is in check is "<<b.isInCheck()<<endl;}
        if (isAlphaBeta) movelist.reserve(moves.GetCount());
        for(int i = 0; i < moves.GetCount(); i++) {
            if (isAlphaBeta || b.isInCheck() || ( getMoveCapture(moves.GetMove(i)) && standPat+b.GetAbsValueAtIndex(getMoveTarget(moves.GetMove(i)))+200 > alpha ) ) {
                Bitboard bCopy = b;
                if (!bCopy.MakeMove(moves.GetMove(i))) {if (logging) {log<<logtab+'\t'<<"removing move "<<GetMoveUci(moves.GetMove(i))<<" for illegality"<<endl;}  continue;}
                if (bCopy.IsDraw()) {if (logging) {log<<logtab+'\t'<<"removing move "<<GetMoveUci(moves.GetMove(i))<<" for draw move"<<endl;}  continue;}
                
                isGameOver=false;
                U64 zvalue = bCopy.GetZobrist();
                ZTableEntry z = ztable.GetEntry(zvalue);
                bool isExactMatch = z.isEqualToZvalue(zvalue) && z.isEqualToNodeType(0);
                int eval = isExactMatch ? z.GetScore() : bCopy.GetEvaluationWithMultiplier();

                movelist.emplace_back(moves.GetMove(i), eval);
            } else {
                isGameOver=false;
                if (logging) {log<<logtab+'\t'<<"removing move "<<GetMoveUci(moves.GetMove(i))<<" for pruning"<<endl;}
            }
        }

        SortMoves();
    };
};