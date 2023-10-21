#include "..\Extensions\U64Extentions.h"
#include <ctype.h>
#include <stdio.h>
#include <unordered_map>
using namespace U64Extensions;

class U64Bitboard {
    private:
    U64 wPawn;
    U64 wKnight;
    U64 wBishop;
    U64 wRook;
    U64 wQueen;
    U64 wKing;

    U64 bPawn;
    U64 bKnight;
    U64 bBishop;
    U64 bRook;
    U64 bQueen;
    U64 bKing;

    U64 wAttacks;
    U64 bAttacks;

    int halfMoveClock;
    int fullTurnNum;
    int enPassantTarget;
    string currFen;
    bool isWhiteMove;
    map<char, bool> castlingRights;

    int materialValue;

    map<int, U64> blockerToPinnnedMoves;
    map<int, vector<int>> checkToBlockSquares;


    public:
    U64Bitboard() { LoadFen(startFen); };
    U64Bitboard(string fen) { LoadFen(fen); };
    U64Bitboard(const U64Bitboard& other) {
        // Copy the scalar members
        this->halfMoveClock = other.halfMoveClock;
        this->fullTurnNum = other.fullTurnNum;
        this->enPassantTarget = other.enPassantTarget;
        this->currFen = other.currFen;
        this->isWhiteMove = other.isWhiteMove;
        this->castlingRights = other.castlingRights;
        this->materialValue = other.materialValue;

        // Copy the map members
        this->blockerToPinnnedMoves = other.blockerToPinnnedMoves;
        this->checkToBlockSquares = other.checkToBlockSquares;

        // Copy the U64 members
        this->wPawn = other.wPawn;
        this->wKnight = other.wKnight;
        this->wBishop = other.wBishop;
        this->wRook = other.wRook;
        this->wQueen = other.wQueen;
        this->wKing = other.wKing;

        this->bPawn = other.bPawn;
        this->bKnight = other.bKnight;
        this->bBishop = other.bBishop;
        this->bRook = other.bRook;
        this->bQueen = other.bQueen;
        this->bKing = other.bKing;

        this->wAttacks = other.wAttacks;
        this->bAttacks = other.bAttacks;
    };
    
    void LoadFenHelper(vector<string> arguments) {
        ClearBoard();
        currFen = arguments[0];
        string moveColor = arguments[1];
        isWhiteMove = moveColor == "w";
        castlingRights = SetCastlingRights(arguments[2]);
        enPassantTarget = (arguments[3] == "-") ? 0 : StringtoIndex(arguments[3]);
        fullTurnNum = stoi(arguments[4]);
        halfMoveClock = stoi(arguments[5]);
    };

    void SetBoard(char c, int sq) {
        switch(c) {
            case 'p': SetBit(bPawn, sq); AddMaterialValue(c); return;
            case 'b': SetBit(bBishop, sq); AddMaterialValue(c); return;
            case 'n': SetBit(bKnight, sq); AddMaterialValue(c); return;
            case 'r': SetBit(bRook, sq); AddMaterialValue(c); return;
            case 'q': SetBit(bQueen, sq); AddMaterialValue(c); return;
            case 'k': SetBit(bKing, sq); AddMaterialValue(c); return;
            case 'P': SetBit(wPawn, sq); AddMaterialValue(c); return;
            case 'B': SetBit(wBishop, sq); AddMaterialValue(c); return;
            case 'N': SetBit(wKnight, sq); AddMaterialValue(c); return;
            case 'R': SetBit(wRook, sq); AddMaterialValue(c); return;
            case 'Q': SetBit(wQueen, sq); AddMaterialValue(c); return;
            case 'K': SetBit(wKing, sq); AddMaterialValue(c); return;
        }
    }

    void AddMaterialValue(char c) {
        switch(c) {
            case 'p': materialValue -= 100; return;
            case 'b': materialValue -= 325;return;
            case 'n': materialValue -= 300;return;
            case 'r': materialValue -= 500;return;
            case 'q': materialValue -= 900;return;
            case 'k': materialValue -= 100000;return;
            case 'P': materialValue += 100;return;
            case 'B': materialValue += 325;return;
            case 'N': materialValue += 300;return;
            case 'R': materialValue += 500;return;
            case 'Q': materialValue += 900;return;
            case 'K': materialValue += 100000;return;
        }
    }

    void RemoveMaterialValue(char c) {
        switch(c) {
            case 'p': materialValue += 100; return;
            case 'b': materialValue += 325;return;
            case 'n': materialValue += 300;return;
            case 'r': materialValue += 500;return;
            case 'q': materialValue += 900;return;
            case 'k': materialValue += 100000;return;
            case 'P': materialValue -= 100;return;
            case 'B': materialValue -= 325;return;
            case 'N': materialValue -= 300;return;
            case 'R': materialValue -= 500;return;
            case 'Q': materialValue -= 900;return;
            case 'K': materialValue -= 100000;return;
        }
    }

    void LoadFen(string fen) {
        materialValue = 0;
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

        SetMoveData();
    };

