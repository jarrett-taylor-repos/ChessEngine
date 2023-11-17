
#include "..\src\Bitboard\Bitboard.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>
using namespace std;

int main() {
  std::string line;
  std::ifstream FENfile("test_files/pieceLocations.txt");

  if (FENfile.is_open()) {
    int linenum = 0;
    Bitboard b;
    string FEN = "";
    while (getline(FENfile, line)) {
      switch (linenum) {
        case 0:
          b.LoadFen(line);
          FEN = line;
          break;
        case 1:
          if (line!=b.GetwPawn()) {
            cout<<"whitePawn locations are incorrect for FEN: "<<FEN<<endl;
            cout<<"file bitset: "<<line<<endl<<"board Bitset: "<<b.GetwPawn()<<endl;
          }
          break;
        case 2:
          if (line!=b.GetwKnight()) {
            cout<<"whiteKnight locations are incorrect for FEN: "<<FEN<<endl;
            cout<<"file bitset: "<<line<<endl<<"board Bitset: "<<b.GetwKnight()<<endl;
          }
          break;
        case 3:
          if (line != b.GetwBishop()) {
            cout << "White Bishop locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetwBishop() << endl;
          }
          break;
        case 4:
          if (line != b.GetwRook()) {
            cout << "White Rook locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetwRook() << endl;
          }
          break;
        case 5:
          if (line != b.GetwQueen()) {
            cout << "White Queen locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetwQueen() << endl;
          }
          break;
        case 6:
          if (line != b.GetwKing()) {
            cout << "White King locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetwKing() << endl;
          }
          break;
        case 7:
          // Black pieces
          if (line != b.GetbPawn()) {
            cout << "Black Pawn locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbPawn() << endl;
          }
          break;
        case 8:
          if (line != b.GetbKnight()) {
            cout << "Black Knight locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbKnight() << endl;
          }
          break;
        case 9:
          if (line != b.GetbBishop()) {
            cout << "Black Bishop locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbBishop() << endl;
          }
          break;
        case 10:
          if (line != b.GetbRook()) {
            cout << "Black Rook locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbRook() << endl;
          }
          break;
        case 11:
          if (line != b.GetbQueen()) {
            cout << "Black Queen locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbQueen() << endl;
          }
          break;
        case 12:
          if (line != b.GetbKing()) {
            cout << "Black King locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbKing() << endl;
          }
          linenum=-1;
          break;

      }
      linenum+=1;
    }
    FENfile.close();
  } else {
    std::cerr << "Unable to open file" << std::endl;
  }

  return 0;
}
