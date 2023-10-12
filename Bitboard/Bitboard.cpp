#include "..\Extensions\Extentions.h"
#include <ctype.h>
#include <stdio.h>
#include <unordered_map>
using namespace Extensions;

class Bitboard {
    private:
    bitset<64> wPawn;
    bitset<64> wKnight;
    bitset<64> wBishop;
    bitset<64> wRook;
    bitset<64> wQueen;
    bitset<64> wKing;

    bitset<64> bPawn;
    bitset<64> bKnight;
    bitset<64> bBishop;
    bitset<64> bRook;
    bitset<64> bQueen;
    bitset<64> bKing;

    int halfMoveClock;
    int fullTurnNum;
    string enPassantTarget;
    string currFen;
    bool isWhiteMove;
    string castlingRights;
    map<string, int> hashFen; //used to see 3 move repition of fen, will be used in the future



    public:
    Bitboard() { LoadFen(startFen); };
    Bitboard(string fen) { LoadFen(fen); };

    void LoadFenHelper(vector<string> arguments) {
        ClearBoard();
        currFen = arguments[0];
        string moveColor = arguments[1];
        isWhiteMove = moveColor == "w";
        castlingRights = arguments[2];
        enPassantTarget = arguments[3];
        fullTurnNum = stoi(arguments[4]);
        halfMoveClock = stoi(arguments[5]);
    };

    void SetBoard(char c, int sq) {
        switch(c) {
            case 'p':
                bPawn.set(sq);return;
            case 'b':
                bBishop.set(sq);return;
            case 'n':
                bKnight.set(sq);return;
            case 'r':
                bRook.set(sq);return;
            case 'q':
                bQueen.set(sq);return;
            case 'k':
                bKing.set(sq);return;
            case 'P':
                wPawn.set(sq);return;
            case 'B':
                wBishop.set(sq);return;
            case 'N':
                wKnight.set(sq);return;
            case 'R':
                wRook.set(sq);return;
            case 'Q':
                wQueen.set(sq);return;
            case 'K':
                wKing.set(sq);return;
        }
    }

    void LoadFen(string fen) {
        LoadFenHelper(Split(fen));

        int sq = 0;
        for(int i = 0; i < currFen.length(); i++) {
            char curr = currFen[i];
            if(curr != '/') {
                bool isInt = isdigit(curr);

                if(isInt) {
                    int value = CharToInt(curr);
                    sq += value;
                } else {
                    SetBoard(curr, sq);
                    sq++;
                }
            }
        }
    };

    string GetFen() {
        bitset<64> allb = AllBoard();
        string fen = "";
        int temp = 0;
        for(int i = 0; i < 64; i++) {
            if((i != 0) && (i % 8 == 0)) { 
                if(temp != 0) {
                    fen += to_string(temp);
                    temp = 0;
                }
                fen += "/";  
            }

            if(allb.test(i)) {
                if(temp != 0) {
                    fen += to_string(temp);
                }
                fen += GetPieceAtIndex(i);
                temp=0;
            } else {
                temp++;
            }
        }
        string moveColor = isWhiteMove ? " w" : " b";
        string fenOthers = moveColor +" "+ castlingRights +" "+ enPassantTarget +" "+ to_string(halfMoveClock) +" "+ to_string(fullTurnNum);
        fen += fenOthers;

        return fen;
    };

    string GetPieceAtIndex(int index) {
        if(wPawn.test(index)) return "P";
        if(wKnight.test(index)) return "N";
        if(wBishop.test(index)) return "B";
        if(wRook.test(index)) return "R";
        if(wQueen.test(index)) return "Q";
        if(wKing.test(index)) return "K";

        if(bPawn.test(index)) return "p";
        if(bKnight.test(index)) return "n";
        if(bBishop.test(index)) return "b";
        if(bRook.test(index)) return "r";
        if(bQueen.test(index)) return "q";
        if(bKing.test(index)) return "k";
    };

    void ClearBoard() {
        wPawn.reset();
        wKnight.reset();
        wBishop.reset();
        wRook.reset();
        wQueen.reset();
        wKing.reset();

        bPawn.reset();
        bKnight.reset();
        bBishop.reset();
        bRook.reset();
        bQueen.reset();
        bKing.reset();
    };