    string GetFenHelper() {
        U64 allb = AllBoard();
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

            if(TestBit(allb, i)) {
                if(temp != 0) {
                    fen += to_string(temp);
                }
                fen += GetPieceAtIndex(i);
                temp=0;
            } else {
                temp++;
            }
        }
        return fen;
    }

    string GetFen() {
        string moveColor = isWhiteMove ? " w" : " b";
        string fen = GetFenHelper() + moveColor +" "+ CastlingRightsString(castlingRights) +" "+ EnpassantTargetToString(enPassantTarget) +" "+ to_string(halfMoveClock) +" "+ to_string(fullTurnNum);
        return fen;
    };

    string GetPieceAtIndex(int index) {
        if(TestBit(wPawn, index)) return "P";
        if(TestBit(wKnight, index)) return "N";
        if(TestBit(wBishop, index)) return "B";
        if(TestBit(wRook, index)) return "R";
        if(TestBit(wQueen, index)) return "Q";
        if(TestBit(wKing, index)) return "K";

        if(TestBit(bPawn, index)) return "p";
        if(TestBit(bKnight, index)) return "n";
        if(TestBit(bBishop, index)) return "b";
        if(TestBit(bRook, index)) return "r";
        if(TestBit(bQueen, index)) return "q";
        if(TestBit(bKing, index)) return "k";
    };

    int GetValueAtIndex(int index) {
        if(TestBit(wPawn, index)) return 1;
        if(TestBit(wKnight, index)) return 3;
        if(TestBit(wBishop, index)) return 3;
        if(TestBit(wRook, index)) return 5;
        if(TestBit(wQueen, index)) return 9;
        if(TestBit(wKing, index)) return 1000;

        if(TestBit(bPawn, index)) return -1;
        if(TestBit(bKnight, index)) return -3;
        if(TestBit(bBishop, index)) return -3;
        if(TestBit(bRook, index)) return -5;
        if(TestBit(bQueen, index)) return -9;
        if(TestBit(bKing, index)) return -1000;
        return 0;
    };

    int GetMaterialValueOld() {
        int sum = 0;
        for (int i = 0; i<64; i++) {
            sum += GetValueAtIndex(i);
        };
        return sum;
    };

    int GetMaterialValue() { return materialValue; };
    string GetCastlingRights() { return CastlingRightsString(castlingRights); };

    void ClearBoard() {
        Reset(wPawn); Reset(wKnight); Reset(wBishop); Reset(wRook); Reset(wQueen); Reset(wKing);
        Reset(bPawn); Reset(bKnight); Reset(bBishop); Reset(bRook); Reset(bQueen); Reset(bKing);

        ClearAttacks();
    };

    U64 GetwPawn() { return wPawn; };
    U64 GetwKnight() { return wKnight; };
    U64 GetwBishop() { return wBishop; };
    U64 GetwRook() { return wRook; };
    U64 GetwQueen() { return wQueen; };
    U64 GetwKing() { return wKing; };
    U64 GetbPawn() { return bPawn; };
    U64 GetbKnight() { return bKnight; };
    U64 GetbBishop() { return bBishop; };
    U64 GetbRook() { return bRook; };
    U64 GetbQueen() { return bQueen; };
    U64 GetbKing() { return bKing; };
    U64 GetwAttacks() { return wAttacks; };
    U64 GetbAttacks() { return bAttacks; };

    bool IsWhiteMove() { return isWhiteMove; };
    int GetMoveMultiplier() { return (isWhiteMove) ? 1 : -1; };

    U64 GetPawn() { return (isWhiteMove) ? wPawn : bPawn; };
    U64 GetKnight() { return (isWhiteMove) ? wKnight : bKnight; };
    U64 GetBishop() { return (isWhiteMove) ? wBishop : bBishop; };
    U64 GetRook() { return (isWhiteMove) ? wRook : bRook; };
    U64 GetQueen() { return (isWhiteMove) ? wQueen : bQueen; };
    U64 GetKing() { return (isWhiteMove) ? wKing : bKing; };
    U64 GetAttacks() { return (isWhiteMove) ? bAttacks : wAttacks; };

    //board functions
    U64 wBoard() { return wPawn | wKnight | wBishop | wRook | wQueen | wKing; };
    U64 bBoard() { return bPawn | bKnight | bBishop | bRook | bQueen | bKing; };
    U64 AllBoard() { return wBoard() | bBoard(); };
    U64 AllKing() { return wKing | bKing; };

    U64 EmptyBoard() { return ~AllBoard(); };
    U64 NotbBoard() { return ~bBoard(); };
    U64 NotwBoard() { return ~wBoard(); };

    U64 GetOpponentBoard() {return isWhiteMove ? bBoard() : wBoard(); };

    //generic movement 
    U64 OneInAllDirection(U64 b) { return (northOne(b) | southOne(b) | eastOne(b) | westOne(b) | northEastOne(b) | northWestOne(b) | southEastOne(b) | southWestOne(b)); };
    U64 northOne(U64 b) { return b >> 8; };
    U64 southOne(U64 b) { return b << 8; };
    U64 eastOne(U64 b) { return b << 1 & notAFile; };
    U64 westOne(U64 b) { return b >> 1 & notHFile; };
    U64 northEastOne(U64 b) { return b >> 7 & notAFile; };
    U64 northWestOne(U64 b) { return b >> 9 & notHFile; };
    U64 southEastOne(U64 b) { return b << 9 & notAFile; };
    U64 southWestOne(U64 b) { return b << 7 & notHFile; };
    bool InBounds(int offset) { return (offset >= 0) && (offset < 64); };

    //pawn functions
    U64 wSinglePushTargets(U64 b) { return northOne(b) & EmptyBoard(); };
    U64 wDoublePushTargets(U64 b) { return northOne(wSinglePushTargets(b)) & EmptyBoard() & rank4; };
    U64 bSinglePushTargets(U64 b) { return southOne(b) & EmptyBoard(); };
    U64 bDoublePushTargets(U64 b) { return southOne(bSinglePushTargets(b)) & EmptyBoard() & rank5; };
    U64 wPawnPushes() { return wSinglePushTargets(wPawn) | wDoublePushTargets(wPawn); };
    U64 bPawnPushes() { return bSinglePushTargets(bPawn) | bDoublePushTargets(bPawn); };

    U64 wPawnPushes(U64 b) { return wSinglePushTargets(b) | wDoublePushTargets(b); };
    U64 bPawnPushes(U64 b) { return bSinglePushTargets(b) | bDoublePushTargets(b); };

    U64 wPawnWestAtt() { return (wPawn >> 9) & notHFile; };
    U64 wPawnEastAtt() { return (wPawn >> 7) & notAFile; };
    U64 wPawnAllAtt() { return wPawnEastAtt() | wPawnWestAtt(); };
    U64 bPawnWestAtt() { return (bPawn << 9) & notAFile; };
    U64 bPawnEastAtt() { return (bPawn << 7) & notHFile; };
    U64 bPawnAllAtt() { return bPawnEastAtt() | bPawnWestAtt(); };
    U64 wPawnWestCaptures() { 
        U64 wpatt = wPawnWestAtt();
        U64 enpass = SingleBitBoard(enPassantTarget);
        U64 westcaptures = (wpatt & bBoard()) | (wpatt & enpass);
        return westcaptures;
    };
    U64 wPawnEastCaptures() { 
        U64 wpatt = wPawnEastAtt();
        U64 enpass = SingleBitBoard(enPassantTarget);
        U64 eastcaptures = (wpatt & bBoard()) | (wpatt & enpass);
        return eastcaptures;
    };
    U64 wPawnAllCaptures() { return wPawnEastCaptures() | wPawnWestCaptures(); };
    U64 bPawnWestCaptures() { 
        U64 bpatt = bPawnWestAtt();
        U64 enpass = SingleBitBoard(enPassantTarget);
        U64 westcaptures = (bpatt & wBoard()) | (bpatt & enpass);
        return westcaptures;
    };
    U64 bPawnEastCaptures() { 
        U64 bpatt = bPawnEastAtt();
        U64 enpass = SingleBitBoard(enPassantTarget);
        U64 eastcaptures = (bpatt & wBoard()) | (bpatt & enpass);
        return eastcaptures;
    };
    U64 bPawnAllCaptures() { return bPawnEastCaptures() | bPawnWestCaptures(); };

    U64 wPawnPsuedoMoves() { return wPawnAllCaptures() | wPawnPushes(); };
    U64 bPawnPsuedoMoves() { return bPawnAllCaptures() | bPawnPushes(); };

    U64 wPawnWestAtt(U64 b) { return (b >> 9) & notHFile; };
    U64 wPawnEastAtt(U64 b) { return (b >> 7) & notAFile; };
    U64 wPawnAllAtt(U64 b) { return wPawnEastAtt(b) | wPawnWestAtt(b); };
    U64 bPawnWestAtt(U64 b) { return (b << 9) & notAFile; };
    U64 bPawnEastAtt(U64 b) { return (b << 7) & notHFile; };
    U64 bPawnAllAtt(U64 b) { return bPawnEastAtt(b) | bPawnWestAtt(b); };
    U64 wPawnWestCaptures(U64 b) { return wPawnWestAtt(b) & (bBoard() | SingleBitBoard(enPassantTarget)); };
    U64 wPawnEastCaptures(U64 b) { return wPawnEastAtt(b) & (bBoard() | SingleBitBoard(enPassantTarget)); };
    U64 wPawnAllCaptures(U64 b) { return wPawnEastCaptures(b) | wPawnWestCaptures(b); };
    U64 bPawnWestCaptures(U64 b) { return bPawnWestAtt(b) & (wBoard() | SingleBitBoard(enPassantTarget)); };
    U64 bPawnEastCaptures(U64 b) { return bPawnEastAtt(b) & (wBoard() | SingleBitBoard(enPassantTarget)); };
    U64 bPawnAllCaptures(U64 b) { return bPawnEastCaptures(b) | bPawnWestCaptures(b); };

    U64 wPawnPsuedoMoves(U64 b) { return wPawnAllCaptures(b) | wPawnPushes(b); };
    U64 bPawnPsuedoMoves(U64 b) { return bPawnAllCaptures(b) | bPawnPushes(b); };

    //knight functions
    U64 KnightAttacks(U64 b) { return noNoEa(b) | noEaEa(b) | soEaEa(b) | soSoEa(b) | noNoWe(b) | noWeWe(b) | soWeWe(b) | soSoWe(b); };
    U64 noNoEa(U64 b) {return (b >> 15) & notAFile; };
    U64 noEaEa(U64 b) {return (b >> 6) & (notAFile & notBFile);};
    U64 soEaEa(U64 b) {return (b << 10) & (notAFile & notBFile);};
    U64 soSoEa(U64 b) {return (b << 17) & notAFile; };
    U64 soSoWe(U64 b) {return (b << 15) & notHFile; };
    U64 soWeWe(U64 b) {return (b << 6) & (notGFile & notHFile); };
    U64 noWeWe(U64 b) {return (b >> 10) & (notGFile & notHFile); };
    U64 noNoWe(U64 b) {return (b >> 17) & notHFile; };

    U64 wKnightAtt(){ return KnightAttacks(wKnight); };
    U64 bKnightAtt(){ return KnightAttacks(bKnight); };

    U64 wKnightPsuedoMoves(){ return KnightAttacks(wKnight) & NotwBoard(); };
    U64 bKnightPsuedoMoves(){ return KnightAttacks(bKnight) & NotbBoard(); };


    //king moves, castling needs to include possible castle through check 
    U64 wKingCastleShort() {
        if(checkToBlockSquares.size() > 0) return C64(0);
        bool canCastle = GetCastlingRightsValueByChar(castlingRights, 'K');
        if(!canCastle) return C64(0);
        U64 allb = AllBoard();
        bool hasBlocker = TestBit(allb, 61) || TestBit(allb, 62);
        if(hasBlocker) return C64(0);
        bool isAttacked = isSquareAttacked(61) || isSquareAttacked(62);
        if(isAttacked) return C64(0);
        return SingleBitBoard(62);
    };

    U64 wKingCastleLong() {
        if(checkToBlockSquares.size() > 0) return C64(0);
        bool canCastle = GetCastlingRightsValueByChar(castlingRights, 'Q');
        if(!canCastle) return C64(0);
        U64 allb = AllBoard();
        bool hasBlocker = TestBit(allb, 57) || TestBit(allb, 58) || TestBit(allb, 59);
        if(hasBlocker) return C64(0);
        bool isAttacked = isSquareAttacked(57) || isSquareAttacked(58)|| isSquareAttacked(59);
        if(isAttacked) return C64(0);
        return SingleBitBoard(58);
    };

    U64 wKingCastle() { return wKingCastleShort() | wKingCastleLong(); };

    U64 bKingCastleShort() {
        if(checkToBlockSquares.size() > 0) return C64(0);
        bool canCastle = GetCastlingRightsValueByChar(castlingRights, 'k');
        if(!canCastle) return C64(0);
        U64 allb = AllBoard();
        bool hasBlocker = TestBit(allb, 5) || TestBit(allb, 6);
        if(hasBlocker) return C64(0);
        bool isAttacked = isSquareAttacked(5) || isSquareAttacked(6);
        if(isAttacked) return C64(0);
        return SingleBitBoard(6);
    };

    U64 bKingCastleLong() {
        if(checkToBlockSquares.size() > 0) return C64(0);
        bool canCastle = GetCastlingRightsValueByChar(castlingRights, 'q');
        if(!canCastle) return C64(0);
        U64 allb = AllBoard();
        bool hasBlocker = TestBit(allb, 1) || TestBit(allb, 2) || TestBit(allb, 3);
        if(hasBlocker) return C64(0);
        bool isAttacked = isSquareAttacked(1) || isSquareAttacked(2) || isSquareAttacked(3);
        if(isAttacked) return C64(0);
        return SingleBitBoard(2);
    };

    U64 bKingCastle() { return bKingCastleShort() | bKingCastleLong(); };

    U64 wKingPsuedoMoves() { return (OneInAllDirection(wKing) | wKingCastle() ) & (NotwBoard() & ~bAttacks); };
    U64 bKingPsuedoMoves() { return (OneInAllDirection(bKing) | bKingCastle() ) & (NotbBoard() & ~wAttacks); };

    U64 wKingAtt() { return OneInAllDirection(wKing); };
    U64 bKingAtt() { return OneInAllDirection(bKing); };

    //sliding moves helper
    bool ValidTravel(U64 overlap, int offset) { 
        U64 notallb = ~AllBoard();
        return InBounds(offset) && TestBit(overlap, offset) && TestBit(notallb, offset); 
    };
    bool ValidTravelAtt(U64 overlap, int offset) { 
        U64 allb = AllBoard();
        return InBounds(offset) && TestBit(overlap, offset) && TestBit(allb, offset); 
    };

    bool ValidTravel(U64 occ, U64 overlap, int offset) { 
        U64 notocc = ~occ;
        return InBounds(offset) && TestBit(overlap, offset) && TestBit(notocc, offset); 
    };
    bool ValidTravelAtt(U64 occ, U64 overlap, int offset) { return InBounds(offset) && TestBit(overlap, offset) && TestBit(occ, offset); };

    U64 SlidingAttacks(U64 overlap, int sq, int direction) {
        U64 moves = 0;
        int offset = sq + direction;
        while(ValidTravel(overlap, offset)) {
            SetBit(moves, offset);
            offset += direction;
        }
        if(ValidTravelAtt(overlap, offset)) {
            SetBit(moves, offset);
        }
        return moves;
    };

    U64 SlidingAttacks(U64 occ, U64 overlap, int sq, int direction) {
        U64 moves = 0;
        int offset = sq + direction;
        while(ValidTravel(occ, overlap, offset)) {
            SetBit(moves, offset);
            offset += direction;
        }
        if(ValidTravelAtt(occ, overlap, offset)) {
            SetBit(moves, offset);
        }
        return moves;
    };

    U64 BishopAttacksBySquare(int sq) {
        U64 moves = 0;
        //travel northEastOne
        moves |= SlidingAttacks(notAFile, sq, -7);
        //travel northWestOne
        moves |= SlidingAttacks(notHFile, sq, -9);
        //travel southEastOne
        moves |= SlidingAttacks(notAFile, sq, 9);
        //travle southWestOne
        moves |= SlidingAttacks(notHFile, sq, 7);
        return moves;
    };

    U64 BishopAttacksBySquare(U64 occ, int sq) {
        U64 moves = 0;
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

    U64 BishopAttacks(U64 bishopBoard) {
        U64 moves = 0;
        vector<int> indexes = GetTrueBits(bishopBoard);
        for(int sq : indexes) moves |= BishopAttacksBySquare(sq); 
        return moves;
    };

    U64 BishopAttacks(U64 bishopBoard, U64 occ) {
        U64 moves = 0;
        vector<int> indexes = GetTrueBits(bishopBoard);
        for(int sq : indexes) moves |= BishopAttacksBySquare(occ, sq); 
        return moves;
    };

    U64 RookAttacksBySquare(int sq) {
        U64 moves = 0;
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

    U64 RookAttacksBySquare(U64 occ, int sq) {
        //travel northOne
        U64 northmoves = SlidingAttacks(occ, ~rank1, sq, -8);
        //travel eastOne
        U64 eastmoves  = SlidingAttacks(occ, ~aFile, sq, 1);
        //travel westOne
        U64 westmoves  = SlidingAttacks(occ, ~hFile, sq, -1);
        //travle southOne
        U64 southmoves  = SlidingAttacks(occ, ~rank8, sq, 8);
        return northmoves | eastmoves | westmoves | southmoves;
    };

    U64 RookAttacks(U64 rBoard) {
        U64 moves = 0;
        vector<int> indexes = GetTrueBits(rBoard);
        for(int sq : indexes) moves |= RookAttacksBySquare(sq);
        return moves;
    };

    U64 RookAttacks(U64 rBoard, U64 occ) {
        U64 moves = 0;
        vector<int> indexes = GetTrueBits(rBoard);
        for(int sq : indexes) moves |= RookAttacksBySquare(occ, sq);
        return moves;
    };

    U64 SlidingAttacksByDirectionAndSquare(int sq, U64 occ, int direction) { 
        switch (direction) {
            case -7: return SlidingAttacks(occ, ~aFile, sq, direction);
            case -9: return SlidingAttacks(occ, ~hFile, sq, direction);
            case 9: return SlidingAttacks(occ, ~aFile, sq, direction);
            case 7: return SlidingAttacks(occ, ~hFile, sq, direction);

            case -8: return SlidingAttacks(occ, ~rank1, sq, direction);
            case 1: return SlidingAttacks(occ, ~aFile, sq, direction);
            case -1: return SlidingAttacks(occ, ~hFile, sq, direction);
            case 8: return SlidingAttacks(occ, ~rank8, sq, direction);
            default: return C64(0);
        }
    };

    U64 QueenAttacks(U64 qBoard) {
        U64 moves = 0;
        vector<int> indexes = GetTrueBits(qBoard);
        for(int sq : indexes) {
            moves |= RookAttacksBySquare(sq);
            moves |= BishopAttacksBySquare(sq);
        }
        return moves;
    };

    U64 QueenAttacks(U64 qBoard, U64 occ) {
        U64 moves = 0;
        vector<int> indexes = GetTrueBits(qBoard);
        for(int sq : indexes) {
            moves |= RookAttacksBySquare(qBoard, sq);
            moves |= BishopAttacksBySquare(qBoard, sq);
        }
        return moves;
    };

    //sliding moves
    U64 wBishopAttacks() { return BishopAttacks(wBishop); };
    U64 bBishopAttacks() { return BishopAttacks(bBishop); };
    U64 wBishopPsuedoMoves() { U64 wbatt = wBishopAttacks(); return wbatt & ~(wbatt & wBoard()); };
    U64 bBishopPsuedoMoves() { U64 bbatt = bBishopAttacks(); return bbatt & ~(bbatt & bBoard()); };

    U64 wRookAttacks() { return RookAttacks(wRook); };
    U64 bRookAttacks() { return RookAttacks(bRook); };
    U64 wRookPsuedoMoves() { U64 wratt = wRookAttacks(); return wratt & ~(wratt & wBoard()); };
    U64 bRookPsuedoMoves() { U64 bratt = bRookAttacks(); return bratt & ~(bratt & bBoard()); };

    U64 wQueenAttacks() { return RookAttacks(wQueen) | BishopAttacks(wQueen); };
    U64 bQueenAttacks() { return RookAttacks(bQueen) | BishopAttacks(bQueen); };
    U64 wQueenPsuedoMoves() { U64 wqatt = wQueenAttacks(); return wqatt & ~(wqatt & wBoard()); };
    U64 bQueenPsuedoMoves() { U64 bqatt = bQueenAttacks(); return bqatt & ~(bqatt & bBoard()); };

    //attacks
    void SetwPawnAttacks() {
        vector<int> indexes = GetTrueBits(wPawn);
        for(int sq : indexes) {
            U64 pawn = SingleBitBoard(sq);
            U64 pawnAttacks = wPawnAllAtt(pawn);
            U64 givesCheck = pawnAttacks & bKing;

            if(givesCheck != C64(0)) InsertCheck(checkToBlockSquares, sq, {sq});

            wAttacks |= pawnAttacks;
        }
    };

    void SetbPawnAttacks() {
        vector<int> indexes = GetTrueBits(bPawn);
        for(int sq : indexes) {
            U64 pawn = SingleBitBoard(sq);
            U64 pawnAttacks = bPawnAllAtt(pawn);
            U64 givesCheck = pawnAttacks & wKing;

            if(givesCheck != C64(0)) InsertCheck(checkToBlockSquares, sq, {sq});

            bAttacks |= pawnAttacks;
        }
    };

    void SetwKnightAttacks() {
        vector<int> indexes = GetTrueBits(wKnight);
        for(int sq : indexes) {
            U64 knight = SingleBitBoard(sq);
            U64 knightAtt = KnightAttacks(knight);
            U64 givesCheck = knightAtt & bKing;

            if(givesCheck != C64(0)) InsertCheck(checkToBlockSquares, sq, {sq});

            wAttacks |= knightAtt;
        }
    };

    void SetbKnightAttacks() {
        vector<int> indexes = GetTrueBits(bKnight);
        for(int sq : indexes) {
            U64 knight = SingleBitBoard(sq);
            U64 knightAtt = KnightAttacks(knight);
            U64 givesCheck = knightAtt & wKing;

            if(givesCheck != C64(0)) InsertCheck(checkToBlockSquares, sq, {sq});

            bAttacks |= knightAtt;
        }
    };

    void SetwKingAttacks() { int sq = GetTrueBits(wKing)[0]; U64 king = SingleBitBoard(sq); U64 kingatt = OneInAllDirection(king); wAttacks |= kingatt; };
    void SetbKingAttacks() { int sq = GetTrueBits(bKing)[0]; U64 king = SingleBitBoard(sq); U64 kingatt = OneInAllDirection(king); bAttacks |= kingatt; };

    void SetxRaySlidingAttacks(U64 moves, U64 occ, U64 pieceBoard, U64 colorBoard, U64 oppBoard, int direction, int sq, U64 &attacks) {
        U64 blocker = moves & occ & ~oppBoard;
        if(blocker == C64(0)) return;

        U64 king = GetKing();
        if(blocker == king) {
            U64 kingSq = GetTrueBits(king)[0];
            U64 blocks = (moves ^ blocker) ^ pieceBoard; 
            vector<int> blockAndCap = GetTrueBits(blocks);
            InsertCheck(checkToBlockSquares, sq, blockAndCap);

            //add attacker thorugh king 
            if(TestBit(occ, kingSq + direction)) return;
            SetBit(attacks, kingSq + direction);
        }
        int blockerSq = GetTrueBits(blocker)[0];
        U64 occAttacks = SlidingAttacksByDirectionAndSquare(sq, occ ^ blocker, direction);
        U64 movesXORoccatt = moves ^ occAttacks;
        U64 xrayattcks = (movesXORoccatt) & colorBoard;
        if(xrayattcks == C64(0)) return;

        if(xrayattcks == king) {
            U64 pinnedMoves = (occAttacks ^ xrayattcks) | pieceBoard;
            InsertPin(blockerToPinnnedMoves, blockerSq, pinnedMoves);
        }
    };

    U64 SetxRayRookAttacks(U64 rook, U64 colorBoard, U64 oppBoard) {
        U64 attacks = 0;
        U64 occ = AllBoard();
        vector<int> indexes = GetTrueBits(rook);
        for(int sq : indexes) {
            U64 rookSq = SingleBitBoard(sq);
            //travle north
            U64 northmoves = SlidingAttacks(occ, ~rank1, sq, -8);
            SetxRaySlidingAttacks(northmoves, occ, rookSq, colorBoard, oppBoard, -8, sq, attacks);
            attacks |= northmoves;

            //travel east
            U64 eastmoves  = SlidingAttacks(occ, ~aFile, sq, 1);
            SetxRaySlidingAttacks(eastmoves, occ, rookSq, colorBoard, oppBoard, 1, sq, attacks);
            attacks |= eastmoves;

            //travel west
            U64 westmoves  = SlidingAttacks(occ, ~hFile, sq, -1);
            SetxRaySlidingAttacks(westmoves, occ, rookSq, colorBoard, oppBoard, -1, sq, attacks);
            attacks |= westmoves;

            //travle south
            U64 southmoves  = SlidingAttacks(occ, ~rank8, sq, 8);
            SetxRaySlidingAttacks(southmoves, occ, rookSq, colorBoard, oppBoard, 8, sq, attacks);
            attacks |= southmoves;;
        }
        return attacks;
    };

    U64 SetxRayBishopAttacks(U64 bishop, U64 colorBoard, U64 oppBoard) {
        U64 attacks = 0;
        U64 occ = AllBoard();
        vector<int> indexes = GetTrueBits(bishop);
        for(int sq : indexes) {
            U64 bishopSq = SingleBitBoard(sq);
            //travel northEast
            U64 northeast = SlidingAttacks(occ, ~aFile, sq, -7);
            attacks |= northeast;
            SetxRaySlidingAttacks(northeast, occ, bishopSq, colorBoard, oppBoard, -7, sq, attacks);

            //travel northWest
            U64 northwest = SlidingAttacks(occ, ~hFile, sq, -9);
            attacks |= northwest;
            SetxRaySlidingAttacks(northwest, occ, bishopSq, colorBoard, oppBoard, -9, sq, attacks);

            //travel southEast
            U64 southeast = SlidingAttacks(occ, ~aFile, sq, 9);
            SetxRaySlidingAttacks(southeast, occ, bishopSq, colorBoard, oppBoard, 9, sq, attacks);
            attacks |= southeast;

            //travle southWest
            U64 southwest = SlidingAttacks(occ, ~hFile, sq, 7);
            SetxRaySlidingAttacks(southwest, occ, bishopSq, colorBoard, oppBoard, 7, sq, attacks);
            attacks |= southwest;
        }
        return attacks;
    };
    
    void SetxRaywRookAttacks() { U64 attacks = SetxRayRookAttacks(wRook, bBoard(), wBoard()); wAttacks |= attacks; };
    void SetxRaybRookAttacks() { U64 attacks = SetxRayRookAttacks(bRook, wBoard(), bBoard()); bAttacks |= attacks; };
    void SetxRaywBishopAttacks() { U64 attacks = SetxRayBishopAttacks(wBishop, bBoard(), wBoard()); wAttacks |= attacks; };
    void SetxRaybBishopAttacks() { U64 attacks = SetxRayBishopAttacks(bBishop, wBoard(), bBoard()); bAttacks |= attacks; };
    void SetxRaywQueenAttacks() { U64 batt = SetxRayBishopAttacks(wQueen, bBoard(), wBoard()); U64 ratt = SetxRayRookAttacks(wQueen, bBoard(), wBoard()); wAttacks |= ratt; wAttacks |= batt; };
    void SetxRaybQueenAttacks() { U64 batt = SetxRayBishopAttacks(bQueen, wBoard(), bBoard()); U64 ratt = SetxRayRookAttacks(bQueen, wBoard(), bBoard()); bAttacks |= ratt; bAttacks |= batt; }; 

    void SetwSlidingAttacks() { SetxRaywRookAttacks(); SetxRaywBishopAttacks(); SetxRaywQueenAttacks(); };
    void SetbSlidingAttacks() { SetxRaybRookAttacks(); SetxRaybBishopAttacks(); SetxRaybQueenAttacks(); };

    void SetwStandardAttacks() { SetwPawnAttacks(); SetwKnightAttacks(); SetwKingAttacks(); };
    void SetbStandardAttacks() { SetbPawnAttacks(); SetbKnightAttacks(); SetbKingAttacks(); };

    void SetwAttacks() { SetwSlidingAttacks(); SetwStandardAttacks(); };
    void SetbAttacks() { SetbSlidingAttacks(); SetbStandardAttacks(); };

    void SetAttacks() { return isWhiteMove ? SetbAttacks() : SetwAttacks(); };
    void ClearAttacks() { Reset(wAttacks); Reset(bAttacks); blockerToPinnnedMoves.clear(); checkToBlockSquares.clear(); }


    //making moves helpers 
    bool isPawn(int sq) { return isWhiteMove ? TestBit(wPawn, sq): TestBit(bPawn, sq); };
    bool isSquareAttacked(int sq) { return isWhiteMove ? TestBit(bAttacks, sq) : TestBit(wAttacks, sq); };


    //Psuedomoves
    U64 wPsuedoMoves() { return wPawnPsuedoMoves() | wKnightPsuedoMoves() | wBishopPsuedoMoves() | wRookPsuedoMoves() | wQueenPsuedoMoves() | wKingPsuedoMoves(); };
    U64 bPsuedoMoves() { return bPawnPsuedoMoves() | bKnightPsuedoMoves() | bBishopPsuedoMoves() | bRookPsuedoMoves() | bQueenPsuedoMoves() | bKingPsuedoMoves(); };

    U64 GetPawnPsuedoMoves() { return isWhiteMove ? wPawnPsuedoMoves(): bPawnPsuedoMoves(); };
    U64 GetKnightPsuedoMoves() { return isWhiteMove ? wKnightPsuedoMoves(): bKnightPsuedoMoves(); };
    U64 GetBishopPsuedoMoves() { return isWhiteMove ? wBishopPsuedoMoves(): bBishopPsuedoMoves(); };
    U64 GetRookPsuedoMoves() { return isWhiteMove ? wRookPsuedoMoves(): bRookPsuedoMoves(); };
    U64 GetQueenPsuedoMoves() { return isWhiteMove ? wQueenPsuedoMoves(): bQueenPsuedoMoves(); };
    U64 GetKingPsuedoMoves() { return isWhiteMove ? wKingPsuedoMoves(): bKingPsuedoMoves(); };

    U64 GetPsuedoMoves() { return isWhiteMove ? wPsuedoMoves() : bPsuedoMoves(); };

    //vector<int> moves
    void GetwPawnMapMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(wPawn);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 pawnMoves = wPawnPsuedoMoves(temp);
            U64 pinned = GetPinnedMoves(blockerToPinnnedMoves, sq);
            U64 legal = pinned & pawnMoves;
            U64ToMapMoves(moves, sq, legal, true, true);
        }
    };
    multimap<int, pair<int, char>> GetwPawnMapMoves() { multimap<int, pair<int, char>> moves; GetwPawnMapMoves(moves); return moves; };

    void GetbPawnMapMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(bPawn);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 pawnMoves = bPawnPsuedoMoves(temp);
            U64 pinned = GetPinnedMoves(blockerToPinnnedMoves, sq);
            U64 legal = pinned & pawnMoves;
            U64ToMapMoves(moves, sq, legal, true, false);
        }
    };
    multimap<int, pair<int, char>> GetbPawnMapMoves() { multimap<int, pair<int, char>> moves; GetbPawnMapMoves(moves); return moves; };

    void GetwKnightMapMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(wKnight);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 watt = KnightAttacks(temp);
            U64 knightMoves = watt & NotwBoard();
            U64 pinned = GetPinnedMoves(blockerToPinnnedMoves, sq);
            U64 legal = pinned & knightMoves;
            U64ToMapMoves(moves, sq, legal);
        }
    };
    multimap<int, pair<int, char>> GetwKnightMapMoves() { multimap<int, pair<int, char>> moves; GetwKnightMapMoves(moves); return moves; };

    void GetbKnightMapMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(bKnight);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 batt = KnightAttacks(temp);
            U64 knightMoves = batt & NotbBoard();
            U64 pinned = GetPinnedMoves(blockerToPinnnedMoves, sq);
            U64 legal = pinned & knightMoves;
            U64ToMapMoves(moves, sq, legal);
        }
    };
    multimap<int, pair<int, char>> GetbKnightMapMoves() { multimap<int, pair<int, char>> moves; GetbKnightMapMoves(moves); return moves; };

    void GetwBishopMapMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(wBishop);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 watt = BishopAttacks(temp);
            U64 bMoves = watt & ~(watt & wBoard());
            U64 pinned = GetPinnedMoves(blockerToPinnnedMoves, sq);
            U64 legal = pinned & bMoves;
            U64ToMapMoves(moves, sq, legal);
        }
    };
    multimap<int, pair<int, char>> GetwBishopMapMoves() { multimap<int, pair<int, char>> moves; GetwBishopMapMoves(moves); return moves; };

    void GetbBishopMapMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(bBishop);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 batt = BishopAttacks(temp);
            U64 bMoves = batt & ~(batt & bBoard());
            U64 pinned = GetPinnedMoves(blockerToPinnnedMoves, sq);
            U64 legal = pinned & bMoves;
            U64ToMapMoves(moves, sq, legal);
        }
    };
    multimap<int, pair<int, char>> GetbBishopMapMoves() { multimap<int, pair<int, char>> moves; GetbBishopMapMoves(moves); return moves; };

    void GetwRookMapMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(wRook);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 watt = RookAttacks(temp);
            U64 rMoves = watt & ~(watt & wBoard());
            U64 pinned = GetPinnedMoves(blockerToPinnnedMoves, sq);
            U64 legal = pinned & rMoves;
            U64ToMapMoves(moves, sq, legal);
        }
    };
    multimap<int, pair<int, char>> GetwRookMapMoves() { multimap<int, pair<int, char>> moves; GetwRookMapMoves(moves); return moves; };

    void GetbRookMapMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(bRook);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 batt = RookAttacks(temp);
            U64 rMoves = batt & ~(batt & bBoard());
            U64 pinned = GetPinnedMoves(blockerToPinnnedMoves, sq);
            U64 legal = pinned & rMoves;
            U64ToMapMoves(moves, sq, legal);
        }
    };
    multimap<int, pair<int, char>> GetbRookMapMoves() { multimap<int, pair<int, char>> moves; GetbRookMapMoves(moves); return moves; };

    void GetwQueenMapMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(wQueen);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 wbatt = BishopAttacks(temp);
            U64 wratt = RookAttacks(temp);
            U64 watt = wratt | wbatt;
            U64 qMoves = watt & ~(watt & wBoard());
            U64 pinned = GetPinnedMoves(blockerToPinnnedMoves, sq);
            U64 legal = pinned & qMoves;
            U64ToMapMoves(moves, sq, legal);
        }
    };
    multimap<int, pair<int, char>> GetwQueenMapMoves() { multimap<int, pair<int, char>> moves; GetwQueenMapMoves(moves); return moves; };

    void GetbQueenMapMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(bQueen);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 bbatt = BishopAttacks(temp);
            U64 bratt = RookAttacks(temp);
            U64 batt = bratt | bbatt;
            U64 qMoves = batt & ~(batt & bBoard());
            U64 pinned = GetPinnedMoves(blockerToPinnnedMoves, sq);
            U64 legal = pinned & qMoves;
            U64ToMapMoves(moves, sq, legal);
        }
    };
    multimap<int, pair<int, char>> GetbQueenMapMoves() { multimap<int, pair<int, char>> moves; GetbQueenMapMoves(moves); return moves; };

    void GetwKingMapMoves(multimap<int, pair<int, char>> &moves) { U64ToMapMoves(moves, GetTrueBits(wKing)[0], wKingPsuedoMoves()); };
    multimap<int, pair<int, char>> GetwKingMapMoves() { multimap<int, pair<int, char>> moves; GetwKingMapMoves(moves); return moves; };

    void GetbKingMapMoves(multimap<int, pair<int, char>> &moves) { U64ToMapMoves(moves, GetTrueBits(bKing)[0], bKingPsuedoMoves()); };
    multimap<int, pair<int, char>> GetbKingMapMoves() { multimap<int, pair<int, char>> moves; GetbKingMapMoves(moves); return moves; };

    void GetwPinnedMapMoves(multimap<int, pair<int, char>> &moves) { GetwPawnMapMoves(moves);GetwKnightMapMoves(moves);GetwBishopMapMoves(moves);GetwRookMapMoves(moves);GetwQueenMapMoves(moves); };
    multimap<int, pair<int, char>> GetwPinnedMapMoves() { multimap<int, pair<int, char>> moves; GetwPinnedMapMoves(moves); return moves; };

    void GetbPinnedMapMoves(multimap<int, pair<int, char>> &moves) { GetbPawnMapMoves(moves);GetbKnightMapMoves(moves);GetbBishopMapMoves(moves);GetbRookMapMoves(moves);GetbQueenMapMoves(moves); };
    multimap<int, pair<int, char>> GetbPinnedMapMoves() { multimap<int, pair<int, char>> moves; GetbPinnedMapMoves(moves); return moves; };

    void GetwPinnedMapMovesNoCheck(multimap<int, pair<int, char>> &moves) { GetwPawnMapMoves(moves);GetwKnightMapMoves(moves);GetwBishopMapMoves(moves);GetwRookMapMoves(moves);GetwQueenMapMoves(moves);GetwKingMapMoves(moves); };
    multimap<int, pair<int, char>> GetwPinnedMapMovesNoCheck() { multimap<int, pair<int, char>> moves; GetwPinnedMapMovesNoCheck(moves); return moves; };

    void GetbPinnedMapMovesNoCheck(multimap<int, pair<int, char>> &moves) { GetbPawnMapMoves(moves);GetbKnightMapMoves(moves);GetbBishopMapMoves(moves);GetbRookMapMoves(moves);GetbQueenMapMoves(moves);GetbKingMapMoves(moves); };
    multimap<int, pair<int, char>> GetbPinnedMapMovesNoCheck() { multimap<int, pair<int, char>> moves; GetbPinnedMapMovesNoCheck(moves); return moves; };

    void GetPinnedMapMovesNoCheck(multimap<int, pair<int, char>> &moves) { return isWhiteMove ? GetwPinnedMapMovesNoCheck(moves): GetbPinnedMapMovesNoCheck(moves); };
    multimap<int, pair<int, char>> GetPinnedMapMovesNoCheck() { multimap<int, pair<int, char>> moves; GetPinnedMapMovesNoCheck(moves); return moves; };

    void GetwMapMoves(multimap<int, pair<int, char>> &moves) {
        if(checkToBlockSquares.size() > 1) {//if double check - move king to non occupied string 
            return GetwKingMapMoves(moves);
        } else if (checkToBlockSquares.size() == 1) {//if single check - move king, capture piece or block
            multimap<int, pair<int, char>> pinnedMoves = GetwPinnedMapMoves();
            vector<int> blocksAndCaptures = checkToBlockSquares.begin()->second;

            for(multimap<int, pair<int, char>>::const_iterator it = pinnedMoves.begin(); it != pinnedMoves.end(); ++it){
                int targetSq = it->second.first;
                if(IsIntInVector(blocksAndCaptures, targetSq)) InsertMove(moves, it->first, it->second.first, it->second.second);
            }
        
            multimap<int, pair<int, char>> kingMoves = GetwKingMapMoves();
            for(multimap<int, pair<int, char>>::const_iterator it = kingMoves.begin(); it != kingMoves.end(); ++it){
                InsertMove(moves, it->first, it->second.first, it->second.second);
            }
            return;
        }//else return all normal moves with pinned to king logic
        return GetwPinnedMapMovesNoCheck(moves);
    }

    void GetbMapMoves(multimap<int, pair<int, char>> &moves) {
        if(checkToBlockSquares.size() > 1) {//if double check - move king to non occupied string 
            return GetbKingMapMoves(moves);
        } else if (checkToBlockSquares.size() == 1) {//if single check - move king, capture piece or block
            multimap<int, pair<int, char>> pinnedMoves = GetbPinnedMapMoves();
            vector<int> blocksAndCaptures = checkToBlockSquares.begin()->second;

            for(multimap<int, pair<int, char>>::const_iterator it = pinnedMoves.begin(); it != pinnedMoves.end(); ++it){
                int targetSq = it->second.first;
                if(IsIntInVector(blocksAndCaptures, targetSq)) InsertMove(moves, it->first, it->second.first, it->second.second);
            }
        
            multimap<int, pair<int, char>> kingMoves = GetbKingMapMoves();
            for(multimap<int, pair<int, char>>::const_iterator it = kingMoves.begin(); it != kingMoves.end(); ++it){
                InsertMove(moves, it->first, it->second.first, it->second.second);
            }
            return;
        }//else return all normal moves with pinned to king logic
        return GetbPinnedMapMovesNoCheck(moves);
    }

    void GetMapMoves(multimap<int, pair<int, char>> &moves) { return isWhiteMove ? GetwMapMoves(moves) : GetbMapMoves(moves); };
    multimap<int, pair<int, char>> GetMapMoves() { multimap<int, pair<int, char>> moves; GetMapMoves(moves); return moves; };



    map<int, U64> GetBlockerPinnedToKingMovesMap() { return blockerToPinnnedMoves; };


    //make move helpers
    void GetwBoardandResetIndex(int index) {
        if(TestBit(wPawn, index)) { ResetBit(wPawn, index); RemoveMaterialValue('P'); }
        if(TestBit(wKnight, index)) { ResetBit(wKnight, index); RemoveMaterialValue('N'); }
        if(TestBit(wBishop, index)) { ResetBit(wBishop, index); RemoveMaterialValue('B'); } 
        if(TestBit(wRook, index)) { ResetBit(wRook, index); RemoveMaterialValue('R'); }
        if(TestBit(wQueen, index)) { ResetBit(wQueen, index); RemoveMaterialValue('Q'); }
        if(TestBit(wKing, index)) { ResetBit(wKing, index); RemoveMaterialValue('K'); } 
    };

    void GetbBoardandResetIndex(int index) {
        if(TestBit(bPawn, index))  { ResetBit(bPawn, index); RemoveMaterialValue('p'); } 
        if(TestBit(bKnight, index))  { ResetBit(bKnight, index); RemoveMaterialValue('n'); } 
        if(TestBit(bBishop, index))  { ResetBit(bBishop, index); RemoveMaterialValue('b'); } 
        if(TestBit(bRook, index))  { ResetBit(bRook, index); RemoveMaterialValue('r'); } 
        if(TestBit(bQueen, index))  { ResetBit(bQueen, index); RemoveMaterialValue('q'); } 
        if(TestBit(bKing, index))  { ResetBit(bKing, index); RemoveMaterialValue('k'); } 
    };

    void GetwBoardResetStartandSetTarget(int start, int target) {
        if(TestBit(wPawn, start))  { ResetBit(wPawn, start); SetBit(wPawn, target);}
        if(TestBit(wKnight, start)) { ResetBit(wKnight, start); SetBit(wKnight, target);}
        if(TestBit(wBishop, start)) { ResetBit(wBishop, start); SetBit(wBishop, target);}
        if(TestBit(wRook, start)) { ResetBit(wRook, start); SetBit(wRook, target);}
        if(TestBit(wQueen, start)) { ResetBit(wQueen, start); SetBit(wQueen, target);}
        if(TestBit(wKing, start)) { ResetBit(wKing, start); SetBit(wKing, target);}
    };

    void GetbBoardResetStartandSetTarget(int start, int target) {
        if(TestBit(bPawn, start)) { ResetBit(bPawn, start); SetBit(bPawn, target); }
        if(TestBit(bKnight, start)) { ResetBit(bKnight, start); SetBit(bKnight, target);}
        if(TestBit(bBishop, start)) { ResetBit(bBishop, start); SetBit(bBishop, target);}
        if(TestBit(bRook, start)) { ResetBit(bRook, start); SetBit(bRook, target);}
        if(TestBit(bQueen, start)) { ResetBit(bQueen, start); SetBit(bQueen, target);}
        if(TestBit(bKing, start)) { ResetBit(bKing, start); SetBit(bKing, target);}
    };

    void GetwBoardandSetPromoIndex(int index, char promoP) {
        switch(promoP){
            case 'q': SetBit(wQueen, index); AddMaterialValue('Q'); return;
            case 'r': SetBit(wRook, index); AddMaterialValue('R'); return;
            case 'b': SetBit(wBishop, index); AddMaterialValue('B'); return;
            case 'n': SetBit(wKnight, index); AddMaterialValue('N'); return;
        } 
    };

    void GetbBoardandSetPromoIndex(int index, char promoP) {
        switch(promoP){
            case 'q': SetBit(bQueen, index); AddMaterialValue('q'); return;
            case 'r': SetBit(bRook, index); AddMaterialValue('r'); return;
            case 'b': SetBit(bBishop, index); AddMaterialValue('b'); return;
            case 'n': SetBit(bKnight, index); AddMaterialValue('n'); return;
        } 
    };

    void GetBoardandResetIndex(int index, bool isCapture) {
        if(isWhiteMove && isCapture) return GetbBoardandResetIndex(index);
        if(isWhiteMove && !isCapture) return GetwBoardandResetIndex(index);

        if(!isWhiteMove && isCapture) return GetwBoardandResetIndex(index);
        if(!isWhiteMove && !isCapture) return GetbBoardandResetIndex(index);
    };

    void GetBoardandSetPromoIndex(int index, char promoP) {
        if(isWhiteMove) return GetwBoardandSetPromoIndex(index, promoP); 
        return GetbBoardandSetPromoIndex(index, promoP); 
    };

    void GetBoardResetStartandSetTarget(int start, int target) {
        if(isWhiteMove) return GetwBoardResetStartandSetTarget(start, target); 
        return GetbBoardResetStartandSetTarget(start, target); 
    };

    void EnpassantMoveUpdate(int startIndex, int targetIndex, int enpassantPawnIndex) {
        GetBoardandResetIndex(enpassantPawnIndex, true);
        GetBoardResetStartandSetTarget(startIndex, targetIndex);
    };

    void CaptureMoveUpdate(int startIndex, int targetIndex) {
        GetBoardandResetIndex(targetIndex, true);
        GetBoardResetStartandSetTarget(startIndex, targetIndex);
    };

    void QuietMoveUpdate(int startIndex, int targetIndex) {
        GetBoardResetStartandSetTarget(startIndex, targetIndex);
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

    void PromotionUpdate(bool isCap, int startIndex, int targetIndex, char promoP) { return isCap ? CapturePromoUpdate(startIndex, targetIndex, promoP) : QuietPromoUpdate(startIndex, targetIndex, promoP); };
    
    void CastleUpdateHelper(U64 &b, int start, int end) { ResetBit(b, start); SetBit(b, end); };
    void wCastleLongUpdate() { CastleUpdateHelper(wRook, 56, 59); CastleUpdateHelper(wKing, 60, 58); };
    void wCastleShortUpdate() { CastleUpdateHelper(wRook, 63, 61); CastleUpdateHelper(wKing, 60, 62); };

    void wCastleUpdate(int targetMinusStart) { return (targetMinusStart > 0) ? wCastleShortUpdate() : wCastleLongUpdate(); };

    void bCastleLongUpdate() { CastleUpdateHelper(bRook, 0, 3); CastleUpdateHelper(bKing, 4, 2); };
    void bCastleShortUpdate() { CastleUpdateHelper(bRook, 7, 5); CastleUpdateHelper(bKing, 4, 2); };

    void bCastleUpdate(int targetMinusStart) { return (targetMinusStart > 0) ? bCastleShortUpdate() : bCastleLongUpdate(); };
    void CastleUpdate(int targetMinusStart) { return (isWhiteMove) ? wCastleUpdate(targetMinusStart) : bCastleUpdate(targetMinusStart); };

    multimap<int, pair<int, char>> GetwBoardMoves(int index) {
        if(TestBit(wPawn, index)) return GetwPawnMapMoves(); 
        if(TestBit(wKnight, index)) return GetwKnightMapMoves();
        if(TestBit(wBishop, index)) return GetwBishopMapMoves();
        if(TestBit(wRook, index)) return GetwRookMapMoves();
        if(TestBit(wQueen, index)) return GetwQueenMapMoves();
        if(TestBit(wKing, index)) return GetwKingMapMoves();
        multimap<int, pair<int, char>> m;
        return m;
    };

    multimap<int, pair<int, char>> GetbBoardMoves(int index) {
        if(TestBit(bPawn, index)) return GetbPawnMapMoves(); 
        if(TestBit(bKnight, index)) return GetbKnightMapMoves();
        if(TestBit(bBishop, index)) return GetbBishopMapMoves();
        if(TestBit(bRook, index)) return GetbRookMapMoves();
        if(TestBit(bQueen, index)) return GetbQueenMapMoves();
        if(TestBit(bKing, index)) return GetbKingMapMoves();
        multimap<int, pair<int, char>> m;
        return m;
    };

    multimap<int, pair<int, char>> GetMovesByBoard(int index) { return isWhiteMove ? GetwBoardMoves(index): GetbBoardMoves(index); };

    bool isPromotionSquare(int sq) { if(isWhiteMove) return (sq < 8); return (sq > 55); };
    bool isCapture(int targetSq) { U64 allb = AllBoard(); return TestBit(allb, targetSq); };
    bool isKingCapture(int targetSq) { U64 kingb = AllKing(); return TestBit(kingb, targetSq); };
    bool isEnpassant(int startSq, int targetSq) { return isPawnMove(startSq) && (targetSq == enPassantTarget); };
    bool isKingMove(int startSq) { U64 king = isWhiteMove ? wKing : bKing; return TestBit(king, startSq); };
    bool isCastle(int startSq, int targetSq) { bool isKing = isKingMove(startSq); bool isCastle = abs(startSq-targetSq) == 2; return isCastle && isKing; };
    bool isPawnMove(int startSq) { U64 pawn = isWhiteMove ? wPawn : bPawn; return TestBit(pawn, startSq); };
    int enPassantOffset(int enPassantTarget) { return isWhiteMove ? enPassantTarget + 8 : enPassantTarget - 8; };

    bool isDoublePawnMove(int startSq, int targetSq) { return isPawnMove(startSq) && (abs(startSq-targetSq) == 16); };

    void UpdateCastlingRightsFromKing() {
        if(isWhiteMove) {
            SetCastlingRightsFalse(castlingRights, 'Q');
            SetCastlingRightsFalse(castlingRights, 'K');
        } else {
            SetCastlingRightsFalse(castlingRights, 'q');
            SetCastlingRightsFalse(castlingRights, 'k');
        }
    }

    void UpdateCastlingRightsFromRook(int startSq, int targetSq) {
        if(startSq == 0 || targetSq == 0) { SetCastlingRightsFalse(castlingRights, 'q'); }
        if(startSq == 7 || targetSq == 7) { SetCastlingRightsFalse(castlingRights, 'k'); }
        if(startSq == 56 || targetSq == 56) { SetCastlingRightsFalse(castlingRights, 'Q'); }
        if(startSq == 63 || targetSq == 63) { SetCastlingRightsFalse(castlingRights, 'K'); }
    };

    //making moves 
    void SetMoveData() { ClearAttacks(); SetAttacks(); };

    bool PossibleMoveIsACapture(int startSq, int targetSq) { return isCapture(targetSq) || isEnpassant(startSq, targetSq); };
    bool isInCheck() { return checkToBlockSquares.size() > 0; };

    bool isMoveCheck(string move) {
        string fen = GetFen();
        bool moveMade = MakeMove(move);
        bool moveGivesCheck = isInCheck();
        LoadFen(fen);
        return moveGivesCheck;
    };

    bool isMoveCheck(int startSq, int targetSq, char promoP = ' ') {
        string fen = GetFen();
        bool moveMade = MakeMove(startSq, targetSq, promoP);
        bool moveGivesCheck = isInCheck();
        LoadFen(fen);
        return moveGivesCheck;
    };

    bool isCheckMate() { multimap<int, pair<int, char>> moves = GetMapMoves(); return (moves.size() == 0) && (checkToBlockSquares.size() != 0) ? true : false; };
    bool isStaleMate() { multimap<int, pair<int, char>> moves = GetMapMoves(); return (moves.size() == 0) && (checkToBlockSquares.size() == 0) ? true : false; };
    bool is50MoveRule() { return halfMoveClock > 100; };
    bool is3FoldRepition() { return false; };
    bool inSufficientMaterial() { return GetTrueBits(AllBoard()).size() < 3;}
    bool isDraw() { return ( isStaleMate() || is50MoveRule() || is3FoldRepition() || inSufficientMaterial() ); };
    bool isGameOver() { return isCheckMate() || isDraw(); };

    bool isWhiteWin() { return !isWhiteMove && isCheckMate(); };
    bool isBlackWin() { return isWhiteMove && isCheckMate(); };


    bool MakeMove(vector<string> listmoves) {
        bool movesMade = true;
        for(string move : listmoves) {
            movesMade &= MakeMove(move);
        }
        return movesMade;
    };

    bool MakeMove(string move) {
        string startSquare = move.substr(0, 2);
        string targetSquare = move.substr(2, 2);
        int startIndex = StringtoIndex(startSquare);
        int targetIndex = StringtoIndex(targetSquare);
        char promoP = (move.length() == 5) ? move[4] : ' ';

        return MakeMove(startIndex, targetIndex, promoP);
    };

    bool MakeMove(int startSq, int targetSq, char promoP = ' ') {
        multimap<int, pair<int, char>> movesBoard = GetMovesByBoard(startSq);
        bool validMove = FindMoveInMap(movesBoard, startSq);
        if(!validMove) return false;

        //booleans for moves
        bool isPromotion = promoP != ' ';
        bool isMovePawn = isPawnMove(startSq);
        bool isMoveKing = isKingMove(startSq);
        bool isCaptureMove = isCapture(targetSq);
        bool isCastleMove = isCastle(startSq, targetSq);
        bool isDoublePawn = isDoublePawnMove(startSq, targetSq);
        bool isEnpassantMove = isEnpassant(startSq, targetSq);

        //board updates 
        if(isPromotion) PromotionUpdate(isCaptureMove, startSq, targetSq, promoP);
        if(isCastleMove) CastleUpdate(targetSq - startSq);
        if(isEnpassantMove) EnpassantMoveUpdate(startSq, targetSq, enPassantOffset(enPassantTarget));
        if(isCaptureMove) CaptureMoveUpdate(startSq, targetSq); 
        if(!isCaptureMove) QuietMoveUpdate(startSq, targetSq); 

        //castling right update
        isMoveKing ? UpdateCastlingRightsFromKing() : UpdateCastlingRightsFromRook(startSq, targetSq);
    
        //enpassant target 
        if(isDoublePawn && isWhiteMove) { enPassantTarget = startSq-8; }
        if(isDoublePawn && !isWhiteMove) { enPassantTarget = startSq+8; }
        if(!isDoublePawn) { enPassantTarget = 0; }
        
        //move side, move num and half clock num 
        isMovePawn || isCaptureMove ? halfMoveClock = 0: halfMoveClock++;
        isWhiteMove = !isWhiteMove;
        if(isWhiteMove) { fullTurnNum++; }

        //isMoveRepition = UpdateFenMapAndFind3Move(hashFen, GetFenHelper() + " " + CastlingRightsString(castlingRights));
        SetMoveData();
        return true;
    };

    //misc
    void PrintAllBoards() {
        Print(wPawn, "wPawn");
        Print(bPawn, "bPawn");
        Print(wKnight, "wKnight");
        Print(bKnight, "bKnight");
        Print(wBishop, "wBishop");
        Print(bBishop, "bBishop");
        Print(wRook, "wRook");
        Print(bRook, "bRook");
        Print(wQueen, "wQueen");
        Print(bQueen, "bQueen");
        Print(wKing, "wKing");
        Print(bKing, "bKing");
    };
};