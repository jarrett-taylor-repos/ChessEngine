
#include "..\Bitboard\U64Bitboard.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>
using namespace std;

int main() {
  std::string line;
  std::ifstream FENfile("test_files/pseudoMoves.txt");
  freopen("wrong_pseudo_moves.txt","w",stdout);

  if (FENfile.is_open()) {
    int linenum = 0;
    U64Bitboard b;
    string FEN = "";
    while (getline(FENfile, line)) {
      switch (linenum) {
        case 0:
          b.LoadFen(line);
          FEN = line;
          break;
        case 1:
          if (line!=b.wPawnMoves().to_string()) {
            cout<<"White Pawn moves are incorrect for FEN: "<<FEN<<endl;
            cout<<"file bitset:  "<<line<<endl<<"board Bitset: "<<b.wPawnMoves()<<endl;
            PrintBitSet(b.wPawnMoves());
          }
          break;
        case 2:
          if (line!=b.wKnightMoves().to_string()) {
            cout<<"White Knight moves are incorrect for FEN: "<<FEN<<endl;
            cout<<"file bitset:  "<<line<<endl<<"board Bitset: "<<b.wKnightMoves()<<endl;
            PrintBitSet(b.wKnightMoves());
          }
          break;
        case 3:
          if (line != b.wBishopMoves().to_string()) {
            cout << "White Bishop moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.wBishopMoves() << endl;
            PrintBitSet(b.wBishopMoves());
          }
          break;

        case 4:
          if (line != b.wRookMoves().to_string()) {
            cout << "White Rook moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.wRookMoves() << endl;
            PrintBitSet(b.wRookMoves());
          }
          break;
        case 5:
          if (line != b.wQueenMoves().to_string()) {
            cout << "White Queen moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.wQueenMoves() << endl;
            PrintBitSet(b.wQueenMoves());
          }
          break;

        case 6:
          if (line != b.wKingMoves().to_string()) {
            cout << "White King moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.wKingMoves() << endl;
            PrintBitSet(b.wKingMoves());
          }
          break;

        case 7:
          if (line != b.bPawnMoves().to_string()) {
            cout << "Black Pawn moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.bPawnMoves() << endl;
            PrintBitSet(b.bPawnMoves());
          }
          break;

        case 8:
          if (line != b.bKnightMoves().to_string()) {
            cout << "Black Knight moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.bKnightMoves() << endl;
            PrintBitSet(b.bKnightMoves());
          }
          break;

        case 9:
          if (line != b.bBishopMoves().to_string()) {
            cout << "Black Bishop moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.bBishopMoves() << endl;
            PrintBitSet(b.bBishopMoves());
          }
          break;

        case 10:
          if (line != b.bRookMoves().to_string()) {
            cout << "Black Rook moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.bRookMoves() << endl;
            PrintBitSet(b.bRookMoves());
          }
          break;

        case 11:
          if (line != b.bQueenMoves().to_string()) {
            cout << "Black Queen moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.bQueenMoves() << endl;
            PrintBitSet(b.bQueenMoves());
          }
          break;

        case 12:
          if (line != b.bKingMoves().to_string()) {
            cout << "Black King moves are incorrect for FEN: " << FEN << endl;
            cout << "file bitset:  " << line << endl << "board Bitset: " << b.bKingMoves() << endl;
            PrintBitSet(b.bKingMoves());
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
