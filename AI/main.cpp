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

  if (b.isGameOver()) {
    if (logging) log<<logtab<<"game is over, returning score of ";
    if (b.isDraw()) {
      if (logging) log<<"0"<<endl;
      return -1;
    }
    if (logging) log<<"-9999999"<<endl;
    return -9999999;
  }

  int ogalpha = alpha;

  int standPat = getEval(b);

  if (logging) {log<<logtab<<"retrieved standPat score of "<<standPat<<endl;}
  if (!b.isInCheck()) {
    if (standPat>=beta) {
      if (logging) {log<<logtab<<"standPat of "<<standPat<<" is greater than beta "<<beta<<" returning beta"<<endl;}
      return beta;
    }
    if (standPat>alpha) alpha = standPat;
  }

  multimap<int, pair<int, char>> moves;
  b.GetMapMoves(moves);
  MoveList allmoves(moves, b, alpha, standPat, logging, log, logtab)`11;
  allmoves.setEvals(b);
  allmoves.sortmoves();
  for(auto & move : allmoves.movelist){//|| b.isMoveCheck(it->first, it->second.first, it->second.second)) {
    if (logging) {log<<logtab<<"testing move: "<<IndexToSquare(move.from)<<IndexToSquare(move.to)<<move.promo<<endl;}
    U64Bitboard bCopy;
    if (move.hasBoard) {
      bCopy = move.b;
    } else {
      U64Bitboard bCopy = b;
      bCopy.MakeMove(move.from, move.to, move.promo);
    }
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

  if (b.isGameOver()) {
    if (logging) log<<logtab<<"game is over, returning score of ";
    if (b.isDraw()) {
      if (logging) log<<"0"<<endl;
      return -1;
    }
    if (logging) log<<"-9999999"<<endl;
    return -9999999;
  }

  int ogalpha = alpha;

  multimap<int, pair<int, char>> moves;
  b.GetMapMoves(moves);
  MoveList allmoves(moves, logging, log, logtab);
  allmoves.setEvals(b);
  allmoves.sortmoves();
  for(auto & move : allmoves.movelist){
    if (logging) {
      log<<logtab<<"testing move "<<IndexToSquare(move.from)<<IndexToSquare(move.to)<<move.promo<<endl;
    }
    U64Bitboard bCopy;
    if (move.hasBoard) {
      bCopy = move.b;
    } else {
      U64Bitboard bCopy = b;
      bCopy.MakeMove(move.from, move.to, move.promo);
    }
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

void rootsearch(U64Bitboard &b, ofstream &log, bool &logging, ofstream &simgames, ZTable &ztable) {
  int DEPTH = 1;

  multimap<int, pair<int, char>> m;
  b.GetMapMoves(m);
  MoveList allmoves(m, logging, log, "\t");
  allmoves.setEvals(b);
  allmoves.sortmoves();
  int alpha = -9999999;
  int beta = 9999999;
  int bestfrom = -1;
  int bestto = -1;
  char promo = ' ';
  for(auto & move : allmoves.movelist){
    if (logging) {
      log<<"\trootsearch move "<<IndexToSquare(move.from)<<IndexToSquare(move.to)<<move.promo<<endl;
    }
    U64Bitboard bCopy;
    if (move.hasBoard) {
      bCopy = move.b;
    } else {
      U64Bitboard bCopy = b;
      bCopy.MakeMove(move.from, move.to, move.promo);
    }
    int eval = -alphabeta(bCopy, alpha, beta, DEPTH, log, logging, "\t\t", ztable);
    if (eval > alpha) {
      alpha = eval;
      bestfrom = move.from;
      bestto = move.to;
      promo = move.promo;
    }
  }
  if (logging) {log<<"making move "<<bestfrom<<" "<<bestto<<" "<<promo; log<<" OR "<<IndexToSquare(bestfrom)<<IndexToSquare(bestto)<<promo<<" with evaluation of: "<<alpha<<endl;}
  bool movemade = b.MakeMove(bestfrom, bestto, promo);
  if (!movemade==1) {
    cout<<"ERROR FOR FEN: "<<b.GetFen();
    cout<<" with from num: "<<bestfrom<<" and to num: "<< bestto;
    cout<<"or uci move: "<<IndexToSquare(bestfrom)<<IndexToSquare(bestto)<<promo<<endl<<endl;
  } else {
  simgames<<IndexToSquare(bestfrom)<<IndexToSquare(bestto)<<promo<<endl;
  }

  ztable.setValue(b.GetZobrist(), DEPTH, alpha, 0);
  // cout<<movemade<<endl<<b.GetFen()<<endl;;
}

void main() {
  U64Bitboard b("r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/2N2N2/PPPP1PPP/R1BQKB1R w KQkq - 4 4");
  // freopen("simgames.txt","w",stdout);
  ofstream log;
  ofstream simgames;
  simgames.open("simgames.txt");

  ZTable* ztable = new ZTable;
  
  bool logging = true;
  int i = 0;
  while (!b.isGameOver()) {
    log.open("logs\\log_"+to_string(i)+".md");
    if (logging) {log<<"NEW MOVE, half move num "<<i<<" FEN: "<<b.GetFen()<<endl;}
    rootsearch(b, log, logging, simgames, *ztable);
    log.close();
    i++;
  }
  cout<<endl;
}

typedef uint64_t u64;
void main2() {
  U64Bitboard b;
  b.LoadFen("3k3r/2p1pp1p/7R/8/8/8/4B3/1N2K1q1 w - - 0 37");
  cout<<b.GetFen()<<endl;
  multimap<int, pair<int, char>> m;
  m = b.GetMapMoves();
  for(multimap<int, pair<int, char>>::const_iterator it = m.begin(); it != m.end(); ++it){
    cout<<IndexToSquare(it->first)<<IndexToSquare(it->second.first)<<it->second.second<<endl;
  }
}

void main3() {
  U64Bitboard b1;
  cout<<b1.isGameOver()<<endl;
  U64Bitboard b2 = b1;
  cout<<b2.isGameOver()<<endl;
}
