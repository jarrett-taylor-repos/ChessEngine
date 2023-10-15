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

    int halfMoveClock;
    int fullTurnNum;
    int enPassantTarget;
    string currFen;
    bool isWhiteMove;
    map<char, bool> castlingRights;
    map<string, int> hashFen; //used to see 3 move repition of fen, will be used in the future

    int materialValue;


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
        this->hashFen = other.hashFen;

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
    };

    string GetFen() {
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

        string moveColor = isWhiteMove ? " w" : " b";
        string fenOthers = moveColor +" "+ CastlingRightsString(castlingRights) +" "+ EnpassantTargetToString(enPassantTarget) +" "+ to_string(halfMoveClock) +" "+ to_string(fullTurnNum);
        fen += fenOthers;

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

    void ClearBoard() {
        Reset(wPawn); Reset(wKnight); Reset(wBishop); Reset(wRook); Reset(wQueen); Reset(wKing);
        Reset(bPawn); Reset(bKnight); Reset(bBishop); Reset(bRook); Reset(bQueen); Reset(bKing);
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

    bool IsWhiteMove() { return isWhiteMove; };
    int GetMoveMultiplier() { return (isWhiteMove) ? 1 : -1; };

    //board functions
    U64 wBoard() { return wPawn | wKnight | wBishop | wRook | wQueen | wKing; };
    U64 bBoard() { return bPawn | bKnight | bBishop | bRook | bQueen | bKing; };
    U64 AllBoard() { return wBoard() | bBoard(); };

    U64 EmptyBoard() { return ~AllBoard(); };
    U64 NotbBoard() { return ~bBoard(); };
    U64 NotwBoard() { return ~wBoard(); };

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

    U64 wPawnMoves() { return wPawnAllCaptures() | wPawnPushes(); };
    U64 bPawnMoves() { return bPawnAllCaptures() | bPawnPushes(); };

    U64 wPawnWestAtt(U64 b) { return (b >> 9) & notHFile; };
    U64 wPawnEastAtt(U64 b) { return (b >> 7) & notAFile; };
    U64 wPawnAllAtt(U64 b) { return wPawnEastAtt(b) | wPawnWestAtt(b); };
    U64 bPawnWestAtt(U64 b) { return (b << 9) & notAFile; };
    U64 bPawnEastAtt(U64 b) { return (b << 7) & notHFile; };
    U64 bPawnAllAtt(U64 b) { return bPawnEastAtt(b) | bPawnWestAtt(b); };
    U64 wPawnWestCaptures(U64 b) { 
        U64 wpatt = wPawnWestAtt(b);
        U64 enpass = SingleBitBoard(enPassantTarget);
        U64 westcaptures = (wpatt & bBoard()) | (wpatt & enpass);
        return westcaptures;
    };
    U64 wPawnEastCaptures(U64 b) { 
        U64 wpatt = wPawnEastAtt(b);
        U64 enpass = SingleBitBoard(enPassantTarget);
        U64 eastcaptures = (wpatt & bBoard()) | (wpatt & enpass);
        return eastcaptures;
    };
    U64 wPawnAllCaptures(U64 b) { return (wPawnEastCaptures(b) | wPawnWestCaptures(b)) & bBoard(); };
    U64 bPawnWestCaptures(U64 b) { 
        U64 bpatt = bPawnWestAtt(b);
        U64 enpass = SingleBitBoard(enPassantTarget);
        U64 westcaptures = (bpatt & wBoard()) | (bpatt & enpass);
        return westcaptures;
    };
    U64 bPawnEastCaptures(U64 b) { 
        U64 bpatt = bPawnEastAtt(b);
        U64 enpass = SingleBitBoard(enPassantTarget);
        U64 eastcaptures = (bpatt & wBoard()) | (bpatt & enpass);
        return eastcaptures;
    };
    U64 bPawnAllCaptures(U64 b) { return (bPawnEastCaptures(b) | bPawnWestCaptures(b)) & wBoard(); };

    U64 wPawnMoves(U64 b) { return wPawnAllCaptures(b) | wPawnPushes(b); };
    U64 bPawnMoves(U64 b) { return bPawnAllCaptures(b) | bPawnPushes(b); };


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

    U64 wKnightMoves(){ return KnightAttacks(wKnight) & NotwBoard(); };
    U64 bKnightMoves(){ return KnightAttacks(bKnight) & NotbBoard(); };

    U64 wKnightAtt(){ return KnightAttacks(wKnight); };
    U64 bKnightAtt(){ return KnightAttacks(bKnight); };

    

    //king moves
    U64 wKingCastleShort() {
        bool canCastle = GetCastlingRightsValueByChar(castlingRights, 'K');
        if(!canCastle) return C64(0);
        U64 allb = AllBoard();
        bool hasBlocker = TestBit(allb, 61) || TestBit(allb, 62);
        if(hasBlocker) return C64(0);
        return SingleBitBoard(62);
    };

    U64 wKingCastleLong() {
        bool canCastle = GetCastlingRightsValueByChar(castlingRights, 'Q');
        if(!canCastle) return C64(0);
        U64 allb = AllBoard();
        bool hasBlocker = TestBit(allb, 57) || TestBit(allb, 58) || TestBit(allb, 59);
        if(hasBlocker) return C64(0);
        return SingleBitBoard(58);
    };

    U64 wKingCastle() { return wKingCastleShort() | wKingCastleLong(); };

    U64 bKingCastleShort() {
        bool canCastle = GetCastlingRightsValueByChar(castlingRights, 'k');
        if(!canCastle) return C64(0);
        U64 allb = AllBoard();
        bool hasBlocker = TestBit(allb, 5) || TestBit(allb, 6);
        if(hasBlocker) return C64(0);
        return SingleBitBoard(6);
    };

    U64 bKingCastleLong() {
        bool canCastle = GetCastlingRightsValueByChar(castlingRights, 'q');
        if(!canCastle) return C64(0);
        U64 allb = AllBoard();
        bool hasBlocker = TestBit(allb, 1) || TestBit(allb, 2) || TestBit(allb, 3);
        if(hasBlocker) return C64(0);
        return SingleBitBoard(2);
    };

    U64 bKingCastle() { return bKingCastleShort() | bKingCastleLong(); };

    U64 wKingMoves() { return (OneInAllDirection(wKing) | wKingCastle() ) & NotwBoard(); };
    U64 bKingMoves() { return (OneInAllDirection(bKing) | bKingCastle() ) & NotbBoard(); };

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
        U64 notocc = !occ;
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
        U64 moves = 0;
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
    U64 wBishopMoves() { U64 wbatt = wBishopAttacks(); return wbatt & ~(wbatt & wBoard()); };
    U64 bBishopMoves() { U64 bbatt = bBishopAttacks(); return bbatt & ~(bbatt & bBoard()); };

    U64 wRookAttacks() { return RookAttacks(wRook); };
    U64 bRookAttacks() { return RookAttacks(bRook); };
    U64 wRookMoves() { U64 wratt = wRookAttacks(); return wratt & ~(wratt & wBoard()); };
    U64 bRookMoves() { U64 bratt = bRookAttacks(); return bratt & ~(bratt & bBoard()); };

    U64 wQueenAttacks() { return RookAttacks(wQueen) | BishopAttacks(wQueen); };
    U64 bQueenAttacks() { return RookAttacks(bQueen) | BishopAttacks(bQueen); };
    U64 wQueenMoves() { U64 wqatt = wQueenAttacks(); return wqatt & ~(wqatt & wBoard()); };
    U64 bQueenMoves() { U64 bqatt = bQueenAttacks(); return bqatt & ~(bqatt & bBoard()); };

    //attacks
    U64 wAttacks() { return wPawnAllAtt() | wKingAtt() | wKnightAtt() | wQueenAttacks() | wBishopAttacks() | wRookAttacks(); };
    U64 bAttacks() { return bPawnAllAtt() | bKingAtt() | bKnightAtt() | bQueenAttacks() | bBishopAttacks() | bRookAttacks(); };

    U64 xRaywRookAttacks() {
        U64 wrookmoves = wRookMoves();
        U64 occ = AllBoard();
        U64 blockers = wrookmoves & occ;
        blockers &= wrookmoves;
        U64 occAttacks = RookAttacks(wRook, occ ^ blockers);
        U64 xrayattcks = (wrookmoves ^ occAttacks) & bBoard();
        return xrayattcks;
    };

    U64 xRaybRookAttacks() {
        U64 brookmoves = bRookMoves();
        U64 occ = AllBoard();
        U64 blockers = brookmoves & occ;
        blockers &= brookmoves;
        U64 occAttacks = RookAttacks(bRook, occ ^ blockers);
        U64 xrayattcks = (brookmoves ^ occAttacks) & wBoard();
        return xrayattcks;
    };

    U64 xRaywBishopAttacks() {
        U64 wbishopmoves =  wBishopMoves();
        U64 occ = AllBoard();
        U64 blockers = wbishopmoves & occ;
        blockers &= wbishopmoves;
        U64 occAttacks = BishopAttacks(wBishop, occ ^ blockers);
        U64 xrayattcks = (wbishopmoves ^ occAttacks) & bBoard();
        return xrayattcks;
    };

    U64 xRaybBishopAttacks() {
        U64 bbishopmoves =  bBishopMoves();
        U64 occ = AllBoard();
        U64 blockers = bbishopmoves & occ;
        blockers &= bbishopmoves;
        U64 occAttacks = BishopAttacks(bBishop, occ ^ blockers);
        U64 xrayattcks = (bbishopmoves ^ occAttacks) & wBoard();
        return xrayattcks;
    };

    U64 xRaywQueenAttacks() {
        U64 wqueenmoves =  wQueenMoves();
        U64 occ = AllBoard();
        U64 blockers = wqueenmoves & occ;
        blockers &= wqueenmoves;
        U64 occAttacks = QueenAttacks(wQueen, occ ^ blockers);
        U64 xrayattcks = (wqueenmoves ^ occAttacks) & bBoard();
        return xrayattcks;
    };

    U64 xRaybQueenAttacks() {
        U64 bqueenmoves =  bQueenMoves();
        U64 occ = AllBoard();
        U64 blockers = bqueenmoves & occ;
        blockers &= bqueenmoves;
        U64 occAttacks = QueenAttacks(bBishop, occ ^ blockers);
        U64 xrayattcks = (bqueenmoves ^ occAttacks) & wBoard();
        return xrayattcks;
    };

    U64 xRaywAttacks() { return xRaywRookAttacks() | xRaywBishopAttacks() | xRaywQueenAttacks(); };
    U64 xRaybAttacks() { return xRaybRookAttacks() | xRaybBishopAttacks() | xRaybQueenAttacks(); };

    U64 xRayAttacks() { 
        if(isWhiteMove) return xRaybAttacks();
        return xRaywAttacks();
    };
    

    //making moves helpers 
    bool isPawn(int sq) {
        if(isWhiteMove) return TestBit(wPawn, sq);
        return TestBit(bPawn, sq);
    };


    //all moves
    U64 wMovesWithCheck() {
        //if double check - move king to non occupied string  
        //if single check - move king, capture piece or block
        //else return all normal moves with pinned logic
    }

    U64 wMoves() { return wPawnMoves() | wKnightMoves() | wBishopMoves() | wRookMoves() | wQueenMoves() | wKingMoves(); };
    U64 bMoves() { return bPawnMoves() | bKnightMoves() | bBishopMoves() | bRookMoves() | bQueenMoves() | bKingMoves(); };

    U64 GetPawnMoves() {
        if(isWhiteMove) return wPawnMoves();
        return bPawnMoves();
    };

    U64 GetKnightMoves() {
        if(isWhiteMove) return wKnightMoves();
        return bKnightMoves();
    };

    U64 GetBishopMoves() {
        if(isWhiteMove) return wBishopMoves();
        return bBishopMoves();
    };

    U64 GetRookMoves() {
        if(isWhiteMove) return wRookMoves();
        return bRookMoves();
    };

    U64 GetQueenMoves() {
        if(isWhiteMove) return wQueenMoves();
        return bQueenMoves();
    };

    U64 GetKingMoves() {
        if(isWhiteMove) return wKingMoves();
        return bKingMoves();
    };

    U64 GetMoves() {
        if(isWhiteMove) return wMoves();
        return bMoves();
    };

    //vector<int> moves
    void GetwMapPawnMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(wPawn);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 pawnMoves = wPawnMoves(temp);
            U64ToMapMoves(moves, sq, pawnMoves, true, true);
        }
    };

    void GetbMapPawnMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(bPawn);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 pawnMoves = bPawnMoves(temp);
            U64ToMapMoves(moves, sq, pawnMoves, true, false);
        }
    };

    void GetwMapKnightMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(wKnight);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 watt = KnightAttacks(temp);
            U64 knightMoves = watt & NotwBoard();
            U64ToMapMoves(moves, sq, knightMoves);
        }
    };

    void GetbMapKnightMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(bKnight);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 batt = KnightAttacks(temp);
            U64 knightMoves = batt & NotbBoard();
            U64ToMapMoves(moves, sq, knightMoves);
        }
    };

    void GetwMapBishopMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(wBishop);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 watt = BishopAttacks(temp);
            U64 bMoves = watt & ~(watt & wBoard());
            U64ToMapMoves(moves, sq, bMoves);
        }
    };

    void GetbMapBishopMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(bBishop);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 batt = BishopAttacks(temp);
            U64 bMoves = batt & ~(batt & bBoard());
            U64ToMapMoves(moves, sq, bMoves);
        }
    };

    void GetwMapRookMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(wRook);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 watt = RookAttacks(temp);
            U64 rMoves = watt & ~(watt & wBoard());
            U64ToMapMoves(moves, sq, rMoves);
        }
    };

    void GetbMapRookMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(bRook);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 batt = RookAttacks(temp);
            U64 rMoves = batt & ~(batt & bBoard());
            U64ToMapMoves(moves, sq, rMoves);
        }
    };

    void GetwMapQueenMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(wQueen);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 wbatt = BishopAttacks(temp);
            U64 wratt = RookAttacks(temp);
            U64 watt = wratt | wbatt;
            U64 qMoves = watt & ~(watt & wBoard());
            U64ToMapMoves(moves, sq, qMoves);
        }
    };

    void GetbMapQueenMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(bQueen);
        for(int sq : indexes) {
            U64 temp = SingleBitBoard(sq);
            U64 bbatt = BishopAttacks(temp);
            U64 bratt = RookAttacks(temp);
            U64 batt = bratt | bbatt;
            U64 qMoves = batt & ~(batt & bBoard());
            U64ToMapMoves(moves, sq, qMoves);
        }
    };

    void GetwMapKingMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(wKing);
        U64 kingMoves = wKingMoves();
        U64ToMapMoves(moves, indexes[0], kingMoves);
    };

    void GetbMapKingMoves(multimap<int, pair<int, char>> &moves) {
        vector<int> indexes = GetTrueBits(bKing);
        U64 kingMoves = bKingMoves();
        U64ToMapMoves(moves, indexes[0], kingMoves);
    };

    void GetwMapMoves(multimap<int, pair<int, char>> &moves) {
        GetwMapPawnMoves(moves);
        GetwMapKnightMoves(moves);
        GetwMapBishopMoves(moves);
        GetwMapRookMoves(moves);
        GetwMapQueenMoves(moves);
        GetwMapKingMoves(moves);
    };

    void GetbMapMoves(multimap<int, pair<int, char>> &moves) {
        GetbMapPawnMoves(moves);
        GetbMapKnightMoves(moves);
        GetbMapBishopMoves(moves);
        GetbMapRookMoves(moves);
        GetbMapQueenMoves(moves);
        GetbMapKingMoves(moves);
    };

    void GetMapMoves(multimap<int, pair<int, char>> &moves) {
        if(isWhiteMove) return GetwMapMoves(moves);
        return GetbMapMoves(moves);
    };

    multimap<int, pair<int, char>> GetMapMoves() {
        multimap<int, pair<int, char>> moves;
        GetMapMoves(moves);
        return moves;
    };


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

    void PromotionUpdate(bool isCap, int startIndex, int targetIndex, char promoP) {
        if(isCap) return CapturePromoUpdate(startIndex, targetIndex, promoP);
        QuietPromoUpdate(startIndex, targetIndex, promoP);
    };
    
    void CastleUpdateHelper(U64 &b, int start, int end) { ResetBit(b, start); SetBit(b, end); };
    void wCastleLongUpdate() { CastleUpdateHelper(wRook, 56, 59); CastleUpdateHelper(wKing, 60, 58); };
    void wCastleShortUpdate() { CastleUpdateHelper(wRook, 63, 61); CastleUpdateHelper(wKing, 60, 62); };

    void wCastleUpdate(int targetMinusStart) {
        if(targetMinusStart > 0) return wCastleShortUpdate();
        return wCastleLongUpdate();
    };

    void bCastleLongUpdate() { CastleUpdateHelper(bRook, 0, 3); CastleUpdateHelper(bKing, 4, 2); };
    void bCastleShortUpdate() { CastleUpdateHelper(bRook, 7, 5); CastleUpdateHelper(bKing, 4, 2); };

    void bCastleUpdate(int targetMinusStart) {
        if(targetMinusStart > 0) return bCastleShortUpdate();
        return bCastleLongUpdate();
    };

    void CastleUpdate(int targetMinusStart) {
        if(isWhiteMove) return wCastleUpdate(targetMinusStart);
        return bCastleUpdate(targetMinusStart);
    };

    U64 GetwBoardMoves(int index) {
        if(TestBit(wPawn, index)) return wPawnMoves(); 
        if(TestBit(wKnight, index)) return wKnightMoves();
        if(TestBit(wBishop, index)) return wBishopMoves();
        if(TestBit(wRook, index)) return wRookMoves();
        if(TestBit(wQueen, index)) return wQueenMoves();
        if(TestBit(wKing, index)) return wKingMoves();
        return C64(0);
    };

    U64 GetbBoardMoves(int index) {
        if(TestBit(bPawn, index)) return bPawnMoves(); 
        if(TestBit(bKnight, index)) return bKnightMoves();
        if(TestBit(bBishop, index)) return bBishopMoves();
        if(TestBit(bRook, index)) return bRookMoves();
        if(TestBit(bQueen, index)) return bQueenMoves();
        if(TestBit(bKing, index)) return bKingMoves();
        return C64(0);
    };

    U64 GetMovesByBoard(int index) {
        if(isWhiteMove) return GetwBoardMoves(index);
        return GetbBoardMoves(index);
    };

    bool isPromotionSquare(int sq) { if(isWhiteMove) return (sq < 8); return (sq > 55); };
    bool isCapture(int targetSq) { U64 allb = AllBoard(); return TestBit(allb, targetSq); };
    bool isEnpassant(int startSq, int targetSq) { return isPawnMove(startSq) && (targetSq == enPassantTarget); };
    bool isKingMove(int startSq) { U64 king = isWhiteMove ? wKing : bKing; return TestBit(king, startSq); };
    bool isCastle(int startSq, int targetSq) { bool isKing = isKingMove(startSq); bool isCastle = abs(startSq-targetSq) == 2; return isCastle && isKing; };
    bool isPawnMove(int startSq) { U64 pawn = isWhiteMove ? wPawn : bPawn; return TestBit(pawn, startSq); };

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
    bool PossibleMoveIsACapture(int startSq, int targetSq) { return isCapture(targetSq) || isEnpassant(startSq, targetSq); };

    bool MakeMove(string move) {
        string startSquare = move.substr(0, 2);
        string targetSquare = move.substr(2, 2);
        int startIndex = StringtoIndex(startSquare);
        int targetIndex = StringtoIndex(targetSquare);
        char promoP = (move.length() == 5) ? move[4] : ' ';

        return MakeMove(startIndex, targetIndex, promoP);
    };

    bool MakeMove(int startSq, int targetSq, char promoP = ' ') {
        U64 movesBoard = GetMovesByBoard(startSq);
        if(movesBoard == C64(0) || !TestBit(movesBoard, targetSq)) return false;

        //booleans for moves
        bool isPromotion = promoP != ' ';
        bool isMovePawn = isPawnMove(startSq);
        bool isMoveKing = isKingMove(startSq);
        bool isCaptureMove = isCapture(targetSq);
        bool isCastleMove = isCastle(startSq, targetSq);
        bool isDoublePawn = isDoublePawnMove(startSq, targetSq);
        bool isEnpassantMove = isEnpassant(startSq, targetSq);

        //board updates 
        if(isPromotion) {
            PromotionUpdate(isCaptureMove, startSq, targetSq, promoP);
        } else if (isCastleMove) {
            CastleUpdate(targetSq - startSq);
        } else if(isEnpassantMove) {
            int offsetSq = isWhiteMove ? enPassantTarget + 8 : enPassantTarget - 8; 
            EnpassantMoveUpdate(startSq, targetSq, offsetSq);
        } else if(isCaptureMove) {
            CaptureMoveUpdate(startSq, targetSq); 
        } else {
            QuietMoveUpdate(startSq, targetSq);
        }

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