#include "..\Bitboard\U64Bitboard.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
using namespace std;

int quiesce(U64Bitboard b, int alpha, int beta) {
  return b.GetMoveMultiplier()*(b.GetMaterialValue()+rand()%10);
}

int alphabeta(U64Bitboard b, int alpha, int beta, int depth){
  if (depth == 0) {
    return quiesce(b, alpha, beta);
  };

  multimap<int, pair<int, char>> moves;
  b.GetMapMoves(moves);
  for(multimap<int, pair<int, char>>::const_iterator it = moves.begin(); it != moves.end(); ++it){
    U64Bitboard bCopy = b;
    bCopy.MakeMove(it->first, it->second.first, it->second.second);
    int score = -alphabeta(bCopy, -beta, -alpha, depth-1);
    if (score >= beta ) return beta;
    if (score > alpha ) alpha = score;
  }
  return alpha;
}

void rootsearch(U64Bitboard &b) {
  multimap<int, pair<int, char>> m;
  b.GetMapMoves(m);
  int bestval = -9999999;
  int bestfrom = -1;
  int bestto = -1;
  char promo = ' ';
  for(multimap<int, pair<int, char>>::const_iterator it = m.begin(); it != m.end(); ++it){
    U64Bitboard bCopy = b;
    bCopy.MakeMove(it->first, it->second.first, it->second.second);
    int eval = -alphabeta(bCopy, -999999, 999999, 2);
    if (eval > bestval) {
      bestval = eval;
      bestfrom = it->first;
      bestto = it->second.first;
      promo = it->second.second;
    }
  }
  // cout<<bestfrom<<endl<<bestto<<endl<<promo;
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
  for (int i=0; i<80; i++) {
    rootsearch(b);
  }
  cout<<endl;
}