    bitset<64> GetwPawn() { return wPawn; };
    bitset<64> GetwKnight() { return wKnight; };
    bitset<64> GetwBishop() { return wBishop; };
    bitset<64> GetwRook() { return wRook; };
    bitset<64> GetwQueen() { return wQueen; };
    bitset<64> GetwKing() { return wKing; };
    bitset<64> GetbPawn() { return bPawn; };
    bitset<64> GetbKnight() { return bKnight; };
    bitset<64> GetbBishop() { return bBishop; };
    bitset<64> GetbRook() { return bRook; };
    bitset<64> GetbQueen() { return bQueen; };
    bitset<64> GetbKing() { return bKing; };

    //board functions
    bitset<64> wBoard() { return wPawn | wKnight | wBishop | wRook | wQueen | wKing; };
    bitset<64> bBoard() { return bPawn | bKnight | bBishop | bRook | bQueen | bKing; };
    bitset<64> AllBoard() { return wBoard() | bBoard(); };

    bitset<64> EmptyBoard() { return ~AllBoard(); };
    bitset<64> NotbBoard() { return ~bBoard(); };
    bitset<64> NotwBoard() { return ~wBoard(); };

    //generic movement 
    bitset<64> OneInAllDirection(bitset<64> b) { return (northOne(b) | southOne(b) | eastOne(b) | westOne(b) | northEastOne(b) | northWestOne(b) | southEastOne(b) | southWestOne(b)); };
    bitset<64> northOne(bitset<64> b) { return b >> 8; };
    bitset<64> southOne(bitset<64> b) { return b << 8; };
    bitset<64> eastOne(bitset<64> b) { return b << 1 & notAFile; };
    bitset<64> westOne(bitset<64> b) { return b >> 1 & notHFile; };
    bitset<64> northEastOne(bitset<64> b) { return b >> 7 & notAFile; };
    bitset<64> northWestOne(bitset<64> b) { return b >> 9 & notHFile; };
    bitset<64> southEastOne(bitset<64> b) { return b << 9 & notAFile; };
    bitset<64> southWestOne(bitset<64> b) { return b << 7 & notHFile; };
    bool InBounds(int offset) { return (offset >= 0) && (offset < 64); };

    //pawn functions
    bitset<64> wSinglePushTargets(bitset<64> b) { return northOne(b) & EmptyBoard(); };
    bitset<64> wDoublePushTargets(bitset<64> b) { return northOne(wSinglePushTargets(b)) & EmptyBoard() & rank4; };
    bitset<64> bSinglePushTargets(bitset<64> b) { return southOne(b) & EmptyBoard(); };
    bitset<64> bDoublePushTargets(bitset<64> b) { return southOne(bSinglePushTargets(b)) & EmptyBoard() & rank5; };
    bitset<64> wPawnPushes() { return wSinglePushTargets(wPawn) | wDoublePushTargets(wPawn); };
    bitset<64> bPawnPushes() { return bSinglePushTargets(bPawn) | bDoublePushTargets(bPawn); };

    bitset<64> wPawnWestAtt() { return (wPawn >> 9) & notHFile; };
    bitset<64> wPawnEastAtt() { return (wPawn >> 7) & notAFile; };
    bitset<64> wPawnAllAtt() { return wPawnEastAtt() | wPawnWestAtt(); };
    bitset<64> bPawnWestAtt() { return (bPawn << 9) & notAFile; };
    bitset<64> bPawnEastAtt() { return (bPawn << 7) & notHFile; };
    bitset<64> bPawnAllAtt() { return bPawnEastAtt() | bPawnWestAtt(); };
    bitset<64> wPawnWestCaptures() { return wPawnWestAtt() & bBoard(); };
    bitset<64> wPawnEastCaptures() { return wPawnEastAtt() & bBoard(); };
    bitset<64> wPawnAllCaptures() { return wPawnEastCaptures() | wPawnWestCaptures(); };
    bitset<64> bPawnWestCaptures() { return bPawnWestAtt() & wBoard(); };
    bitset<64> bPawnEastCaptures() { return bPawnEastAtt() & wBoard(); };
    bitset<64> bPawnAllCaptures() { return bPawnEastCaptures() | bPawnWestCaptures(); };

    bitset<64> wPawnMoves() { return wPawnAllCaptures() | wPawnPushes(); };
    bitset<64> bPawnMoves() { return bPawnAllCaptures() | bPawnPushes(); };

