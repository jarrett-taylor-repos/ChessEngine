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
  if (standPat>=beta) return beta;
  if (standPat>alpha) alpha = standPat;
  multimap<int, pair<int, char>> moves;
  b.GetMapMoves(moves);
  for(multimap<int, pair<int, char>>::const_iterator it = moves.begin(); it != moves.end(); ++it){
    if (b.isKingCapture(it->second.first)) return beta;
    if (b.isCapture(it->second.first)) {
      if (logging) {log<<logtab<<"testing capture: "<<IndexToSquare(it->first)<<IndexToSquare(it->second.first)<<it->second.second<<endl;}
      U64Bitboard bCopy = b;
      bCopy.MakeMove(it->first, it->second.first, it->second.second);
      int score = -quiesce(bCopy, -beta, -alpha, log, logging, logtab+"\t");
      if (score >= beta ) return beta;
      if (score > alpha ) alpha = score;
    }
  }
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
    if (b.isKingCapture(it->second.first)) return beta;
    if (logging) {
      log<<logtab<<"testing move "<<IndexToSquare(it->first)<<IndexToSquare(it->second.first)<<it->second.second<<endl;
    }
    U64Bitboard bCopy = b;
    bCopy.MakeMove(it->first, it->second.first, it->second.second);
    int score = -alphabeta(bCopy, -beta, -alpha, depth-1, log, logging, logtab+"\t");
    if (score >= beta ) return beta;
    if (score > alpha ) alpha = score;
  }
  return alpha;
}

void rootsearch(U64Bitboard &b, ofstream &log, bool &logging) {
  multimap<int, pair<int, char>> m;
  b.GetMapMoves(m);
  int bestval = -9999999;
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
      int eval = -alphabeta(bCopy, -999999, 999999, 2, log, logging, "\t\t");
      if (eval > bestval) {
        bestval = eval;
        bestfrom = it->first;
        bestto = it->second.first;
        promo = it->second.second;
      }
    }
  }
  // cout<<bestfrom<<endl<<bestto<<endl<<promo;
  if (logging) {log<<"making move "<<bestfrom<<" "<<bestto<<" "<<promo; log<<" OR "<<IndexToSquare(bestfrom)<<IndexToSquare(bestto)<<promo<<endl;}
  bool movemade = b.MakeMove(bestfrom, bestto, promo);
  if (!movemade==1) {
    cout<<"ERROR FOR FEN: "<<b.GetFen();
    cout<<" with from num: "<<bestfrom<<" and to num: "<< bestto;
    cout<<"or uci move: "<<IndexToSquare(bestfrom)<<IndexToSquare(bestto)<<promo<<endl;
  }
  cout<<IndexToSquare(bestfrom)<<IndexToSquare(bestto)<<promo<<endl;
  // cout<<movemade<<endl<<b.GetFen()<<endl;;
}

void main() {
  U64Bitboard b;
  freopen("simgames.txt","w",stdout);
  ofstream log;
  log.open("log.txt");
  log<<"test"<<endl;
  bool logging = true;
  for (int i=0; i<80; i++) {
    if (logging) {log<<"NEW MOVE, half move num "<<i<<" FEN: "<<b.GetFen()<<endl;}
    rootsearch(b, log, logging);
  }
  cout<<endl;
}