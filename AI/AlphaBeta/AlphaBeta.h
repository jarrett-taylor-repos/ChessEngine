#include "../../src/Bitboard/Bitboard.cpp"
#include "../helpers/moveList.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <chrono>

namespace AlphaBeta {
    int alphabeta(Bitboard b, int alpha, int beta, int depth, ofstream &log, bool &logging, string logtab, ZTable &ztable, int &numnodes){
        numnodes++;
        if (logging) {
            log<<logtab<<"starting search for depth "<<depth<<" FEN: "<<b.GetFen()<<" alpha: "<<alpha<<" beta: "<<beta<<endl;
        }

        int ogalpha = alpha;
        U64 zvalue = b.GetZobrist();
        bool shouldReturn = false;
        int returnValue = ztable.getValue(alpha, beta, shouldReturn, depth, zvalue, log, logtab, logging);
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
            int score = -alphabeta(bCopy, -beta, -alpha, depth-1, log, logging, logtab+"\t", ztable, numnodes);
            if (logging) log<<logtab<<"retrieved score of "<<score<<endl;
            if (score >= beta ) {
            if (logging) {
                log<<logtab<<"score "<<score<<" is greater than beta "<<beta<<" returning beta"<<endl;}
                ztable.setValue(b.GetZobrist(), depth, beta, -1);
                return beta;
            }
            if (score > alpha ) alpha = score;
        }
        if (logging) {log<<logtab<<"sending back score of "<<alpha<<endl;}
        if (ogalpha==alpha){
            ztable.setValue(b.GetZobrist(), depth, alpha, 1);
        } else {
            ztable.setValue(b.GetZobrist(), depth, alpha, 0);
        }
        return alpha;
    }

    int rootsearch(Bitboard &b, MoveList &allmoves, ofstream &log, bool &logging, string logtab, ZTable &ztable, int &numnodes, int depth=2, int alpha = -9999999, int beta = 9999999) {
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
            int eval = -alphabeta(bCopy, -beta, -alpha, depth, log, logging, logtab+"\t\t", ztable, numnodes);
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
            ztable.setValue(b.GetZobrist(), depth, alpha, 0);
            if (logging) log<<logtab<<"setting zvalue "<<b.GetZobrist()<<" with score "<<alpha<<" and depth "<<depth<<endl;
            ZTableEntry zEntry = ztable.getEntry(b.GetZobrist());
            if (logging) log<<logtab<<" TESTING: received entry with zvalue"<<zEntry.GetZvalue()<<endl;
        };
        return bestMove;
    }

    int bestMoveAtDepth(Bitboard &b, ofstream &log, bool &logging, ZTable &ztable, int &numnodes, int depth=2) {
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
        ZTableEntry zEntry = ztable.getEntry(zValue);
        if (logging) log<<"\t\treceived entry with zvalue"<<zEntry.GetZvalue()<<endl;
        if (zEntry.isEqualToZvalue(zValue)) {
            if (zEntry.isEqualToNodeType(0)) {
                eval = zEntry.GetScore();
                // cout<<"eval exact"<<endl;
            }
            if (zEntry.isEqualToNodeType(-1) && zEntry.isScoreGreaterThan(eval)) {//lower bound
                // cout<<"eval lower"<<endl;
                eval=zEntry.GetScore();;
            }
            if (zEntry.isEqualToNodeType(1) && zEntry.isScoreLessThan(eval)) {//upper bound
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
            bestMove = rootsearch(b, allmoves, log, logging, "\t\t\t", ztable, numnodes, depth, alpha, beta);
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
        ztable.setValue(b.GetZobrist(), depth, ogalpha, 0);
        return bestMove;
    }

    bool makeMoveSetDepth(Bitboard &b, ofstream &log, bool &logging, ofstream &simgames, ZTable &ztable, int &numnodes, int depth=1) {
        if (logging) log<<"starting set-depth search for depth: "<<depth<<"and FEN: "<<b.GetFen()<<endl;
        int bestMove = 0;
        // bestMove = bestMoveAtDepth(b, log, logging, ztable, depth);
        for (int i=0; i<depth+1; i++) {
            bestMove = bestMoveAtDepth(b, log, logging, ztable, numnodes, i);
        }

        if (bestMove == 0) {
            if (logging) log<<"game is over"<<endl;
            return false;
        }

        if (logging) {log<<"making move "<<GetMoveUci(bestMove)<<endl;}
        bool movemade = b.MakeMove(bestMove);
        if (!movemade==1) {
            cout<<"ERROR FOR FEN: "<<b.GetFen();
            cout<<" with num: "<<bestMove;
            cout<<"or uci move: "<<GetMoveUci(bestMove)<<endl<<endl;
        } else {
            simgames<<GetMoveUci(bestMove)<<endl;
        }
        return true;
        }

    bool makeMoveSetTime(Bitboard &b, ofstream &log, bool &logging, ofstream &simgames, ZTable &ztable, int &numnodes, int &maxdepth, int expected_time = 2) {
        if (logging) log<<"starting set-time search for time: "<<time<<"and FEN: "<<b.GetFen()<<endl;
        int bestMove = 0;
        // bestMove = bestMoveAtDepth(b, log, logging, ztable, depth);
        long time_taken = 0;
        long time_elapsed = 0;
        int depth = 0;

        while (time_taken+3*time_elapsed < expected_time*1000) { //TODO: change equation
            long start = clock();
            bestMove = bestMoveAtDepth(b, log, logging, ztable, numnodes, depth);
            long end = clock();
            time_elapsed = end-start;
            // cout<<"time elapsed: "<<time_elapsed<<endl;
            time_taken+=time_elapsed;
            depth++;
        }
        maxdepth = depth-1;

        if (bestMove == 0) {
            if (logging) log<<"game is over"<<endl;
            return false;
        }

        if (logging) {log<<"making move "<<GetMoveUci(bestMove)<<endl;}
        cout<<GetMoveUci(bestMove)<<endl;
        bool movemade = b.MakeMove(bestMove);
        if (!movemade==1) {
            cout<<"ERROR FOR FEN: "<<b.GetFen();
            cout<<" with num: "<<bestMove;
            cout<<"or uci move: "<<GetMoveUci(bestMove)<<endl<<endl;
        } else {
            simgames<<GetMoveUci(bestMove)<<endl;
        }
        return true;
    }
}