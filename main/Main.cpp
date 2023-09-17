#include "..\Bitboard\Bitboard.cpp"
#include "..\Bitboard\BitboardHelper.cpp"

int main () {
    cout << "MAIN" << endl;
    Bitboard board;
    BitBoardHelper whitepawn = board.GetWhitepawn();
    BitBoardHelper blackrook = board.GetBlackrook();
    BitBoardHelper whiteking = board.GetWhiteking();
    BitBoardHelper blackking = board.GetBlackking();
    whitepawn.Print();
    cout << endl;
    blackrook.Print();
    cout << endl;
    whiteking.Print();
    cout << endl;
    blackking.Print();
}