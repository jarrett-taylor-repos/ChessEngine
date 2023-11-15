// #include "..\Bitboard\Bitboard.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include "./helpers/moveList.cpp"
using namespace std;

int alphabeta(Bitboard b, int alpha, int beta, int depth, ofstream &log, bool &logging, string logtab, ZTable &ztable){

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
    int standPat = getEval(b); //TODO: change stand pat to pull from ztable

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
  if (isGameOver) {
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
      log<<logtab<<"testing move "<<GetMoveUci(move.move)<<endl;
    }
    // Bitboard bCopy = move.b;
    Bitboard bCopy = b;
    bCopy.MakeMove(move.move);
    int score = -alphabeta(bCopy, -beta, -alpha, depth-1, log, logging, logtab+"\t", ztable);
    if (logging) log<<logtab<<"retrieved score of "<<score<<endl;
    if (score >= beta ) {
      if (logging) {log<<logtab<<"score "<<score<<" is greater than beta "<<beta<<" returning beta"<<endl;}
      ztable.setValue(b.GetZobrist(), depth, beta, -1);
      return beta;
    }
    if (score > alpha ) alpha = score;
  }
  if (logging) {log<<logtab<<"sending back score of "<<alpha<<endl;}
  if (ogalpha==alpha){
    ztable.setValue(b.GetZobrist(), depth, alpha, 3);
  } else {
    ztable.setValue(b.GetZobrist(), depth, alpha, 0);
  }
  return alpha;
}

bool rootSearchwMake(Bitboard &b, ofstream &log, bool &logging, ofstream &simgames, ZTable &ztable, int depth=2, int alpha = -9999999, int beta = 9999999) {
  int bestMove = 0;

  Moves moves;
  b.GenerateMoves(moves); 

  bool isGameOver = true;
  MoveList allmoves(moves, b, alpha, true, 0, isGameOver, ztable, logging, log, "\t");
  if (isGameOver) return false;

  for(auto & move : allmoves.movelist){
    if (logging) {
      log<<"\trootsearch move "<<GetMoveUci(move.move)<<alpha<<"|"<<beta<<endl;
    }
    // Bitboard bCopy = currentMove.b;
    Bitboard bCopy = b;
    bCopy.MakeMove(move.move);
    int eval = -alphabeta(bCopy, -beta, -alpha, depth, log, logging, "\t\t", ztable);
    if (eval > alpha) {
      alpha = eval;
      bestMove = move.move;
    }
  }
  if (logging) {log<<"making move "<<GetMoveUci(bestMove)<<" with evaluation of: "<<alpha<<endl;}
  bool movemade = b.MakeMove(bestMove);
  if (!movemade==1) {
    cout<<"ERROR FOR FEN: "<<b.GetFen();
    cout<<" with num: "<<bestMove;
    cout<<"or uci move: "<<GetMoveUci(bestMove)<<endl<<endl;
  } else {
    simgames<<GetMoveUci(bestMove)<<endl;
  }

  ztable.setValue(b.GetZobrist(), depth, alpha, 0);
  return true;
  // cout<<movemade<<endl<<b.GetFen()<<endl;;
}

void oldmain() {
  InitAll();
  srand(5);
  Bitboard b("rnb1k2r/1pqp1ppp/p3pn2/8/1b1NP3/2N1BP2/PPPQ2PP/R3KB1R b KQkq - 0 8");
  // Bitboard b("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  ofstream log;
  ofstream simgames;
  simgames.open("simgames.txt");

  ZTable* ztable = new ZTable;
  
  bool logging = true;
  int i = 0;
  bool isGameOngoing = true;
  while (isGameOngoing  && i < 250) {
    log.open("logs\\log_"+to_string(i)+".md");
    if (logging) {log<<"NEW MOVE, half move num "<<i<<" FEN: "<<b.GetFen()<<endl;}
    isGameOngoing = rootSearchwMake(b, log, logging, simgames, *ztable, 1);
    log.close();
    i++;
  }
  cout<<endl;
}

