#include "..\Bitboard\Bitboard.cpp"
#include "..\Bitboard\BitboardHelper.cpp"

int main () {
    Bitboard board;
    BitBoardHelper whitepawn = board.GetWhitepawn();
    BitBoardHelper blackpawn = board.GetBlackpawn();
    whitepawn.PrintBitBoard();
    cout << endl;
    blackpawn.PrintBitBoard();
    cout << endl;

    BitBoardHelper whiteknight = board.GetWhiteknight();
    BitBoardHelper blackknight = board.GetBlackknight();
    whiteknight.PrintBitBoard();
    cout << endl;
    blackknight.PrintBitBoard();
    cout << endl;

    BitBoardHelper whitebishop = board.GetWhitebishop();
    BitBoardHelper blackbishop = board.GetBlackbishop();
    whitebishop.PrintBitBoard();
    cout << endl;
    blackbishop.PrintBitBoard();
    cout << endl;

    BitBoardHelper whiterook = board.GetWhiterook();
    BitBoardHelper blackrook = board.GetBlackrook();
    whiterook.PrintBitBoard();
    cout << endl;
    blackrook.PrintBitBoard();
    cout << endl;

    BitBoardHelper whitequeen = board.GetWhitequeen();
    BitBoardHelper blackqueen = board.GetBlackqueen();
    whitequeen.PrintBitBoard();
    cout << endl;
    blackqueen.PrintBitBoard();
    cout << endl;

    BitBoardHelper whiteking = board.GetWhiteking();
    BitBoardHelper blackking = board.GetBlackking();
    whiteking.PrintBitBoard();
    cout << endl;
    blackking.PrintBitBoard();
    cout << endl;
}