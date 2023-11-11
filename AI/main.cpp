// #include "..\Bitboard\U64Bitboard.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include "./helpers/moveList.cpp"
using namespace std;

int quiesce(U64Bitboard b, int alpha, int beta, ofstream &log, bool &logging, string logtab, ZTable &ztable) {
  if (logging) {log<<logtab<<"quiesce search for FEN: "<<b.GetFen()<<endl;}

  U64 zvalue = b.GetZobrist();
  bool shouldReturn = false;
  int returnValue = ztable.getValue(alpha, beta, shouldReturn, 0, zvalue, log, logtab, logging);

  int ogalpha = alpha;

  Moves moves;
  b.GenerateMoves(moves);

  if (!moves.GetCount()) { //game is over
    if (logging) log<<logtab<<"game is over, returning score of ";

    if (b.isInCheck()) { //checkmate
      if (logging) log<<"-9999999"<<endl;
      return -9999999;
    }

    //game is draw
    if (logging) log<<"-1"<<endl;
    return -1;
  }

  int standPat = getEval(b);
  if (logging) {log<<logtab<<"retrieved standPat score of "<<standPat<<endl;}
  if (!b.isInCheck()) {
    if (standPat>=beta) {
      if (logging) {log<<logtab<<"standPat of "<<standPat<<" is greater than beta "<<beta<<" returning beta"<<endl;}
      return beta;
    }
    if (standPat>alpha) alpha = standPat;
  }

  MoveList allmoves(moves, b, alpha, false, standPat, ztable, logging, log, logtab);
  for(auto & move : allmoves.movelist){//|| b.isMoveCheck(it->first, it->second.first, it->second.second)) {
    if (logging) {log<<logtab<<"testing move: "<<GetMoveUci(move.move)<<endl;}
    // U64Bitboard bCopy = move.b;
    U64Bitboard bCopy = b;
    bCopy.MakeMove(move.move);
    int score = -quiesce(bCopy, -beta, -alpha, log, logging, logtab+"\t", ztable);
    if (logging) log<<logtab<<"retrieved score of "<<score<<endl;
    if (score >= beta ) {
      if (logging) {log<<logtab<<"score "<<score<<" is greater than beta "<<beta<<" returning beta"<<endl;}
      ztable.setValue(b.GetZobrist(), 0, beta, -1);
      return beta;
    }
    if (score > alpha ) alpha = score;
  }
  if (logging) {log<<logtab<<"sending back score of "<<alpha<<endl;}
  if (ogalpha==alpha){
    ztable.setValue(b.GetZobrist(), 0, alpha, 3);
  } else {
    ztable.setValue(b.GetZobrist(), 0, alpha, 0);
  }
  return alpha;
}

int alphabeta(U64Bitboard b, int alpha, int beta, int depth, ofstream &log, bool &logging, string logtab, ZTable &ztable){
  if (depth == 0) {
    return quiesce(b, alpha, beta, log, logging, logtab, ztable);
  };

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

  if (!moves.GetCount() || b.IsDraw()) { //game is over
    if (logging) log<<logtab<<"game is over, returning score of ";

    if (b.isInCheck()) { //checkmate
      if (logging) log<<"-9999999"<<endl;
      return -9999999;
    }

    //game is draw
    if (logging) log<<"-1"<<endl;
    return -1;
  }

  MoveList allmoves(moves, b, alpha, true, 0, ztable, logging, log, logtab);
  for(auto & move : allmoves.movelist){
    if (logging) {
      log<<logtab<<"testing move "<<GetMoveUci(move.move)<<endl;
    }
    // U64Bitboard bCopy = move.b;
    U64Bitboard bCopy = b;
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

bool rootsearch(U64Bitboard &b, ofstream &log, bool &logging, ofstream &simgames, ZTable &ztable, int depth=2, int alpha = -9999999, int beta = 9999999) {
  int bestMove = 0;

  Moves moves;
  b.GenerateMoves(moves);

  if (!moves.GetCount()) return false; //game is over 

  MoveList allmoves(moves, b, alpha, true, 0, ztable, logging, log, "\t");
  for(auto & move : allmoves.movelist){
    if (logging) {
      log<<"\trootsearch move "<<GetMoveUci(move.move)<<alpha<<"|"<<beta<<endl;
    }
    // U64Bitboard bCopy = currentMove.b;
    U64Bitboard bCopy = b;
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

void main() {
  InitAll();
  U64Bitboard b("rnb1k2r/1pqp1ppp/p3pn2/8/1b1NP3/2N1BP2/PPPQ2PP/R3KB1R b KQkq - 0 8");
  // U64Bitboard b("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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
    isGameOngoing = rootsearch(b, log, logging, simgames, *ztable);
    log.close();
    i++;
  }
  cout<<endl;
}

void main2() {
  InitAll();
  cout<<"_____________________________________________"<<endl;
  cout<<"_____________________________________________"<<endl;
  U64Bitboard b("rnb1k2r/1pqp1ppp/p3pn2/8/1b1NP3/2N1BP2/PPPQ2PP/R3KB1R b KQkq - 0 8");
  
  Moves moves1;
  b.GenerateMoves(moves1);
  for(int i = 0; i < moves1.GetCount(); i++) {
    if (GetMoveUci(moves1.GetMove(i))=="c7c4 ") {
      b.MakeMove(moves1.GetMove(i));
      Moves moves2;
      b.GenerateMoves(moves2);
      for(int j = 0; j < moves2.GetCount(); j++) {
        if (GetMoveUci(moves2.GetMove(j))=="d4e2 ") {
          b.MakeMove(moves2.GetMove(j));
          Moves moves3;
          b.GenerateMoves(moves3);
          PrintMoveListUci(moves3);
          cout<<"----------------------"<<endl;
          for(int k = 0; k < moves3.GetCount(); k++) {
            U64Bitboard b4 = b;
            if (b4.MakeMove(moves3.GetMove(k))) {
              cout<<"move "<<GetMoveUci(moves3.GetMove(k))<<" is legal"<<endl;
            } else {
              cout<<"move "<<GetMoveUci(moves3.GetMove(k))<<" is NOT legal"<<endl;
            }
          }
        }
      }
    }
  }
}

// void main() {
//   InitAll();
//   U64Bitboard b("rnb1k2r/1pqp1ppp/p3pn2/8/1b1NP3/2N1BP2/PPPQ2PP/R3KB1R b KQkq - 0 8");

//   U64Bitboard b2 = b;

  
// }