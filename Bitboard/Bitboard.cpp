#include "bitboardhelper.cpp"

class Bitboard {
    private:
    BitBoardHelper wPawn;
    BitBoardHelper wKnight;
    BitBoardHelper wBishop;
    BitBoardHelper wRook;
    BitBoardHelper wQueen;
    BitBoardHelper wKing;

    BitBoardHelper bPawn;
    BitBoardHelper bKnight;
    BitBoardHelper bBishop;
    BitBoardHelper bRook;
    BitBoardHelper bQueen;
    BitBoardHelper bKing;

    public:
    Bitboard() {
        string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
        wPawn.LoadFen(fen, 1001);
        wKnight.LoadFen(fen, 1010);
        wBishop.LoadFen(fen, 1011);
        wRook.LoadFen(fen, 1100);
        wQueen.LoadFen(fen, 1101);
        wKing.LoadFen(fen, 1110);

        bPawn.LoadFen(fen, 0001);
        bKnight.LoadFen(fen, 0010);
        bBishop.LoadFen(fen, 0011);
        bRook.LoadFen(fen, 0100);
        bQueen.LoadFen(fen, 0101);
        bKing.LoadFen(fen, 0110);
    };

    Bitboard(string fen) {
        wPawn.LoadFen(fen, 1001);
        wKnight.LoadFen(fen, 1010);
        wBishop.LoadFen(fen, 1011);
        wRook.LoadFen(fen, 1100);
        wQueen.LoadFen(fen, 1101);
        wKing.LoadFen(fen, 1110);

        bPawn.LoadFen(fen, 0001);
        bKnight.LoadFen(fen, 0010);
        bBishop.LoadFen(fen, 0011);
        bRook.LoadFen(fen, 0100);
        bQueen.LoadFen(fen, 0101);
        bKing.LoadFen(fen, 0110);
    };

    BitBoardHelper GetwPawn() { return wPawn; };
    BitBoardHelper GetwKnight() { return wKnight; };
    BitBoardHelper GetwBishop() { return wBishop; };
    BitBoardHelper GetwRook() { return wRook; };
    BitBoardHelper GetwQueen() { return wQueen; };
    BitBoardHelper GetwKing() { return wKing; };
    BitBoardHelper GetbPawn() { return bPawn; };
    BitBoardHelper GetbKnight() { return bKnight; };
    BitBoardHelper GetbBishop() { return bBishop; };
    BitBoardHelper GetbRook() { return bRook; };
    BitBoardHelper GetbQueen() { return bQueen; };
    BitBoardHelper GetbKing() { return bKing; };

    bool GetBit(int square) {
        return wPawn.GetBit(square) || wKnight.GetBit(square) || wBishop.GetBit(square) || wRook.GetBit(square) || wQueen.GetBit(square) || wKing.GetBit(square) || 
            bPawn.GetBit(square) || bKnight.GetBit(square) || bBishop.GetBit(square) || bRook.GetBit(square) || bQueen.GetBit(square) || bKing.GetBit(square);
    };

    bitset<64> wBoard() {
        return wPawn.GetBoard() | wKnight.GetBoard() | wBishop.GetBoard() | wRook.GetBoard() | wQueen.GetBoard() | wKing.GetBoard();
    }
    bitset<64> bBoard() {
        return bPawn.GetBoard() | bKnight.GetBoard() | bBishop.GetBoard() | bRook.GetBoard() | bQueen.GetBoard() | bKing.GetBoard();
    }
    bitset<64> allBoard() {
        return wPawn.GetBoard() | wKnight.GetBoard() | wBishop.GetBoard() | wRook.GetBoard() | wQueen.GetBoard() | wKing.GetBoard() |
            bPawn.GetBoard() | bKnight.GetBoard() | bBishop.GetBoard() | bRook.GetBoard() | bQueen.GetBoard() | bKing.GetBoard();
    }

    void PrintAllBoards() {
        wPawn.PrintBitBoard();
        cout << endl;
        bPawn.PrintBitBoard();
        cout << endl;
        wKnight.PrintBitBoard();
        cout << endl;
        bKnight.PrintBitBoard();
        cout << endl;
        wBishop.PrintBitBoard();
        cout << endl;
        bBishop.PrintBitBoard();
        cout << endl;
        wRook.PrintBitBoard();
        cout << endl;
        bRook.PrintBitBoard();
        cout << endl;
        wQueen.PrintBitBoard();
        cout << endl;
        bQueen.PrintBitBoard();
        cout << endl;
        wKing.PrintBitBoard();
        cout << endl;
        bKing.PrintBitBoard();
        cout << endl;
    }
};