    vector<string> wPawnWestCapturesUCI() { 
        bitset<64> wPawnWestCap = wPawnWestAtt() & bBoard(); 
        bitset<64> wPawnOrigin = wPawnWestCap << 9; 
        return BitsetToUCI(wPawnOrigin, wPawnWestCap, true, true);
    };
    vector<string> wPawnEastCapturesUCI() {  
        bitset<64> wPawnEastCap = wPawnEastAtt() & bBoard(); 
        bitset<64> wPawnOrigin = wPawnEastCap << 7; 
        return BitsetToUCI(wPawnOrigin, wPawnEastCap, true, true);
    };

    vector<string> wPawnMovesAllCapturesUCI() { return Combine(wPawnWestCapturesUCI(), wPawnEastCapturesUCI()); };

    vector<string> wPawnSinglePushesUCI() {
        bitset<64> wPawnBoard = wPawn;
        bitset<64> wPawnSinglePushes = wSinglePushTargets(wPawnBoard);
        bitset<64> wPawnSinglePushesOriginal = wPawnSinglePushes << 8;
        return BitsetToUCI(wPawnSinglePushesOriginal, wPawnSinglePushes, true, true);
    };
    vector<string> wPawnDoublePushesUCI() {
        bitset<64> wPawnBoard = wPawn;
        bitset<64> wPawnDoublePushes = wDoublePushTargets(wPawnBoard);
        bitset<64> wPawDoublePushesOriginal = wPawnDoublePushes << 16;
        return BitsetToUCI(wPawDoublePushesOriginal, wPawnDoublePushes);
    };

    vector<string> wPawnMovesPushesUCI() { return Combine(wPawnSinglePushesUCI(), wPawnDoublePushesUCI()); };
    vector<string> wPawnMovesUCI() { return Combine(wPawnMovesAllCapturesUCI(), wPawnMovesPushesUCI()); };

    vector<string> bPawnWestCapturesUCI() { 
        //need to check fro promo
        bitset<64> bPawnWestCap = bPawnWestAtt() & wBoard(); 
        bitset<64> bPawnOrigin = bPawnWestCap >> 9; 
        return BitsetToUCI(bPawnOrigin, bPawnWestCap, true, false);
    };
    vector<string> bPawnEastCapturesUCI() {  
        //need to check for promo
        bitset<64> bPawnEastCap = bPawnEastAtt() & wBoard(); 
        bitset<64> bPawnOrigin = bPawnEastCap >> 7; 
        return BitsetToUCI(bPawnOrigin, bPawnEastCap, true, false);
    };

    vector<string> bPawnMovesAllCapturesUCI() { return Combine(bPawnWestCapturesUCI(), bPawnEastCapturesUCI()); };

    vector<string> bPawnSinglePushesUCI() {
        //need to check for promo
        bitset<64> bPawnBoard = bPawn;
        bitset<64> bPawnSinglePushes = bSinglePushTargets(bPawnBoard);
        bitset<64> bPawnSinglePushesOriginal = bPawnSinglePushes >> 8;
        return BitsetToUCI(bPawnSinglePushesOriginal, bPawnSinglePushes, true, false);
    };
    vector<string> bPawnDoublePushesUCI() {
        bitset<64> bPawnBoard = bPawn;
        bitset<64> bPawnDoublePushes = bDoublePushTargets(bPawnBoard);
        bitset<64> bPawnDoublePushesOriginal = bPawnDoublePushes >> 16;
        return BitsetToUCI(bPawnDoublePushesOriginal, bPawnDoublePushes);
    };

    vector<string> bPawnMovesPushesUCI() { return Combine(bPawnSinglePushesUCI(), bPawnDoublePushesUCI()); };
    vector<string> bPawnMovesUCI() { return Combine(bPawnMovesAllCapturesUCI(), bPawnMovesPushesUCI()); };


    //knight functions
    bitset<64> KnightMoveHelper(bitset<64> b) { return noNoEa(b) | noEaEa(b) | soEaEa(b) | soSoEa(b) | noNoWe(b) | noWeWe(b) | soWeWe(b) | soSoWe(b); };
    bitset<64> noNoEa(bitset<64> b) {return (b >> 15) & notAFile; };
    bitset<64> noEaEa(bitset<64> b) {return (b >> 6) & (notAFile & notBFile);};
    bitset<64> soEaEa(bitset<64> b) {return (b << 10) & (notAFile & notBFile);};
    bitset<64> soSoEa(bitset<64> b) {return (b << 17) & notAFile; };
    bitset<64> soSoWe(bitset<64> b) {return (b << 15) & notHFile; };
    bitset<64> soWeWe(bitset<64> b) {return (b << 6) & (notGFile & notHFile); };
    bitset<64> noWeWe(bitset<64> b) {return (b >> 10) & (notGFile & notHFile); };
    bitset<64> noNoWe(bitset<64> b) {return (b >> 17) & notHFile; };