int rootsearch(Bitboard &b, MoveList &allmoves, ofstream &log, bool &logging, string logtab, ZTable &ztable, int depth=2, int alpha = -9999999, int beta = 9999999) {
  if (logging) {
    log<<logtab<<"starting root search with alpha: "<<alpha<<" and beta: "<<beta<<endl;
  }
  
  int bestMove = 0;
  // bool isGameOver = true;
  // MoveList allmoves(moves, b, alpha, true, 0, isGameOver, ztable, logging, log, "\t");
  // if (isGameOver) return false;

  for(auto & move : allmoves.movelist){
    if (logging) {
      log<<logtab<<"rootsearch move "<<GetMoveUci(move.move)<<alpha<<"|"<<beta<<endl;
    }
    // Bitboard bCopy = currentMove.b;
    Bitboard bCopy = b;
    bCopy.MakeMove(move.move);
    int eval = -alphabeta(bCopy, -beta, -alpha, depth, log, logging, logtab+"\t\t", ztable);
    if (eval > alpha) {
      alpha = eval;
      bestMove = move.move;
    }
    if (eval >= beta) {
      //beta cutoff, need to change aspiration window
      log<<logtab<<"found eval of "<<eval<<", which is greater than beta: "<<beta<<"returning beta"<<endl;
      return -1;
    }
  }
  log<<logtab<<"found bestMove: "<<GetMoveUci(bestMove)<<" with eval "<<alpha<<endl;
  if (!bestMove) ztable.setValue(b.GetZobrist(), depth, alpha, 0);
  return bestMove;
}

int bestMoveAtDepth(Bitboard &b, ofstream &log, bool &logging, ZTable &ztable, int depth=2) {
  if (logging) log<<"\tsearch for depth "<<depth<<endl;
  bool isGameOver = true;
  Moves moves;
  b.GenerateMoves(moves);
  MoveList allmoves(moves, b, 0, true, 0, isGameOver, ztable, logging, log, "\t\t");
  if (isGameOver) return 0;

  //getting alpha and beta
  U64 zValue = b.GetZobrist();
  int eval = getEval(b);
  ZTableEntry zEntry = ztable.getEntry(zValue);
  if (zEntry.zvalue==zValue) {
    cout<<"got entry"<<endl;
    if (zEntry.nodetype==0) {
      eval = zEntry.score;
    }
    if (zEntry.nodetype==-1 && zEntry.score>eval) {//lower bound
      eval=zEntry.score;
    }
    if (zEntry.nodetype==1 && zEntry.score<eval) {//upper bound
      eval=zEntry.score;
    }
  }

  int alphaWindow = 0;
  int betaWindow = 0;
  int alpha = -9999999;
  int beta = 9999999;
  int ogalpha = alpha;

  int bestMove = 0;
  while (bestMove==0 || bestMove ==-1) {
    int alpha = eval - (250 * pow(2, alphaWindow)) ;
    ogalpha = alpha;
    int beta = eval + (250 * pow(2, betaWindow)); //TODO, adjust these
    if (logging) log<<"\t\ttesting window alpha: "<<alpha<<"beta: "<<beta<<endl;
    bestMove = rootsearch(b, allmoves, log, logging, "\t\t\t", ztable, depth, alpha, beta);
    if (bestMove==0) {
      if (logging) log<<"\t\tno move could raise alpha, lowering bound for aspiration window"<<endl;
      alphaWindow++;
    }
    if (bestMove==-1) {
      if (logging) log<<"\t\tmove caused beta cutoff, raising bound for aspiration window"<<endl;
      betaWindow++;
    }
  }
  if (logging) log<<"\tfound bestmove: "<<GetMoveUci(bestMove)<<endl;
  ztable.setValue(b.GetZobrist(), depth, ogalpha, 0);
  return bestMove;
}

bool makeMoveSetDepth(Bitboard &b, ofstream &log, bool &logging, ofstream &simgames, ZTable &ztable, int depth=1) {
  if (logging) log<<"starting set-depth search for depth: "<<depth<<"and FEN: "<<b.GetFen()<<endl;
  int bestMove = 0;
  bestMove = bestMoveAtDepth(b, log, logging, ztable, depth);
  for (int i=0; i<depth+1; i++) {
    bestMove = bestMoveAtDepth(b, log, logging, ztable, i);
  }

  if (!bestMove) {
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

//todo, add timing one

void main() {
  InitAll();
  srand(5);
  Bitboard b("rnb1k2r/1pqp1ppp/p3pn2/8/1b1NP3/2N1BP2/PPPQ2PP/R3KB1R b KQkq - 0 8");
  // Bitboard b("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  ofstream log;
  ofstream simgames;
  simgames.open("simgames.txt");

  ZTable* ztable = new ZTable;
  
  bool logging = true;
  int i = 0;
  bool isGameOngoing = true;
  while (isGameOngoing  && i < 250) {
    log.open("logs\\log_"+to_string(i)+".md");
    isGameOngoing = makeMoveSetDepth(b, log, logging, simgames, *ztable, 1);
    log.close();
    i++;
    cout<<i<<endl;
  }
  cout<<endl;
}
