#include "../../src/Engine/Bitboard/Bitboard.cpp"
#include "../Helpers/MoveEval/MoveList.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <chrono>

namespace Search {
    int AlphaBeta(Bitboard b, int alpha, int beta, int depth, ofstream &log, bool &logging, string logtab, ZTable &ztable, int &numnodes){
        numnodes++;
        if (logging) {
            log<<logtab<<"starting search for depth "<<depth<<" FEN: "<<b.GetFen()<<" alpha: "<<alpha<<" beta: "<<beta<<endl;
        }

        int ogalpha = alpha;
        U64 zvalue = b.GetZobrist();
        bool shouldReturn = false;
        int returnValue = ztable.GetValue(alpha, beta, shouldReturn, depth, zvalue, log, logtab, logging);
        if (shouldReturn) return returnValue;

        Moves moves;
        b.GenerateMoves(moves);

        int standPat = 0;
        if (depth<=0){
            int standPat = b.GetEvaluationWithMultiplier(); //TODO: change stand pat to pull from ztable

            if (logging) {log<<logtab<<"retrieved standPat score of "<<standPat<<endl;}
            if (!b.isInCheck()) {
                if (standPat>=beta) {
                    if (logging) {log<<logtab<<"standPat of "<<standPat<<" is greater than beta "<<beta<<" returning beta"<<endl;}
                    return beta;
                }
                if (standPat>alpha) alpha = standPat;
            }
        }

        bool isGameOver = true;
        MoveList allmoves(moves, b, alpha, depth>0, standPat, isGameOver, ztable, logging, log, logtab);
        if (isGameOver || b.IsDraw()) {
            if (logging) log<<logtab<<"game is over, returning score of ";
            if (b.isInCheck()) { //checkmate
                if (logging) log<<"-999999"<<endl;
                return -999999;
            }
            //game is draw
            if (logging) log<<"-1"<<endl;
            return -1;
        }
        for(auto & move : allmoves.movelist){
            if (logging) {
                log<<logtab<<"testing move "<<GetMoveUci(move.GetMove())<<endl;
            }
            // Bitboard bCopy = move.b;
            Bitboard bCopy = b;
            bCopy.MakeMove(move.GetMove());
            
            int score = -AlphaBeta(bCopy, -beta, -alpha, depth-1, log, logging, logtab+"\t", ztable, numnodes);
            if (logging) log<<logtab<<"retrieved score of "<<score<<endl;
            if (score >= beta ) {
            if (logging) {
                log<<logtab<<"score "<<score<<" is greater than beta "<<beta<<" returning beta"<<endl;}
                ztable.SetValue(b.GetZobrist(), depth, beta, -1);
                return beta;
            }
            if (score > alpha ) alpha = score;
        }
        if (logging) {log<<logtab<<"sending back score of "<<alpha<<endl;}
        if (ogalpha==alpha){
            ztable.SetValue(b.GetZobrist(), depth, alpha, 1);
        } else {
            ztable.SetValue(b.GetZobrist(), depth, alpha, 0);
        }
        return alpha;
    }

