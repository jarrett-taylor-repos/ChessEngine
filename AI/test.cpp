// #include "..\src\Bitboard\Bitboard.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <windows.h>
#include "./helpers/moveList.cpp"
using namespace std;

void main() {
  InitAll();
  srand(5);
  Bitboard b("rnb1k2r/1pqp1ppp/p3pn2/8/1b1NP3/2N1BP2/PPPQ2PP/R3KB1R b KQkq - 0 8");
  // Bitboard b("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  ofstream log;


  
  ZTable* ztable = new ZTable;
  U64 zvalue = b.GetZobrist();
  cout<<zvalue<<endl;
  ztable->setValue(zvalue, 10, 11, 0);
  ZTableEntry ans = ztable->getEntry(zvalue);
  cout<<ans.zvalue<<" "<<ans.score<<endl;
  long t1 = clock();
  Sleep(5000);
  long t2 = clock();
  cout<<t2-t1;
}