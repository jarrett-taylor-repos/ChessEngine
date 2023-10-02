
#include "..\Bitboard\Bitboard.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>
using namespace std;

int main() {
  std::string line;
  std::ifstream FENfile("test_files/pseudoMoves.txt");
  freopen("wrong_pseudo_moves.tst","w",stdout);

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
          // if (line != b.GetwBishop().GetBoard().to_string()) {
          //   cout << "White Bishop moves are incorrect for FEN: " << FEN << endl;
          //   cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetwBishop().GetBoard().to_string() << endl;
          // }
          break;
        case 4:
          // if (line != b.GetwRook().GetBoard().to_string()) {
          //   cout << "White Rook moves are incorrect for FEN: " << FEN << endl;
          //   cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetwRook().GetBoard().to_string() << endl;
          // }
          break;
        case 5:
          // if (line != b.GetwQueen().GetBoard().to_string()) {
          //   cout << "White Queen moves are incorrect for FEN: " << FEN << endl;
          //   cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetwQueen().GetBoard().to_string() << endl;
          // }
          break;
        case 6:
          if (line != b.wKingMoves().to_string()) {
            cout << "White King moves are incorrect for FEN: " << FEN << endl;
            cout << "File bitset:  " << line << endl << "Board Bitset: " << b.wKingMoves().to_string() << endl;
            PrintBitSet(b.wKingMoves());
          }
          break;
        case 7:
          // Black pieces
          if (line != b.bPawnMoves().to_string()) {
            cout << "Black Pawn moves are incorrect for FEN: " << FEN << endl;
            cout << "File bitset:  " << line << endl << "Board Bitset: " << b.bPawnMoves().to_string() << endl;
            PrintBitSet(b.bPawnMoves());
          }
          break;
        case 8:
          if (line != b.bKnightMoves().to_string()) {
            cout << "Black Knight moves are incorrect for FEN: " << FEN << endl;
            cout << "File bitset:  " << line << endl << "Board Bitset: " << b.bKnightMoves().to_string() << endl;
            PrintBitSet(b.bKnightMoves());
          }
          break;
        case 9:
          // if (line != b.GetbBishop().GetBoard().to_string()) {
          //   cout << "Black Bishop moves are incorrect for FEN: " << FEN << endl;
          //   cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbBishop().GetBoard().to_string() << endl;
          // }
          break;
        case 10:
          // if (line != b.GetbRook().GetBoard().to_string()) {
          //   cout << "Black Rook moves are incorrect for FEN: " << FEN << endl;
          //   cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbRook().GetBoard().to_string() << endl;
          // }
          break;
        case 11:
          // if (line != b.GetbQueen().GetBoard().to_string()) {
          //   cout << "Black Queen moves are incorrect for FEN: " << FEN << endl;
          //   cout << "File bitset: " << line << endl << "Board Bitset: " << b.GetbQueen().GetBoard().to_string() << endl;
          // }
          break;
        case 12:
          if (line != b.bKingMoves().to_string()) {
            cout << "Black King moves are incorrect for FEN: " << FEN << endl;
            cout << "File bitset:  " << line << endl << "Board Bitset: " << b.bKingMoves().to_string() << endl;
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
