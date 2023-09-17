#include "bitboardhelper.cpp"

class Bitboard {
    private:
    BitBoardHelper Whitepawn;
    BitBoardHelper Whiteknight;
    BitBoardHelper Whitebishop;
    BitBoardHelper Whiterook;
    BitBoardHelper Whitequeen;
    BitBoardHelper Whiteking;

    BitBoardHelper Blackpawn;
    BitBoardHelper Blackknight;
    BitBoardHelper Blackbishop;
    BitBoardHelper Blackook;
    BitBoardHelper Blackqueen;
    BitBoardHelper Blackking;

    public:
    Bitboard() {
        string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
        Whitepawn.SetFenAndPiece(fen, 1001);
        Whiteknight.SetFenAndPiece(fen, 1010);
        Whitebishop.SetFenAndPiece(fen, 1011);
        Whiterook.SetFenAndPiece(fen, 1100);
        Whitequeen.SetFenAndPiece(fen, 1101);
        Whiteking.SetFenAndPiece(fen, 1110);

        Blackpawn.SetFenAndPiece(fen, 0001);
        Blackknight.SetFenAndPiece(fen, 0010);
        Blackbishop.SetFenAndPiece(fen, 0011);
        Blackook.SetFenAndPiece(fen, 0100);
        Blackqueen.SetFenAndPiece(fen, 0101);
        Blackking.SetFenAndPiece(fen, 0110);
    };

    Bitboard(string fen) {
        Whitepawn.SetFenAndPiece(fen, 1001);
        Whiteknight.SetFenAndPiece(fen, 1010);
        Whitebishop.SetFenAndPiece(fen, 1011);
        Whiterook.SetFenAndPiece(fen, 1100);
        Whitequeen.SetFenAndPiece(fen, 1101);
        Whiteking.SetFenAndPiece(fen, 1110);

        Blackpawn.SetFenAndPiece(fen, 0001);
        Blackknight.SetFenAndPiece(fen, 0010);
        Blackbishop.SetFenAndPiece(fen, 0011);
        Blackook.SetFenAndPiece(fen, 0100);
        Blackqueen.SetFenAndPiece(fen, 0101);
        Blackking.SetFenAndPiece(fen, 0110);
    };


    BitBoardHelper GetWhitepawn() { return Whitepawn; };
    BitBoardHelper GetWhiteknight() { return Whiteknight; };
    BitBoardHelper GetWhitebishop() { return Whitebishop; };
    BitBoardHelper GetWhiterook() { return Whiterook; };
    BitBoardHelper GetWhitequeen() { return Whitequeen; };
    BitBoardHelper GetWhiteking() { return Whiteking; };
    BitBoardHelper GetBlackpawn() { return Blackpawn; };
    BitBoardHelper GetBlackknight() { return Blackknight; };
    BitBoardHelper GetBlackbishop() { return Blackbishop; };
    BitBoardHelper GetBlackrook() { return Blackook; };
    BitBoardHelper GetBlackqueen() { return Blackqueen; };
    BitBoardHelper GetBlackking() { return Blackking; };

};