    bitset<64> wKnightMoves(){ return KnightMoveHelper(wKnight) & NotwBoard(); };
    bitset<64> bKnightMoves(){ return KnightMoveHelper(bKnight) & NotbBoard(); };

    vector<string> wKnightMovesUCI() { 
        vector<string> ucimoves;
        vector<int> indexes = BitSetTrueIndexes(wKnight);
        for(int sq : indexes) {
            bitset<64> helper;
            helper.set(sq);
            bitset<64> moves = KnightMoveHelper(helper) & NotwBoard();
            vector<string> currMoves = BitsetToUCI(IndexToSquare(sq), moves);
            ucimoves = Combine(ucimoves, currMoves);
        }
        return ucimoves;
    };

    vector<string> bKnightMovesUCI() { 
        vector<string> ucimoves;
        vector<int> indexes = BitSetTrueIndexes(bKnight);
        for(int sq : indexes) {
            bitset<64> helper;
            helper.set(sq);
            bitset<64> moves = KnightMoveHelper(helper) & NotbBoard();
            vector<string> currMoves = BitsetToUCI(IndexToSquare(sq), moves);
            ucimoves = Combine(ucimoves, currMoves);
        }
        return ucimoves;
    };

    //king moves
    bitset<64> wKingMoves() { return OneInAllDirection(wKing) & NotwBoard(); };
    bitset<64> bKingMoves() { return OneInAllDirection(bKing) & NotbBoard(); };

    vector<string> wKingMovesUCI() {
        int index = BitSetTrueIndexes(wKing).at(0);
        bitset<64> wkingmoves = wKingMoves();
        return BitsetToUCI(IndexToSquare(index), wkingmoves);
    };

    vector<string> bKingMovesUCI() {
        int index = BitSetTrueIndexes(bKing).at(0);
        bitset<64> bkingmoves = bKingMoves();
        return BitsetToUCI(IndexToSquare(index), bkingmoves);
    };

    //sliding moves helper
    bool ValidTravel(bitset<64> overlap, int offset) { return InBounds(offset) && overlap.test(offset) && !AllBoard().test(offset); };
    bool ValidTravelAtt(bitset<64> overlap, int offset) { return InBounds(offset) && overlap.test(offset) && AllBoard().test(offset); };

    bool ValidTravel(bitset<64> occ, bitset<64> overlap, int offset) { return InBounds(offset) && overlap.test(offset) && !occ.test(offset); };
    bool ValidTravelAtt(bitset<64> occ, bitset<64> overlap, int offset) { return InBounds(offset) && overlap.test(offset) && occ.test(offset); };

    bitset<64> SlidingAttacks(bitset<64> overlap, int sq, int direction) {
        bitset<64> moves;
        int offset = sq + direction;
        while(ValidTravel(overlap, offset)) {
            moves.set(offset);
            offset += direction;
        }
        if(ValidTravelAtt(overlap, offset)) {
            moves.set(offset);
        }
        return moves;
    };

    bitset<64> SlidingAttacks(bitset<64> occ, bitset<64> overlap, int sq, int direction) {
        bitset<64> moves;
        int offset = sq + direction;
        while(ValidTravel(occ, overlap, offset)) {
            moves.set(offset);
            offset += direction;
        }
        if(ValidTravelAtt(occ, overlap, offset)) {
            moves.set(offset);
        }
        return moves;
    };

    bitset<64> BishopAttacksBySquare(int sq) {
        bitset<64> moves;
        //travel northEastOne
        moves |= SlidingAttacks(~aFile, sq, -7);
        //travel northWestOne
        moves |= SlidingAttacks(~hFile, sq, -9);
        //travel southEastOne
        moves |= SlidingAttacks(~aFile, sq, 9);
        //travle southWestOne
        moves |= SlidingAttacks(~hFile, sq, 7);
        return moves;
    };

