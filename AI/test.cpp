// #include "..\Bitboard\U64Bitboard.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include "./helpers/moveList.cpp"
using namespace std;

void main() {
  InitAll();
  // U64Bitboard b("r1b3k1/1r4pp/p1np4/4p3/p2B4/P2P4/5NPP/1R2KB1R b K - 0 30");
  U64Bitboard b("7k/8/8/4K3/8/8/8/8 b - - 0 1");
  Moves moves1;
  b.GenerateMoves(moves1);
  MoveList ml(moves1);
  for(auto & move : ml.movelist){
    cout<<GetMoveUci(move.move)<<endl;
  }
}