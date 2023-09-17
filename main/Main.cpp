#include "..\Bitboard\Bitboard.cpp"
#include "..\Bitboard\BitboardHelper.cpp"

int main () {
    cout << "MAIN" << endl;
    Bitboard board;
    BitBoardHelper whitepawn = board.GetWhitepawn();
    BitBoardHelper blackpawn = board.GetBlackpawn();
    BitBoardHelper blackrook = board.GetBlackrook();
    BitBoardHelper whiteking = board.GetWhiteking();
    BitBoardHelper blackking = board.GetBlackking();
    whitepawn.PrintBitBoard();
    cout << endl;
    blackpawn.PrintBitBoard();
    cout << endl;
    blackrook.PrintBitBoard();
}