    bitset<64> BishopAttacksBySquare(bitset<64> occ, int sq) {
        bitset<64> moves;
        //travel northEastOne
        moves |= SlidingAttacks(occ, ~aFile, sq, -7);
        //travel northWestOne
        moves |= SlidingAttacks(occ, ~hFile, sq, -9);
        //travel southEastOne
        moves |= SlidingAttacks(occ, ~aFile, sq, 9);
        //travle southWestOne
        moves |= SlidingAttacks(occ, ~hFile, sq, 7);
        return moves;
    };

    bitset<64> BishopAttacks(bitset<64> bishopBoard) {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(bishopBoard);
        for(int sq : indexes) moves |= BishopAttacksBySquare(sq); 
        return moves;
    };

    bitset<64> BishopAttacks(bitset<64> bishopBoard, bitset<64> occ) {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(bishopBoard);
        for(int sq : indexes) moves |= BishopAttacksBySquare(occ, sq); 
        return moves;
    };

    bitset<64> RookAttacksBySquare(int sq) {
        bitset<64> moves;
        //travel northOne
        moves |= SlidingAttacks(~rank1, sq, -8);
        //travel eastOne
        moves |= SlidingAttacks(notAFile, sq, 1);
        //travel westOne
        moves |= SlidingAttacks(notHFile, sq, -1);
        //travle southOne
        moves |= SlidingAttacks(~rank8, sq, 8);
        return moves;
    };

    bitset<64> RookAttacksBySquare(bitset<64> occ, int sq) {
        bitset<64> moves;
        //travel northOne
        moves |= SlidingAttacks(occ, ~rank1, sq, -8);
        //travel eastOne
        moves |= SlidingAttacks(occ, ~aFile, sq, 1);
        //travel westOne
        moves |= SlidingAttacks(occ, ~hFile, sq, -1);
        //travle southOne
        moves |= SlidingAttacks(occ, ~rank8, sq, 8);
        return moves;
    };

    bitset<64> RookAttacks(bitset<64> rBoard) {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(rBoard);
        for(int sq : indexes) moves |= RookAttacksBySquare(sq);
        return moves;
    };

    bitset<64> RookAttacks(bitset<64> rBoard, bitset<64> occ) {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(rBoard);
        for(int sq : indexes) moves |= RookAttacksBySquare(occ, sq);
        return moves;
    };

    //sliding moves
    bitset<64> wBishopAttacks() { return BishopAttacks(wBishop); };
    bitset<64> bBishopAttacks() { return BishopAttacks(bBishop); };
    bitset<64> wBishopMoves() { bitset<64> wbatt = wBishopAttacks(); return wbatt & ~(wbatt & wBoard()); };
    bitset<64> bBishopMoves() { bitset<64> bbatt = bBishopAttacks(); return bbatt & ~(bbatt & bBoard()); };

    bitset<64> wRookAttacks() { return RookAttacks(wRook); };
    bitset<64> bRookAttacks() { return RookAttacks(bRook); };
    bitset<64> wRookMoves() { bitset<64> wratt = wRookAttacks(); return wratt & ~(wratt & wBoard()); };
    bitset<64> bRookMoves() { bitset<64> bratt = bRookAttacks(); return bratt & ~(bratt & bBoard()); };

    bitset<64> wQueenAttacks() { return RookAttacks(wQueen) | BishopAttacks(wQueen); };
    bitset<64> bQueenAttacks() { return RookAttacks(bQueen) | BishopAttacks(bQueen); };
    bitset<64> wQueenMoves() { bitset<64> wqatt = wQueenAttacks(); return wqatt & ~(wqatt & wBoard()); };
    bitset<64> bQueenMoves() { bitset<64> bqatt = bQueenAttacks(); return bqatt & ~(bqatt & bBoard()); };

    bitset<64> wMoves() { return wPawnMoves() | wKnightMoves() | wBishopMoves() | wRookMoves() | wQueenMoves() | wKingMoves(); };
    bitset<64> bMoves() { return bPawnMoves() | bKnightMoves() | bBishopMoves() | bRookMoves() | bQueenMoves() | bKingMoves(); };


    vector<string> wBishopMovesUCI() {
        vector<string> ucimoves;
        bitset<64> wbishop = wBishop;
        vector<int> indexes = BitSetTrueIndexes(wbishop);
        for(int sq : indexes) {
            bitset<64> singleBishop;
            singleBishop.set(sq);
            bitset<64> batt = BishopAttacks(singleBishop);
            bitset<64> bishopmoves = batt & ~(batt & wBoard());
            vector<string> currMoves = BitsetToUCI(IndexToSquare(sq), bishopmoves);
            ucimoves = Combine(ucimoves, currMoves);
        }
        return ucimoves;
    };

