
#include "..\src\Bitboard\Bitboard.cpp"
#include <iostream>
#include <fstream>
#include <string>

int main() {
  std::string FEN;
  std::ifstream FENfile("test_files/validFENs.txt");

  if (FENfile.is_open()) {
    while (getline(FENfile, FEN)) {
      //std::cout << "Testing FEN: " << FEN << std::endl;
      Bitboard x(FEN); // Use the FEN read from the file
    }
    FENfile.close();
  } else {
    std::cerr << "Unable to open file" << std::endl;
  }

  return 0;
}
