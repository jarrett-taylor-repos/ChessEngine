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
    string enPassantTarget;
    string currFen;
    bool isWhiteMove;
    string castlingRights;
    map<string, int> hashFen; //used to see 3 move repition of fen, will be used in the future


    public:
    U64Bitboard() { LoadFen(startFen); };
    U64Bitboard(string fen) { LoadFen(fen); };

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
            case 'p': SetBit(bPawn, sq);return;
            case 'b': SetBit(bBishop, sq);return;
            case 'n': SetBit(bKnight, sq);return;
            case 'r': SetBit(bRook, sq);return;
            case 'q': SetBit(bQueen, sq);return;
            case 'k': SetBit(bKing, sq);return;
            case 'P': SetBit(wPawn, sq);return;
            case 'B': SetBit(wBishop, sq);return;
            case 'N': SetBit(wKnight, sq);return;
            case 'R': SetBit(wRook, sq);return;
            case 'Q': SetBit(wQueen, sq);return;
            case 'K': SetBit(wKing, sq);return;
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

    void ClearBoard() {
        Reset(wPawn);
        Reset(wKnight);
        Reset(wBishop);
        Reset(wRook);
        Reset(wQueen);
        Reset(wKing);

        Reset(bPawn);
        Reset(bKnight);
        Reset(bBishop);
        Reset(bRook);
        Reset(bQueen);
        Reset(bKing);
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

    U64 wPawnWestAtt() { return (wPawn >> 9) & notHFile; };
    U64 wPawnEastAtt() { return (wPawn >> 7) & notAFile; };
    U64 wPawnAllAtt() { return wPawnEastAtt() | wPawnWestAtt(); };
    U64 bPawnWestAtt() { return (bPawn << 9) & notAFile; };
    U64 bPawnEastAtt() { return (bPawn << 7) & notHFile; };
    U64 bPawnAllAtt() { return bPawnEastAtt() | bPawnWestAtt(); };
    U64 wPawnWestCaptures() { return wPawnWestAtt() & bBoard(); };
    U64 wPawnEastCaptures() { return wPawnEastAtt() & bBoard(); };
    U64 wPawnAllCaptures() { return wPawnEastCaptures() | wPawnWestCaptures(); };
    U64 bPawnWestCaptures() { return bPawnWestAtt() & wBoard(); };
    U64 bPawnEastCaptures() { return bPawnEastAtt() & wBoard(); };
    U64 bPawnAllCaptures() { return bPawnEastCaptures() | bPawnWestCaptures(); };

    U64 wPawnMoves() { return wPawnAllCaptures() | wPawnPushes(); };
    U64 bPawnMoves() { return bPawnAllCaptures() | bPawnPushes(); };


    //knight functions
    U64 KnightMoveHelper(U64 b) { return noNoEa(b) | noEaEa(b) | soEaEa(b) | soSoEa(b) | noNoWe(b) | noWeWe(b) | soWeWe(b) | soSoWe(b); };
    U64 noNoEa(U64 b) {return (b >> 15) & notAFile; };
    U64 noEaEa(U64 b) {return (b >> 6) & (notAFile & notBFile);};
    U64 soEaEa(U64 b) {return (b << 10) & (notAFile & notBFile);};
    U64 soSoEa(U64 b) {return (b << 17) & notAFile; };
    U64 soSoWe(U64 b) {return (b << 15) & notHFile; };
    U64 soWeWe(U64 b) {return (b << 6) & (notGFile & notHFile); };
    U64 noWeWe(U64 b) {return (b >> 10) & (notGFile & notHFile); };
    U64 noNoWe(U64 b) {return (b >> 17) & notHFile; };

    U64 wKnightMoves(){ return KnightMoveHelper(wKnight) & NotwBoard(); };
    U64 bKnightMoves(){ return KnightMoveHelper(bKnight) & NotbBoard(); };

    //king moves
    U64 wKingMoves() { return OneInAllDirection(wKing) & NotwBoard(); };
    U64 bKingMoves() { return OneInAllDirection(bKing) & NotbBoard(); };

    //sliding moves helper
    bool ValidTravel(U64 overlap, int offset) { 
        U64 notallb = !AllBoard();
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
        U64 moves;
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
        U64 moves;
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
        U64 moves;
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

    U64 BishopAttacksBySquare(U64 occ, int sq) {
        U64 moves;
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
        U64 moves;
        vector<int> indexes = GetTrueBits(bishopBoard);
        for(int sq : indexes) moves |= BishopAttacksBySquare(sq); 
        return moves;
    };

    U64 BishopAttacks(U64 bishopBoard, U64 occ) {
        U64 moves;
        vector<int> indexes = GetTrueBits(bishopBoard);
        for(int sq : indexes) moves |= BishopAttacksBySquare(occ, sq); 
        return moves;
    };

    U64 RookAttacksBySquare(int sq) {
        U64 moves;
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
        U64 moves;
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
        U64 moves;
        vector<int> indexes = GetTrueBits(rBoard);
        for(int sq : indexes) moves |= RookAttacksBySquare(sq);
        return moves;
    };

    U64 RookAttacks(U64 rBoard, U64 occ) {
        U64 moves;
        vector<int> indexes = GetTrueBits(rBoard);
        for(int sq : indexes) moves |= RookAttacksBySquare(occ, sq);
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
    U64 wAttacks() { return wPawnAllAtt() | wKingMoves() | wKnightMoves() | wQueenAttacks() | wBishopAttacks() | wRookAttacks(); };
    U64 bAttacks() { return bPawnAllAtt() | bKingMoves() | bKnightMoves() | bQueenAttacks() | bBishopAttacks() | bRookAttacks(); };

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

    //making moves helpers 
    bool isPawn(int sq) {
        if(isWhiteMove) return TestBit(wPawn, sq);
        return TestBit(bPawn, sq);
    };

    
    //all moves
    U64 wMoves() { return wPawnMoves() | wKnightMoves() | wBishopMoves() | wRookMoves() | wQueenMoves() | wKingMoves(); };
    U64 bMoves() { return bPawnMoves() | bKnightMoves() | bBishopMoves() | bRookMoves() | bQueenMoves() | bKingMoves(); };

    U64 GetMoves() {
        if(isWhiteMove) return wMoves();
        return bMoves();
    };

    bool isPromotionSquare(int sq) {
        if(isWhiteMove) return (sq < 8);
        return (sq > 55);
    };

    void GetwBoardandResetIndex(int index) {
        if(TestBit(wPawn, index)) ResetBit(wPawn, index); 
        if(TestBit(wKnight, index)) ResetBit(wKnight, index); 
        if(TestBit(wBishop, index)) ResetBit(wBishop, index); 
        if(TestBit(wRook, index)) ResetBit(wRook, index); 
        if(TestBit(wQueen, index)) ResetBit(wQueen, index); 
        if(TestBit(wKing, index)) ResetBit(wKing, index); 
    };

    void GetbBoardandResetIndex(int index) {
        if(TestBit(bPawn, index))  ResetBit(bPawn, index); 
        if(TestBit(bKnight, index))  ResetBit(bKnight, index); 
        if(TestBit(bBishop, index))  ResetBit(bBishop, index); 
        if(TestBit(bRook, index))  ResetBit(bRook, index); 
        if(TestBit(bQueen, index))  ResetBit(bQueen, index); 
        if(TestBit(bKing, index))  ResetBit(bKing, index); 
    };

    void GetwBoardandSetIndex(int index) {
        if(TestBit(wPawn,index)) SetBit(wPawn,index); 
        if(TestBit(wKnight, index)) SetBit(wKnight, index); 
        if(TestBit(wBishop, index)) SetBit(wBishop, index); 
        if(TestBit(wRook, index)) SetBit(wRook, index); 
        if(TestBit(wQueen, index)) SetBit(wQueen, index); 
        if(TestBit(wKing, index)) SetBit(wKing, index); 
    };

    void GetbBoardandSetIndex(int index) {
        if(TestBit(bPawn, index))  SetBit(bPawn, index); 
        if(TestBit(bKnight, index))  SetBit(bKnight, index); 
        if(TestBit(bBishop, index))  SetBit(bBishop, index); 
        if(TestBit(bRook, index))  SetBit(bRook, index); 
        if(TestBit(bQueen, index))  SetBit(bQueen, index); 
        if(TestBit(bKing, index))  SetBit(bKing, index); 
    };

    void GetwBoardandSetPromoIndex(int index, char promoP) {
        switch(promoP){
            case 'q': SetBit(wQueen, index);return;
            case 'r': SetBit(wRook, index);return;
            case 'b': SetBit(wBishop, index);return;
            case 'n': SetBit(wKnight, index);return;
        } 
    };

    void GetbBoardandSetPromoIndex(int index, char promoP) {
        switch(promoP){
            case 'q': SetBit(bQueen, index);return;
            case 'r': SetBit(bRook, index);return;
            case 'b': SetBit(bBishop, index);return;
            case 'n': SetBit(bKnight, index);return;
        } 
    };

    void GetBoardandSetIndex(int index) {
        if(isWhiteMove) return GetwBoardandSetIndex(index); 
        return GetbBoardandSetIndex(index); 
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
        if(isWhiteMove) return GetwBoardandSetPromoIndex(index, promoP); 
        return GetbBoardandSetPromoIndex(index, promoP); 
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
        if(TestBit(bPawn, index)) return bPawn; 
        if(TestBit(bKnight, index)) return bKnight;
        if(TestBit(bBishop, index)) return bBishop;
        if(TestBit(bRook, index)) return bRook;
        if(TestBit(bQueen, index)) return bQueen;
        if(TestBit(bKing, index)) return bKing;
        return C64(0);
    };

    U64 GetMovesByBoard(int index) {
        if(isWhiteMove) return GetwBoardMoves(index);
        return GetbBoardMoves(index);
    };

    //making moves 
    bool MakeMove(string move) {
        //determine board and moves
        string startSquare = move.substr(0, 2);
        string targetSquare = move.substr(2, 2);
        int startIndex = StringtoIndex(startSquare);
        int targetIndex = StringtoIndex(targetSquare);

        U64 movesBoard = GetMovesByBoard(startIndex);
        if(movesBoard == C64(0) || !TestBit(movesBoard, targetIndex)) return false;

        //test if promotion
        U64 allb = AllBoard();
        bool isPromotion = move.length() == 5;
        bool isCaptureMove = TestBit(allb, targetIndex);
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