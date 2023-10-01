
#include "..\Bitboard\Bitboard.cpp"
#include <iostream>
#include <fstream>
#include <string>

int main() {
  std::string FEN;
  std::ifstream FENfile("test_files/pieceLocations.txt");

  if (FENfile.is_open()) {
    int line = 0;
    while (getline(FENfile, FEN)) {
      switch (line) {
        case 1:
          Bitboard b(FEN);
          break;
        case 2:
          break;
      }
    }
    FENfile.close();
  } else {
    std::cerr << "Unable to open file" << std::endl;
  }

  return 0;
}
