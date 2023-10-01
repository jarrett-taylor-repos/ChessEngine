
#include "..\Bitboard\Bitboard.cpp"
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
          if (line!=b.GetwPawn().GetBoard().to_string()) {
            cout<<"whitePawn locations are incorrect for FEN: "<<FEN<<endl;
            cout<<"file bitset: "<<line<<endl<<"board Bitset: "<<b.GetwPawn().GetBoard().to_string()<<endl;
          }
          break;
        case 2:
          if (line!=b.GetwKnight().GetBoard().to_string()) {
            cout<<"whiteKnight locations are incorrect for FEN: "<<FEN<<endl;
            cout<<"file bitset: "<<line<<endl<<"board Bitset: "<<b.GetwKnight().GetBoard().to_string()<<endl;
          }
          break;
        case 3:
          if (line != b.GetwBishop().GetBoard().to_string()) {
            cout << "White Bishop locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetwBishop().GetBoard().to_string() << endl;
          }
          break;
        case 4:
          if (line != b.GetwRook().GetBoard().to_string()) {
            cout << "White Rook locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetwRook().GetBoard().to_string() << endl;
          }
          break;
        case 5:
          if (line != b.GetwQueen().GetBoard().to_string()) {
            cout << "White Queen locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetwQueen().GetBoard().to_string() << endl;
          }
          break;
        case 6:
          if (line != b.GetwKing().GetBoard().to_string()) {
            cout << "White King locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetwKing().GetBoard().to_string() << endl;
          }
          break;
        case 7:
          // Black pieces
          if (line != b.GetbPawn().GetBoard().to_string()) {
            cout << "Black Pawn locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbPawn().GetBoard().to_string() << endl;
          }
          break;
        case 8:
          if (line != b.GetbKnight().GetBoard().to_string()) {
            cout << "Black Knight locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbKnight().GetBoard().to_string() << endl;
          }
          break;
        case 9:
          if (line != b.GetbBishop().GetBoard().to_string()) {
            cout << "Black Bishop locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbBishop().GetBoard().to_string() << endl;
          }
          break;
        case 10:
          if (line != b.GetbRook().GetBoard().to_string()) {
            cout << "Black Rook locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbRook().GetBoard().to_string() << endl;
          }
          break;
        case 11:
          if (line != b.GetbQueen().GetBoard().to_string()) {
            cout << "Black Queen locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbQueen().GetBoard().to_string() << endl;
          }
          break;
        case 12:
          if (line != b.GetbKing().GetBoard().to_string()) {
            cout << "Black King locations are incorrect for FEN: " << FEN << endl;
            cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbKing().GetBoard().to_string() << endl;
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