    int RootSearch(Bitboard &b, MoveList &allmoves, ofstream &log, bool &logging, string logtab, ZTable &ztable, int &numnodes, int depth=2, int alpha = -9999999, int beta = 9999999) {
        if (logging) {
            log<<logtab<<"starting root search with alpha: "<<alpha<<" and beta: "<<beta<<endl;
        }
        
        int bestMove = 0;
        // bool isGameOver = true;
        // MoveList allmoves(moves, b, alpha, true, 0, isGameOver, ztable, logging, log, "\t");
        // if (isGameOver) return false;

        for(auto & move : allmoves.movelist){
            if (logging) {
                log<<logtab<<"rootsearch move "<<GetMoveUci(move.GetMove())<<alpha<<"|"<<beta<<endl;
            }
            // Bitboard bCopy = currentMove.b;
            Bitboard bCopy = b;
            bCopy.MakeMove(move.GetMove());

            int eval = -AlphaBeta(bCopy, -beta, -alpha, depth, log, logging, logtab+"\t\t", ztable, numnodes);
            if (eval > alpha) {
                alpha = eval;
                bestMove = move.GetMove();
            }
            if (eval >= beta) {
                //beta cutoff, need to change aspiration window
                log<<logtab<<"found eval of "<<eval<<", which is greater than beta: "<<beta<<"returning beta"<<endl;
                return -1;
            }
        }
        if (logging) log<<logtab<<"found bestMove: "<<GetMoveUci(bestMove)<<" with eval "<<alpha<<" TEMP NUM "<<bestMove<<endl;
        if (bestMove != 0) {
            ztable.SetValue(b.GetZobrist(), depth, alpha, 0);
            if (logging) log<<logtab<<"setting zvalue "<<b.GetZobrist()<<" with score "<<alpha<<" and depth "<<depth<<endl;
            ZTableEntry zEntry = ztable.GetEntry(b.GetZobrist());
            if (logging) log<<logtab<<" TESTING: received entry with zvalue"<<zEntry.GetZvalue()<<endl;
        };
        //cout<<"eval for bestMove of "<<GetMoveUci(bestMove)<<" is "<<alpha<<endl; //TODO: remove
        return bestMove;
    }

    int BestMoveAtDepth(Bitboard &b, ofstream &log, bool &logging, ZTable &ztable, int &numnodes, int depth) {
        if (logging) log<<"\tsearch for depth "<<depth<<endl;
        bool isGameOver = true;
        Moves moves;
        b.GenerateMoves(moves);
        MoveList allmoves(moves, b, 0, true, 0, isGameOver, ztable, logging, log, "\t\t");
        if (isGameOver || b.IsDraw()) return 0;

        //getting alpha and beta
        U64 zValue = b.GetZobrist();
        int eval = b.GetEvaluationWithMultiplier();
        if (logging) log<<"\t\ttrying to retrieve table entry with zvalue "<<zValue<<endl;
        ZTableEntry zEntry = ztable.GetEntry(zValue);
        if (logging) log<<"\t\treceived entry with zvalue"<<zEntry.GetZvalue()<<endl;
        if (zEntry.isEqualToZvalue(zValue)) {
            if (zEntry.isEqualToNodeType(ExactValue)) {
                eval = zEntry.GetScore();
                // cout<<"eval exact"<<endl;
            }
            if (zEntry.isEqualToNodeType(LowerBound) && zEntry.isScoreGreaterThan(eval)) {//lower bound
                // cout<<"eval lower"<<endl;
                eval=zEntry.GetScore();;
            }
            if (zEntry.isEqualToNodeType(UpperBound) && zEntry.isScoreLessThan(eval)) {//upper bound
                // cout<<"eval upper"<<endl;
                eval=zEntry.GetScore();;
            }
        }

        int alphaWindow = 0;
        int betaWindow = 0;
        int alpha = -9999999;
        int beta = 9999999;
        int ogalpha = alpha;

        int bestMove = 0;
        while (bestMove==0 || bestMove ==-1) {
            // int alpha = eval - (250 * pow(2, alphaWindow)) ;
            ogalpha = alpha;
            // int beta = eval + (250 * pow(2, betaWindow)); //TODO, adjust these
            if (logging) log<<"\t\ttesting window alpha: "<<alpha<<" beta: "<<beta<<endl;
            bestMove = RootSearch(b, allmoves, log, logging, "\t\t\t", ztable, numnodes, depth, alpha, beta);
            if (bestMove==0) {
                cout<<"Lower cutoff"<<endl;
                if (logging) log<<"\t\tno move could raise alpha, lowering bound for aspiration window"<<endl;
                alphaWindow++;
            }
            if (bestMove==-1) {
                cout<<"upper cutoff"<<endl;
                if (logging) log<<"\t\tmove caused beta cutoff, raising bound for aspiration window"<<endl;
                betaWindow++;
            }
        }
        if (logging) log<<"\tfound bestmove: "<<GetMoveUci(bestMove)<<endl;
        ztable.SetValue(b.GetZobrist(), depth, ogalpha, 0);
        return bestMove;
    }
}