    vector<string> bBishopMovesUCI() {
        vector<string> ucimoves;
        bitset<64> bbishop = bBishop;
        vector<int> indexes = BitSetTrueIndexes(bbishop);
        for(int sq : indexes) {
            bitset<64> singleBishop;
            singleBishop.set(sq);
            bitset<64> batt = BishopAttacks(singleBishop);
            bitset<64> bishopmoves = batt & ~(batt & bBoard());
            vector<string> currMoves = BitsetToUCI(IndexToSquare(sq), bishopmoves);
            ucimoves = Combine(ucimoves, currMoves);
        }
        return ucimoves;
    };

    vector<string> wRookMovesUCI() {
        vector<string> ucimoves;
        bitset<64> wrook = wRook;
        vector<int> indexes = BitSetTrueIndexes(wrook);
        for(int sq : indexes) {
            bitset<64> singleRook;
            singleRook.set(sq);
            bitset<64> ratt = RookAttacks(singleRook);
            bitset<64> rookmoves = ratt & ~(ratt & wBoard());
            vector<string> currMoves = BitsetToUCI(IndexToSquare(sq), rookmoves);
            ucimoves = Combine(ucimoves, currMoves);
        }
        return ucimoves;
    };

    vector<string> bRookMovesUCI() {
        vector<string> ucimoves;
        bitset<64> brook = bRook;
        vector<int> indexes = BitSetTrueIndexes(brook);
        for(int sq : indexes) {
            bitset<64> singleRook;
            singleRook.set(sq);
            bitset<64> ratt = RookAttacks(singleRook);
            bitset<64> rookmoves = ratt & ~(ratt & bBoard());
            vector<string> currMoves = BitsetToUCI(IndexToSquare(sq), rookmoves);
            ucimoves = Combine(ucimoves, currMoves);
        }
        return ucimoves;
    };

    vector<string> wQueenMovesUCI() {
        vector<string> ucimoves;
        bitset<64> wqueen = wQueen;
        vector<int> indexes = BitSetTrueIndexes(wqueen);
        for(int sq : indexes) {
            bitset<64> singleQueen;
            singleQueen.set(sq);
            bitset<64> ratt = RookAttacks(singleQueen);
            bitset<64> batt = BishopAttacks(singleQueen);
            bitset<64> qatt = ratt | batt;
            bitset<64> queenMoves = qatt & ~(qatt & wBoard()); 
            vector<string> currMoves = BitsetToUCI(IndexToSquare(sq), queenMoves);
            ucimoves = Combine(ucimoves, currMoves);
        }
        return ucimoves;
    };

    vector<string> bQueenMovesUCI() {
        vector<string> ucimoves;
        bitset<64> bqueen = bQueen;
        vector<int> indexes = BitSetTrueIndexes(bqueen);
        for(int sq : indexes) {
            bitset<64> singleQueen;
            singleQueen.set(sq);
            bitset<64> ratt = RookAttacks(singleQueen);
            bitset<64> batt = BishopAttacks(singleQueen);
            bitset<64> qatt = ratt | batt;
            bitset<64> queenMoves = qatt & ~(qatt & bBoard()); 
            vector<string> currMoves = BitsetToUCI(IndexToSquare(sq), queenMoves);
            ucimoves = Combine(ucimoves, currMoves);
        }
        return ucimoves;
    };

    //attacks
    bitset<64> wAttacks() { return wPawnAllAtt() | wKingMoves() | wKnightMoves() | wQueenAttacks() | wBishopAttacks() | wRookAttacks(); };
    bitset<64> bAttacks() { return bPawnAllAtt() | bKingMoves() | bKnightMoves() | bQueenAttacks() | bBishopAttacks() | bRookAttacks(); };

    bitset<64> xRaywRookAttacks() {
        bitset<64> wrookmoves = wRookMoves();
        bitset<64> occ = AllBoard();
        bitset<64> blockers = wrookmoves & occ;
        blockers &= wrookmoves;
        bitset<64> occAttacks = RookAttacks(wRook, occ ^ blockers);
        bitset<64> xrayattcks = (wrookmoves ^ occAttacks) & bBoard();
        return xrayattcks;
    };

