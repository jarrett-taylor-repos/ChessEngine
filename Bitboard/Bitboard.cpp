#include "bitboardhelper.cpp"
#include "..\Extensions\BitboardConstants.h"
using namespace BitboardConstants;

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

    //board functions
    bitset<64> wBoard() {
        return wPawn.GetBoard() | wKnight.GetBoard() | wBishop.GetBoard() | wRook.GetBoard() | wQueen.GetBoard() | wKing.GetBoard();
    };
    bitset<64> bBoard() {
        return bPawn.GetBoard() | bKnight.GetBoard() | bBishop.GetBoard() | bRook.GetBoard() | bQueen.GetBoard() | bKing.GetBoard();
    };
    bitset<64> AllBoard() {
        return wPawn.GetBoard() | wKnight.GetBoard() | wBishop.GetBoard() | wRook.GetBoard() | wQueen.GetBoard() | wKing.GetBoard() |
            bPawn.GetBoard() | bKnight.GetBoard() | bBishop.GetBoard() | bRook.GetBoard() | bQueen.GetBoard() | bKing.GetBoard();
    };

    bitset<64> EmptyBoard() { return ~AllBoard(); };
    bitset<64> NotbBoard() { return ~bBoard(); };
    bitset<64> NotwBoard() { return ~wBoard(); };

    //pawn functions
    bitset<64> northOne(bitset<64> b) { return b >> 8; };
    bitset<64> southOne(bitset<64> b) { return b << 8; };
    bitset<64> wSinglePushTargets(bitset<64> b) { return northOne(b) & EmptyBoard(); };
    bitset<64> wDoublePushTargets(bitset<64> b) { 
        bitset<64> singlePushes = wSinglePushTargets(b);
        return northOne(singlePushes) & EmptyBoard() & rank4; 
    };
    bitset<64> bSinglePushTargets(bitset<64> b) { return southOne(b) & EmptyBoard(); };
    bitset<64> bDoublePushTargets(bitset<64> b) {
        bitset<64> singlePushes = bSinglePushTargets(b);
        return southOne(singlePushes) & EmptyBoard() & rank5; 
    };
    bitset<64> wPawnPushes() { return wSinglePushTargets(wPawn.GetBoard()) | wDoublePushTargets(wPawn.GetBoard()); };
    bitset<64> bPawnPushes() { return bSinglePushTargets(bPawn.GetBoard()) | bDoublePushTargets(bPawn.GetBoard()); };

    bitset<64> wPawnWestAtt() { return (wPawn.GetBoard() >> 9) & notAFile; };
    bitset<64> wPawnEastAtt() { return (wPawn.GetBoard() >> 7) & notHFile; };
    bitset<64> wPawnAllAtt() { return wPawnEastAtt() | wPawnWestAtt(); };
    bitset<64> bPawnWestAtt() { return (bPawn.GetBoard() << 9) & notHFile; };
    bitset<64> bPawnEastAtt() { return (bPawn.GetBoard() << 7) & notAFile; };
    bitset<64> bPawnAllAtt() { return bPawnEastAtt() | bPawnWestAtt(); };
    bitset<64> wPawnWestCaptures() { return wPawnWestAtt() & bBoard(); };
    bitset<64> wPawnEastCaptures() { return wPawnEastAtt() & bBoard(); };
    bitset<64> wPawnAllCaptures() { return wPawnEastCaptures() | wPawnWestCaptures(); };
    bitset<64> bPawnWestCaptures() { return bPawnWestAtt() & wBoard(); };
    bitset<64> bPawnEastCaptures() { return bPawnEastAtt() & wBoard(); };
    bitset<64> bPawnAllCaptures() { return bPawnEastCaptures() | bPawnWestCaptures(); };


    //knight functions
    bitset<64> KnightMoveHelper(bitset<64> b) { return noNoEa(b) | noEaEa(b) | soEaEa(b) | soSoEa(b) | noNoWe(b) | noWeWe(b) | soWeWe(b) | soSoWe(b); }
    bitset<64> noNoEa(bitset<64> b) {return (b >> 15) & notAFile; }
    bitset<64> noEaEa(bitset<64> b) {return (b >> 6) & (notAFile & notBFile);}
    bitset<64> soEaEa(bitset<64> b) {return (b << 10) & (notAFile & notBFile);}
    bitset<64> soSoEa(bitset<64> b) {return (b << 17) & notAFile; }
    bitset<64> soSoWe(bitset<64> b) {return (b << 15) & notHFile; }
    bitset<64> soWeWe(bitset<64> b) {return (b << 6) & (notGFile & notHFile); }
    bitset<64> noWeWe(bitset<64> b) {return (b >> 10) & (notGFile & notHFile); }
    bitset<64> noNoWe(bitset<64> b) {return (b >> 17) & notHFile; }

    bitset<64> wKnightMoves(){ return KnightMoveHelper(wKnight.GetBoard()) & NotwBoard(); }
    bitset<64> bKnightMoves(){ return KnightMoveHelper(bKnight.GetBoard()) & NotbBoard(); }

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