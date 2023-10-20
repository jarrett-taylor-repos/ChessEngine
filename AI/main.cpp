#include "..\Bitboard\U64Bitboard.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
using namespace std;

int quiesce(U64Bitboard b, int alpha, int beta, ofstream &log, bool &logging, string logtab) {
  if (logging) {log<<logtab<<"quiesce search for FEN: "<<b.GetFen()<<endl;}
  int standPat = b.GetMoveMultiplier()*(b.GetMaterialValue()+rand()%10);
  if (standPat>=beta) {
    if (logging) {log<<logtab<<"standPat of "<<standPat<<" is greater than beta "<<beta<<" returning beta"<<endl;}
    return beta;
  }
  if (standPat>alpha) alpha = standPat;
  multimap<int, pair<int, char>> moves;
  b.GetMapMoves(moves);
  for(multimap<int, pair<int, char>>::const_iterator it = moves.begin(); it != moves.end(); ++it){
    if (logging) {
      log<<logtab<<"testing move ["<<IndexToSquare(it->first)<<IndexToSquare(it->second.first)<<it->second.second;
    }

    if (b.isKingCapture(it->second.first)) {
      if (logging) {log<<logtab<<"move is king capture, returning beta: "<<beta<<endl;}
      return beta;
    }

    if (b.isCapture(it->second.first)) {
      if (logging) {log<<logtab<<"testing capture: "<<IndexToSquare(it->first)<<IndexToSquare(it->second.first)<<it->second.second<<endl;}
      U64Bitboard bCopy = b;
      bCopy.MakeMove(it->first, it->second.first, it->second.second);
      int score = -quiesce(bCopy, -beta, -alpha, log, logging, logtab+"\t");
      if (score >= beta ) {
        if (logging) {log<<logtab<<"score "<<score<<" is greater than beta "<<beta<<" returning beta"<<endl;}
        return beta;
      }
      if (score > alpha ) alpha = score;
    }
  }
  if (logging) {log<<logtab<<"sending back score of "<<alpha<<endl;}
  return alpha;
}

int alphabeta(U64Bitboard b, int alpha, int beta, int depth, ofstream &log, bool &logging, string logtab){
  if (depth == 0) {
    return quiesce(b, alpha, beta, log, logging, logtab);
  };

  if (logging) {
    log<<logtab<<"starting search for depth "<<depth<<" FEN: "<<b.GetFen()<<endl;
  }

  multimap<int, pair<int, char>> moves;
  b.GetMapMoves(moves);
  for(multimap<int, pair<int, char>>::const_iterator it = moves.begin(); it != moves.end(); ++it){
    if (logging) {
      log<<logtab<<"testing move "<<IndexToSquare(it->first)<<IndexToSquare(it->second.first)<<it->second.second<<endl;
    }
    if (b.isKingCapture(it->second.first)) {
      if (logging) {log<<logtab<<"move is king capture, returning beta: "<<beta<<endl;}
      return beta;
    }
    U64Bitboard bCopy = b;
    bCopy.MakeMove(it->first, it->second.first, it->second.second);
    int score = -alphabeta(bCopy, -beta, -alpha, depth-1, log, logging, logtab+"\t");
    if (logging) log<<logtab<<"retrieved score of "<<score<<endl;
    if (score >= beta ) {
      if (logging) {log<<logtab<<"score "<<score<<" is greater than beta "<<beta<<" returning beta"<<endl;}
      return beta;
    }
    if (score > alpha ) alpha = score;
  }
  if (logging) {log<<logtab<<"sending back score of "<<alpha<<endl;}
  return alpha;
}

void rootsearch(U64Bitboard &b, ofstream &log, bool &logging) {
  multimap<int, pair<int, char>> m;
  b.GetMapMoves(m);
  int alpha = -9999999;
  int beta = 9999999;
  int bestfrom = -1;
  int bestto = -1;
  char promo = ' ';
  for(multimap<int, pair<int, char>>::const_iterator it = m.begin(); it != m.end(); ++it){
    if (logging) {
      log<<"\trootsearch move "<<IndexToSquare(it->first)<<IndexToSquare(it->second.first)<<it->second.second<<endl;
    }
    if (!b.isKingCapture(it->second.first)) {
      U64Bitboard bCopy = b;
      bCopy.MakeMove(it->first, it->second.first, it->second.second);
      int eval = -alphabeta(bCopy, alpha, beta, 1, log, logging, "\t\t");
      if (eval > alpha) {
        alpha = eval;
        bestfrom = it->first;
        bestto = it->second.first;
        promo = it->second.second;
      }
    } else {
      log<<"is king capture"<<endl;
    }
  }
  if (logging) {log<<"making move "<<bestfrom<<" "<<bestto<<" "<<promo; log<<" OR "<<IndexToSquare(bestfrom)<<IndexToSquare(bestto)<<promo<<" with evaluation of: "<<alpha<<endl;}
  bool movemade = b.MakeMove(bestfrom, bestto, promo);
  if (!movemade==1) {
    cout<<"ERROR FOR FEN: "<<b.GetFen();
    cout<<" with from num: "<<bestfrom<<" and to num: "<< bestto;
    cout<<"or uci move: "<<IndexToSquare(bestfrom)<<IndexToSquare(bestto)<<promo<<endl<<endl;
  } else {
  cout<<IndexToSquare(bestfrom)<<IndexToSquare(bestto)<<promo<<endl;
  }
  // cout<<movemade<<endl<<b.GetFen()<<endl;;
}

void main() {
  U64Bitboard b;
  freopen("simgames.txt","w",stdout);
  ofstream log;
  
  bool logging = true;
  for (int i=0; i<100; i++) {
    log.open("logs\\log_"+to_string(i)+".md");
    if (logging) {log<<"NEW MOVE, half move num "<<i<<" FEN: "<<b.GetFen()<<"turn: "<<b.IsWhiteMove()<<endl;}
    rootsearch(b, log, logging);
    log.close();
  }
  cout<<endl;
}

void main2() {
  U64Bitboard b;
  b.LoadFen("3r1bnr/1Np1ppp1/2p1b2p/4k3/Ppn1Q1P1/4P2P/3P1P2/R1B1K1NR b K - 5 18");
  multimap<int, pair<int, char>> m;
  m = b.GetMapMoves();
  for(multimap<int, pair<int, char>>::const_iterator it = m.begin(); it != m.end(); ++it){
    cout<<IndexToSquare(it->first)<<IndexToSquare(it->second.first)<<it->second.second<<endl;
  }
}