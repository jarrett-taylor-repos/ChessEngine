#include "../Utils/Transposition/TranspositionTable.cpp"
#include "../../Engine/Bitboard/Bitboard.cpp"

namespace Minimax {
    int Alphabeta(int depth, Bitboard &b, TranspositionTable table, int alpha, int beta, int &numnodes) {
        numnodes++;

        int ogalpha = alpha;
        U64 zobrist = b.GetZobrist();
        bool shouldReturn = false;
        int returnValue = table.GetValue(alpha, beta, shouldReturn, depth);
        if (shouldReturn) return returnValue;

        Moves moves;
        b.GenerateMoves(moves);

        int standPat = 0;
        if (depth<=0){
            int standPat = b.GetEvaluationWithMultiplier(); //TODO: change stand pat to pull from ztable
            if (!b.isInCheck()) {
                if (standPat>=beta) return beta;
                if (standPat>alpha) alpha = standPat;
            }
        }

        bool isGameOver = true;
        MoveList allmoves(moves, b, alpha, depth>0, standPat, isGameOver, ztable, logging, log, logtab);
        if (isGameOver || b.IsDraw()) {
            if (b.isInCheck()) return -999999;

            //game is draw
            return -1;
        }
        for(auto & move : allmoves.movelist){
            // Bitboard bCopy = move.b;
            Bitboard bCopy = b;
            bCopy.MakeMove(move.GetMove());
            
            int score = -AlphaBeta(bCopy, -beta, -alpha, depth-1, log, logging, logtab+"\t", ztable, numnodes);
            if (score >= beta) {
                ztable.SetValue(b.GetZobrist(), depth, beta, -1);
                return beta;
            }
            if (score > alpha ) alpha = score;
        }

        ztable.SetValue(b.GetZobrist(), depth, alpha, ogalpha==alpha ? 1 : 0);
        return alpha;
    }
}