    bitset<64> xRaybRookAttacks() {
        bitset<64> brookmoves = bRookMoves();
        bitset<64> occ = AllBoard();
        bitset<64> blockers = brookmoves & occ;
        blockers &= brookmoves;
        bitset<64> occAttacks = RookAttacks(bRook, occ ^ blockers);
        bitset<64> xrayattcks = (brookmoves ^ occAttacks) & wBoard();
        return xrayattcks;
    };

    bitset<64> xRaywBishopAttacks() {
        bitset<64> wbishopmoves =  wBishopMoves();
        bitset<64> occ = AllBoard();
        bitset<64> blockers = wbishopmoves & occ;
        blockers &= wbishopmoves;
        bitset<64> occAttacks = BishopAttacks(wBishop, occ ^ blockers);
        bitset<64> xrayattcks = (wbishopmoves ^ occAttacks) & bBoard();
        return xrayattcks;
    };

    bitset<64> xRaybBishopAttacks() {
        bitset<64> bbishopmoves =  bBishopMoves();
        bitset<64> occ = AllBoard();
        bitset<64> blockers = bbishopmoves & occ;
        blockers &= bbishopmoves;
        bitset<64> occAttacks = BishopAttacks(bBishop, occ ^ blockers);
        bitset<64> xrayattcks = (bbishopmoves ^ occAttacks) & wBoard();
        return xrayattcks;
    };

    //UCI moves
    vector<string> wUciMoves() {
        vector<string> ucimoves = wPawnMovesUCI();
        ucimoves = Combine(ucimoves, wKnightMovesUCI());
        ucimoves = Combine(ucimoves, wKingMovesUCI());
        ucimoves = Combine(ucimoves, wBishopMovesUCI());
        ucimoves = Combine(ucimoves, wRookMovesUCI());
        ucimoves = Combine(ucimoves, wQueenMovesUCI());
        return ucimoves;
    };

    vector<string> bUciMoves() {
        vector<string> ucimoves = bPawnMovesUCI();
        ucimoves = Combine(ucimoves, bKnightMovesUCI());
        ucimoves = Combine(ucimoves, bKingMovesUCI());
        ucimoves = Combine(ucimoves, bBishopMovesUCI());
        ucimoves = Combine(ucimoves, bRookMovesUCI());
        ucimoves = Combine(ucimoves, bQueenMovesUCI());
        return ucimoves;
    };

   vector<string> GetUciMoves() {
        if(isWhiteMove) return wUciMoves();
        return bUciMoves();
    };
    

    //making moves helpers 
    bool isPawn(int sq) {
        if(isWhiteMove) return wPawn.test(sq);
        return bPawn.test(sq);
    };

    bool isPromotionSquare(int sq) {
        if(isWhiteMove) return (sq < 8);
        return (sq > 55);
    };

    void GetwBoardandResetIndex(int index) {
        if(wPawn.test(index)) wPawn.reset(index); 
        if(wKnight.test(index)) wKnight.reset(index); 
        if(wBishop.test(index)) wBishop.reset(index); 
        if(wRook.test(index)) wRook.reset(index); 
        if(wKing.test(index)) wKing.reset(index); 
        if(wQueen.test(index)) wQueen.reset(index); 
    };

    void GetbBoardandResetIndex(int index) {
        if(bPawn.test(index))  bPawn.reset(index); 
        if(bKnight.test(index))  bKnight.reset(index); 
        if(bBishop.test(index))  bBishop.reset(index); 
        if(bRook.test(index))  bRook.reset(index); 
        if(bKing.test(index))  bKing.reset(index); 
        if(bQueen.test(index))  bQueen.reset(index); 
    };

    void GetwBoardandSetIndex(int index) {
        if(wPawn.test(index)) wPawn.set(index); 
        if(wKnight.test(index)) wKnight.set(index); 
        if(wBishop.test(index)) wBishop.set(index); 
        if(wRook.test(index)) wRook.set(index); 
        if(wKing.test(index)) wKing.set(index); 
        if(wQueen.test(index)) wQueen.set(index); 
    };

    void GetbBoardandSetIndex(int index) {
        if(bPawn.test(index)) bPawn.set(index); 
        if(bKnight.test(index)) bKnight.set(index); 
        if(bBishop.test(index)) bBishop.set(index); 
        if(bRook.test(index)) bRook.set(index); 
        if(bKing.test(index)) bKing.set(index); 
        if(bQueen.test(index)) bQueen.set(index); 
    };

