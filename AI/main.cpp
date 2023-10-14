#include "..\Bitboard\U64Bitboard.cpp"
#include <iostream>
#include <string>
using namespace std;

void rootsearch(U64Bitboard &b) {
  multimap<int, pair<int, char>> m;
  b.GetMapMoves(m);
  int bestval = -99999;
  int bestfrom = -1;
  int bestto = -1;
  char promo = ' ';
  for(multimap<int, pair<int, char>>::const_iterator it = m.begin(); it != m.end(); ++it){
    U64Bitboard bCopy = b;
    bCopy.MakeMove(it->first, it->second.first, it->second.second);
    if (bCopy.GetMaterialValue()> bestval) {
      bestval = bCopy.GetMaterialValue();
      bestfrom = it->first;
      bestto = it->second.first;
      promo = it->second.second;
    }
  }
  // cout<<bestfrom<<endl<<bestto<<endl<<promo;
  bool movemade = b.MakeMove(bestfrom, bestto, promo);
  cout<<IndexToSquare(bestfrom)<<IndexToSquare(bestto)<<promo<<endl;
  // cout<<movemade<<endl<<b.GetFen()<<endl;;
}

void main() {
  U64Bitboard b;
  for (int i=0; i<80; i++) {
    rootsearch(b);
    // cout<<b.GetFen()<<endl;
  }
}