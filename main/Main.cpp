#include "..\Bitboard\Bitboard.cpp"
#include "..\Bitboard\BitboardHelper.cpp"
#include "..\MoveEngine\MoveCoordinates.h"
using namespace MoveCoordinates;

int main () {
    Bitboard board;
    //board.PrintAllBoards();


    bitset<8> test (string("00001111"));
    bitset<8> test2 (string("11110000"));

    cout << "test: " << test << endl;
    cout << "test2: " << test2 << endl;

    bitset<8> resultOr = test | test2;
    bitset<8> resultAnd = test & test2;
    cout << "resultOr: " << resultOr << endl;
    cout << "resultAnd: " << resultAnd << endl;
}