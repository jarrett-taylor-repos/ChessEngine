
#include "..\src\Bitboard\Bitboard.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>
#include <bitset>
using namespace std;

int main() {
  std::string line;
  std::ifstream FENfile("test_files/pseudoMoves.txt");
  freopen("wrong_pseudo_moves.txt","w",stdout);
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
          if (line!=bitset<64>(b.wPawnMoves()).to_string()) {
            cout<<"White Pawn moves are incorrect for FEN: "<<FEN<<endl;
            cout<<"file bitset:  "<<line<<endl<<"board Bitset: "<<bitset<64>(b.wPawnMoves()).to_string()<<endl;
          }
          break;
        case 2:
          if (line!= bitset<64>(b.wKnightMoves()).to_string()) {
            cout<<"White Knight moves are incorrect for FEN: "<<FEN<<endl;
            cout<<"file bitset:  "<<line<<endl<<"board Bitset: "<<b.wKnightMoves()<<endl;
          }
          break;
        case 3:
          if (line != bitset<64>(b.wBishopMoves()).to_string()) {
            cout << "White Bishop moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.wBishopMoves() << endl;
          }
          break;

        case 4:
          if (line != bitset<64>(b.wRookMoves()).to_string()) {
            cout << "White Rook moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.wRookMoves() << endl;
          }
          break;
        case 5:
          if (line != bitset<64>(b.wQueenMoves()).to_string()) {
            cout << "White Queen moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.wQueenMoves() << endl;
          }
          break;

        case 6:
          if (line != bitset<64>(b.wKingMoves()).to_string()) {
            cout << "White King moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << bitset<64>(b.wKingMoves()).to_string() << endl;
          }
          break;

        case 7:
          if (line != bitset<64>(b.bPawnMoves()).to_string()) {
            cout << "Black Pawn moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.bPawnMoves() << endl;
          }
          break;

        case 8:
          if (line != bitset<64>(b.bKnightMoves()).to_string()) {
            cout << "Black Knight moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.bKnightMoves() << endl;
          }
          break;

        case 9:
          if (line != bitset<64>(b.bBishopMoves()).to_string()) {
            cout << "Black Bishop moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.bBishopMoves() << endl;
          }
          break;

        case 10:
          if (line != bitset<64>(b.bRookMoves()).to_string()) {
            cout << "Black Rook moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.bRookMoves() << endl;
          }
          break;

        case 11:
          if (line != bitset<64>(b.bQueenMoves()).to_string()) {
            cout << "Black Queen moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.bQueenMoves() << endl;
          }
          break;

        case 12:
          if (line != bitset<64>(b.bKingMoves()).to_string()) {
            cout << "Black King moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << bitset<64>(b.bKingMoves()).to_string() << endl;
            Print(b.bKingMoves());
          }
          linenum=-1;
          break;
        }
      linenum+=1;
    }
    cout<<"done"<<endl;
    FENfile.close();
  } else {
    std::cerr << "Unable to open file" << std::endl;
  }
  return 0;
}