    void GetwBoardandSetPromoIndex(int index, char promoP) {
        switch(promoP){
            case 'q':
                wQueen.set(index);return;
            case 'r':
                wRook.set(index);return;
            case 'b':
                wBishop.set(index);return;
            case 'n':
                wKnight.set(index);return;
        } 
    };

    void GetbBoardandSetPromoIndex(int index, char promoP) {
        switch(promoP){
            case 'q':
                bQueen.set(index);return;
            case 'r':
                bRook.set(index);return;
            case 'b':
                bBishop.set(index);return;
            case 'n':
                bKnight.set(index);return;
        } 
    };

    void GetBoardandSetIndex(int index) {
        if(isWhiteMove)  {
            GetwBoardandSetIndex(index); 
        } else {
            GetbBoardandSetIndex(index); 
        }
    };

    void GetBoardandResetIndex(int index, bool isCapture) {
        if(isWhiteMove) {
            if(isCapture)  {
                GetbBoardandResetIndex(index); 
            } else {
                GetwBoardandResetIndex(index); 
            } 
        } else {
            if(isCapture) {
                GetbBoardandResetIndex(index); 
            } else {
                GetwBoardandResetIndex(index); 
            }
        }
    };

    void GetBoardandSetPromoIndex(int index, char promoP) {
        if(isWhiteMove)  {
            GetwBoardandSetPromoIndex(index, promoP); 
        } else {
            GetbBoardandSetPromoIndex(index, promoP); 
        }
    };

    void CaptureMoveUpdate(int startIndex, int targetIndex) {
        GetBoardandResetIndex(targetIndex, true);
        GetBoardandResetIndex(startIndex, false);
        GetBoardandSetIndex(targetIndex);
    };

    void QuietMoveUpdate(int startIndex, int targetIndex) {
        GetBoardandResetIndex(startIndex, false);
        GetBoardandSetIndex(targetIndex);
    };

    void CapturePromoUpdate(int startIndex, int targetIndex, char promoP) {
        GetBoardandResetIndex(targetIndex, true);
        GetBoardandResetIndex(startIndex, false);
        GetBoardandSetPromoIndex(targetIndex, promoP);
    };

    void QuietPromoUpdate(int startIndex, int targetIndex, char promoP) {
        GetBoardandResetIndex(startIndex, false);
        GetBoardandSetPromoIndex(targetIndex, promoP);
    };

    //making moves 
    bool MakeMove(string move) {
        vector<string> ucimoves = GetUciMoves();

        //if move is not in moves
        auto it = find(ucimoves.begin(), ucimoves.end(), move);
        if (it == ucimoves.end()) return false;
        
        //determine board and moves
        string startSquare = move.substr(0, 2);
        string targetSquare = move.substr(2, 2);
        int startIndex = StringtoIndex(startSquare);
        int targetIndex = StringtoIndex(targetSquare);

        //test if promotion
        bool isPromotion = move.length() == 5;
        bool isCaptureMove = AllBoard().test(targetIndex);
        if(isPromotion) {
            char piecePromotion = move[4];
            if(isCaptureMove) {
                CapturePromoUpdate(startIndex, targetIndex, piecePromotion);
            } else {
                QuietPromoUpdate(startIndex, targetIndex, piecePromotion);
            }
        }

        //test if castle

        //test if capture
        else if(isCaptureMove) { 
            CaptureMoveUpdate(startIndex, targetIndex); 
        } else {
            QuietMoveUpdate(startIndex, targetIndex);
        }

        //after move is made 
        isWhiteMove = !isWhiteMove;

        return true;
    };




    //misc
    void PrintAllBoards() {
        PrintBitSet(wPawn, "wPawn");
        PrintBitSet(bPawn, "bPawn");
        PrintBitSet(wKnight, "wKnight");
        PrintBitSet(bKnight, "bKnight");
        PrintBitSet(wBishop, "wBishop");
        PrintBitSet(bBishop, "bBishop");
        PrintBitSet(wRook, "wRook");
        PrintBitSet(bRook, "bRook");
        PrintBitSet(wQueen, "wQueen");
        PrintBitSet(bQueen, "bQueen");
        PrintBitSet(wKing, "wKing");
        PrintBitSet(bKing, "bKing");
    };
};