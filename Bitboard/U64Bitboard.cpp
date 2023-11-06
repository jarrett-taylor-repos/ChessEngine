#include "..\Extensions\U64Extentions.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
using namespace U64Extensions;     

class U64Bitboard {
    private:
    U64 bb[12];
    U64 occ[3];

    U64 zobrist;

    int wKingSq;
    int bKingSq;

    int halfMoveClock;
    int fullTurnNum;
    int enPassantTarget;
    int castlingRights;
    bool isWhiteMove;
    bool isMoveRepition;
    int materialValue;

    map<U64, int> zobristFenHash;

    public:

    U64Bitboard() { ClearBoard(); };
    U64Bitboard(string fen) { LoadFen(fen); };

    U64Bitboard& operator=(const U64Bitboard& other) {
        // Copy the scalar members
        this->halfMoveClock = other.halfMoveClock;
        this->fullTurnNum = other.fullTurnNum;
        this->enPassantTarget = other.enPassantTarget;
        this->isWhiteMove = other.isWhiteMove;
        this->materialValue = other.materialValue;
        this->isMoveRepition = other.isMoveRepition;
        this->castlingRights = other.castlingRights;

        // Copy the map members
        this->zobristFenHash = other.zobristFenHash;

        // Copy King squarea
        this->wKingSq = other.wKingSq;
        this->bKingSq = other.bKingSq;

        // Copy the U64 members
        for(int i = P; i < k; i++) this->bb[i] = other.bb[i];
        for(int i = WHITE; i < BOTH; i++) this->bb[i] = other.bb[i]; 

        this->zobrist = other.zobrist;
    };

    void SetBoardToCopy(const U64Bitboard& other) {
        // Copy the scalar members
        this->halfMoveClock = other.halfMoveClock;
        this->fullTurnNum = other.fullTurnNum;
        this->enPassantTarget = other.enPassantTarget;
        this->isWhiteMove = other.isWhiteMove;
        this->materialValue = other.materialValue;
        this->isMoveRepition = other.isMoveRepition;
        this->castlingRights = other.castlingRights;

        // Copy the map members
        this->zobristFenHash = other.zobristFenHash;

        // Copy King squarea
        this->wKingSq = other.wKingSq;
        this->bKingSq = other.bKingSq;

        // Copy the U64 members
        for(int i = P; i < k; i++) this->bb[i] = other.bb[i];
        for(int i = WHITE; i < BOTH; i++) this->bb[i] = other.bb[i]; 

        this->zobrist = other.zobrist;
    };

    U64Bitboard CopyBoard() { return *this; };
    
    void LoadFenHelper(vector<string> arguments) {
        ClearBoard();
        isMoveRepition = false;
        materialValue = 0;
        string moveColor = arguments[1];
        isWhiteMove = moveColor == "w";
        castlingRights = SetCastlingRights(arguments[2]);
        enPassantTarget = (arguments[3] == "-") ? 0 : StringtoIndex(arguments[3]);
        fullTurnNum = stoi(arguments[4]);
        halfMoveClock = stoi(arguments[5]);

        SetZobristHash(zobrist, enPassantTarget);
        SetZobristHash(zobrist, isWhiteMove);
    };

    void SetBoard(char c, int sq) {
        SetBit(occ[BOTH], sq);
        SetZobristHash(zobrist, sq, c);
        switch(c) {
            case 'p': SetBit(bb[p], sq); SetBit(occ[BLACK], sq); AddMaterialValue(c); return;
            case 'b': SetBit(bb[b], sq); SetBit(occ[BLACK], sq); AddMaterialValue(c); return;
            case 'n': SetBit(bb[n], sq); SetBit(occ[BLACK], sq); AddMaterialValue(c); return;
            case 'r': SetBit(bb[r], sq); SetBit(occ[BLACK], sq); AddMaterialValue(c); return;
            case 'q': SetBit(bb[q], sq); SetBit(occ[BLACK], sq); AddMaterialValue(c); return;
            case 'k': SetBit(bb[k], sq); SetBit(occ[BLACK], sq); AddMaterialValue(c); bKingSq = sq; return;
            case 'P': SetBit(bb[P], sq); SetBit(occ[WHITE], sq); AddMaterialValue(c); return;
            case 'B': SetBit(bb[B], sq); SetBit(occ[WHITE], sq); AddMaterialValue(c); return;
            case 'N': SetBit(bb[N], sq); SetBit(occ[WHITE], sq); AddMaterialValue(c); return;
            case 'R': SetBit(bb[R], sq); SetBit(occ[WHITE], sq); AddMaterialValue(c); return;
            case 'Q': SetBit(bb[Q], sq); SetBit(occ[WHITE], sq); AddMaterialValue(c); return;
            case 'K': SetBit(bb[K], sq); SetBit(occ[WHITE], sq); AddMaterialValue(c); wKingSq = sq; return;
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
        vector<string> fenArgs = Split(fen);
        string currFen = fenArgs[0];
        LoadFenHelper(fenArgs);

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

        UpdateAndCheckZobristHash(zobristFenHash, zobrist);
    };

    string GetFenHelper() {
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

            if(TestBit(occ[BOTH], i)) {
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
        string moveColor = isWhiteMove ? " w " : " b ";
        string fen = GetFenHelper() + moveColor + CastlingRightsString(castlingRights) +" "+ EnpassantTargetToString(enPassantTarget) +" "+ to_string(halfMoveClock) +" "+ to_string(fullTurnNum);
        return fen;
    };

    string GetPieceAtIndex(int index) {
        if(TestBit(bb[P], index)) return "P";
        if(TestBit(bb[K], index)) return "N";
        if(TestBit(bb[B], index)) return "B";
        if(TestBit(bb[R], index)) return "R";
        if(TestBit(bb[Q], index)) return "Q";
        if(TestBit(bb[K], index)) return "K";

        if(TestBit(bb[p], index)) return "p";
        if(TestBit(bb[n], index)) return "n";
        if(TestBit(bb[b], index)) return "b";
        if(TestBit(bb[r], index)) return "r";
        if(TestBit(bb[q], index)) return "q";
        if(TestBit(bb[k], index)) return "k";
        return "";
    };

    int GetValueAtIndex(int index) {
        if(TestBit(bb[P], index)) return 1;
        if(TestBit(bb[K], index)) return 3;
        if(TestBit(bb[B], index)) return 3;
        if(TestBit(bb[R], index)) return 5;
        if(TestBit(bb[Q], index)) return 9;
        if(TestBit(bb[K], index)) return 1000;

        if(TestBit(bb[p], index)) return -1;
        if(TestBit(bb[n], index)) return -3;
        if(TestBit(bb[b], index)) return -3;
        if(TestBit(bb[r], index)) return -5;
        if(TestBit(bb[q], index)) return -9;
        if(TestBit(bb[k], index)) return -1000;
        return 0;
    };

    int GetAbsValueAtIndex(int index) {
        if(TestBit(bb[P], index)) return 1;
        if(TestBit(bb[K], index)) return 3;
        if(TestBit(bb[B], index)) return 3;
        if(TestBit(bb[R], index)) return 5;
        if(TestBit(bb[Q], index)) return 9;
        if(TestBit(bb[K], index)) return 1000;

        if(TestBit(bb[p], index)) return 1;
        if(TestBit(bb[n], index)) return 3;
        if(TestBit(bb[b], index)) return 3;
        if(TestBit(bb[r], index)) return 5;
        if(TestBit(bb[q], index)) return 9;
        if(TestBit(bb[k], index)) return 1000;
        return 0;
    };

    int GetMaterialValue() { return materialValue; };
    string GetCastlingRights() { return CastlingRightsString(castlingRights); };

    void ClearBoard() {
        memset(bb, 0, sizeof(bb));
        memset(occ, 0, sizeof(occ));

        Reset(zobrist); zobristFenHash.clear();
        wKingSq = -1; bKingSq = -1;
    };

    U64 GetwPawn() { return bb[P]; };
    U64 GetwKnight() { return bb[K]; };
    U64 GetwBishop() { return bb[B]; };
    U64 GetwRook() { return bb[R]; };
    U64 GetwQueen() { return bb[Q]; };
    U64 GetwKing() { return bb[K]; };
    U64 GetbPawn() { return bb[p]; };
    U64 GetbKnight() { return bb[n]; };
    U64 GetbBishop() { return bb[b]; };
    U64 GetbRook() { return bb[r]; };
    U64 GetbQueen() { return bb[q]; };
    U64 GetbKing() { return bb[k]; };

    U64 GetwBoard() { return occ[WHITE]; };
    U64 GetbBoard() { return occ[BLACK]; };
    U64 GetOccBoard() { return occ[BOTH]; };

    U64 EmptyBoard() { return ~occ[BOTH]; };
    U64 NotbBoard() { return ~occ[BLACK]; };
    U64 NotwBoard() { return ~occ[WHITE]; };

    U64 GetOpponentBoard() {return isWhiteMove ? occ[BLACK] : occ[WHITE]; };
    U64 GetZobrist() { return zobrist; };

    bool IsWhiteMove() { return isWhiteMove; };
    int GetMoveMultiplier() { return (isWhiteMove) ? 1 : -1; };

    U64 GetPawn() { return (isWhiteMove) ? bb[P] : bb[p]; };
    U64 GetKnight() { return (isWhiteMove) ? bb[K] : bb[n]; };
    U64 GetBishop() { return (isWhiteMove) ? bb[B] : bb[b]; };
    U64 GetRook() { return (isWhiteMove) ? bb[R] : bb[r]; };
    U64 GetQueen() { return (isWhiteMove) ? bb[Q] : bb[q]; };
    U64 GetKing() { return (isWhiteMove) ? bb[K] : bb[k]; };

    //board functions
    U64 AllKing() { return bb[K] | bb[k]; };

    //generic movement 
    U64 northOne(U64 b) { return b >> 8; };
    U64 southOne(U64 b) { return b << 8; };
    U64 eastOne(U64 b) { return b << 1 & notAFile; };
    U64 westOne(U64 b) { return b >> 1 & notHFile; };
    U64 northEastOne(U64 b) { return b >> 7 & notAFile; };
    U64 northWestOne(U64 b) { return b >> 9 & notHFile; };
    U64 southEastOne(U64 b) { return b << 9 & notAFile; };
    U64 southWestOne(U64 b) { return b << 7 & notHFile; };
    bool InBounds(int offset) { return (offset >= 0) && (offset < 64); };

    U64 GetMovesFromPieceAttacks(U64 attacks, U64 unableToCapture) { return attacks & ~(attacks & unableToCapture); };

    //pawn functions
    U64 wSinglePushTargets(U64 b) { return northOne(b) & EmptyBoard(); };
    U64 wDoublePushTargets(U64 b) { return northOne(wSinglePushTargets(b)) & EmptyBoard() & rank4; };
    U64 bSinglePushTargets(U64 b) { return southOne(b) & EmptyBoard(); };
    U64 bDoublePushTargets(U64 b) { return southOne(bSinglePushTargets(b)) & EmptyBoard() & rank5; };
    U64 wPawnPushes(U64 b) { return wSinglePushTargets(b) | wDoublePushTargets(b); };
    U64 bPawnPushes(U64 b) { return bSinglePushTargets(b) | bDoublePushTargets(b); };

    U64 wPawnWestAtt(U64 b) { return (b >> 9) & notHFile; };
    U64 wPawnEastAtt(U64 b) { return (b >> 7) & notAFile; };
    U64 wPawnAllAtt(U64 b) { return wPawnEastAtt(b) | wPawnWestAtt(b); };
    U64 bPawnWestAtt(U64 b) { return (b << 9) & notAFile; };
    U64 bPawnEastAtt(U64 b) { return (b << 7) & notHFile; };
    U64 bPawnAllAtt(U64 b) { return bPawnEastAtt(b) | bPawnWestAtt(b); };
    U64 wPawnWestCaptures(U64 b) { return wPawnWestAtt(b) & (occ[BLACK] | precomputtedSingleBit[enPassantTarget]); };
    U64 wPawnEastCaptures(U64 b) { return wPawnEastAtt(b) & (occ[BLACK] | precomputtedSingleBit[enPassantTarget]); };
    U64 wPawnAllCaptures(U64 b) { return wPawnEastCaptures(b) | wPawnWestCaptures(b); };
    U64 bPawnWestCaptures(U64 b) { return bPawnWestAtt(b) & (occ[WHITE] | precomputtedSingleBit[enPassantTarget]); };
    U64 bPawnEastCaptures(U64 b) { return bPawnEastAtt(b) & (occ[WHITE] | precomputtedSingleBit[enPassantTarget]); };
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

    U64 wKnightAtt(){ return KnightAttacks(bb[K]); };
    U64 bKnightAtt(){ return KnightAttacks(bb[n]); };

    U64 wKnightPsuedoMoves(){ return KnightAttacks(bb[K]) & NotwBoard(); };
    U64 bKnightPsuedoMoves(){ return KnightAttacks(bb[n]) & NotbBoard(); };

    //king moves, castling needs to include possible castle through check 
    U64 wKingCastleShort() {
        bool canCastle = castlingRights & wk;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], f1) || TestBit(occ[BOTH], g1);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(f1) || isSquareAttacked(g1);
        if(isAttacked) return Empty;
        return precomputtedSingleBit[g1];
    };

    U64 wKingCastleLong() {
        bool canCastle = castlingRights & wq;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], b1) || TestBit(occ[BOTH], c1) || TestBit(occ[BOTH], d1);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(b1) || isSquareAttacked(c1)|| isSquareAttacked(d1);
        if(isAttacked) return Empty;
        return precomputtedSingleBit[c1];
    };

    U64 wKingCastle() { return wKingCastleShort() | wKingCastleLong(); };

    U64 bKingCastleShort() {
        bool canCastle = castlingRights & bk;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], f8) || TestBit(occ[BOTH], g8);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(f8) || isSquareAttacked(g8);
        if(isAttacked) return Empty;
        return precomputtedSingleBit[g8];
    };

    U64 bKingCastleLong() {
        bool canCastle = castlingRights & bq;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], b8) || TestBit(occ[BOTH], c8) || TestBit(occ[BOTH], d8);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(b8) || isSquareAttacked(c8) || isSquareAttacked(d8);
        if(isAttacked) return Empty;
        return precomputtedSingleBit[c8];
    };

    U64 bKingCastle() { return bKingCastleShort() | bKingCastleLong(); };

    U64 wKingPsuedoMoves() { return precomputtedKings[wKingSq] | wKingCastle(); };
    U64 bKingPsuedoMoves() { return precomputtedKings[bKingSq] | bKingCastle(); };

    U64 wKingAtt() { return precomputtedKings[wKingSq]; };
    U64 bKingAtt() { return precomputtedKings[bKingSq]; };

    //sliding moves on the fly 
    U64 BishopAttacksOnTheFly(int sq, U64 blocks) {
        U64 attacks = Empty;
        int file,rank;

        int targetRank = sq/8;
        int targetFile = sq%8;

        //south east 
        for(rank = targetRank + 1, file = targetFile + 1; rank <= 7 && file <= 7; rank++, file++) {
            U64 bit = precomputtedSingleBit[rank*8 + file];
            attacks |= bit;
            if(bit & blocks) break;
        }

        //north east 
        for(rank = targetRank - 1, file = targetFile + 1; rank >= 0 && file <= 7; rank--, file++) {
            U64 bit = precomputtedSingleBit[rank*8 + file];
            attacks |= bit;
            if(bit & blocks) break;
        }

        //south west 
        for(rank = targetRank + 1, file = targetFile - 1; rank <= 7 && file >= 0; rank++, file--) {
            U64 bit = precomputtedSingleBit[rank*8 + file];
            attacks |= bit;
            if(bit & blocks) break;
        }

        //north west
        for(rank = targetRank - 1, file = targetFile - 1; rank >= 0 && file >= 0; rank--, file--) {
            U64 bit = precomputtedSingleBit[rank*8 + file];
            attacks |= bit;
            if(bit & blocks) break;
        }
        return attacks;
    }

    U64 RookAttacksOnTheFly(int sq, U64 blocks) {
        U64 attacks = Empty;
        int file,rank;

        int targetRank = sq/8;
        int targetFile = sq%8;

        for(rank = targetRank + 1; rank <=7; rank++) {
            U64 bit = precomputtedSingleBit[rank*8 + targetFile];
            attacks |= bit;
            if(bit & blocks) break;
        }
        for(rank = targetRank - 1; rank >=0; rank--) {
            U64 bit = precomputtedSingleBit[rank*8 + targetFile];
            attacks |= bit;
            if(bit & blocks) break;
        }
        for(file = targetFile + 1; file <=7; file++) {
            U64 bit = precomputtedSingleBit[targetRank*8 + file];
            attacks |= bit;
            if(bit & blocks) break;
        }
        for(file = targetFile - 1; file >=0; file--) {
            U64 bit = precomputtedSingleBit[targetRank*8 + file];
            attacks |= bit;
            if(bit & blocks) break;
        }
        return attacks;
    }

    //sliding moves
    U64 MagicBishopAttacks(U64 bishop, U64 occupied) {
        U64 moves = Empty;
        U64 temp = bishop;
        while(temp != Empty) {
            int sq = GetLSBIndex(temp);
            PopBit(temp, sq);
            moves |= GetBishopAttacks(sq, occupied);
        }
        return moves;
    };
    U64 wMagicBishopPsuedoMoves() { U64 wbatt = MagicBishopAttacks(bb[B], occ[BOTH]); return GetMovesFromPieceAttacks(wbatt, occ[WHITE]); };
    U64 bMagicBishopPsuedoMoves() { U64 bbatt = MagicBishopAttacks(bb[b], occ[BOTH]); return GetMovesFromPieceAttacks(bbatt, occ[BLACK]); };

    U64 MagicRookAttacks(U64 rook, U64 occupied) {
        U64 moves = Empty;
        U64 temp = rook;
        while(temp != Empty) {
            int sq = GetLSBIndex(temp);
            PopBit(temp, sq);
            moves |= GetBishopAttacks(sq, occupied);
        }
        return moves;
    };
    U64 wMagicRookPsuedoMoves() { U64 wratt = MagicRookAttacks(bb[R], occ[BOTH]); return GetMovesFromPieceAttacks(wratt, occ[WHITE]);};
    U64 bMagicRookPsuedoMoves() { U64 bratt = MagicRookAttacks(bb[r], occ[BOTH]); return GetMovesFromPieceAttacks(bratt, occ[BLACK]); };

    U64 MagicQueenAttacks(U64 queen, U64 occupied) { 
        U64 moves = Empty;
        U64 temp = queen;
        while(temp != Empty) {
            int sq = GetLSBIndex(temp);
            PopBit(temp, sq);
            moves |= GetQueenAttacks(sq, occupied);
        }
        return moves;  
    };
    U64 wMagicQueenPsuedoMoves() { U64 wqatt = MagicQueenAttacks(bb[Q], occ[BOTH]); return GetMovesFromPieceAttacks(wqatt, occ[WHITE]); };
    U64 bMagicQueenPsuedoMoves() { U64 bqatt = MagicQueenAttacks(bb[q], occ[BOTH]); return GetMovesFromPieceAttacks(bqatt, occ[BLACK]); };

    U64 MagicBishopPsuedoMoves() { return isWhiteMove ? wMagicBishopPsuedoMoves() : bMagicBishopPsuedoMoves(); };
    U64 MagicRookPsuedoMoves() { return isWhiteMove ? wMagicRookPsuedoMoves() : bMagicRookPsuedoMoves(); };
    U64 MagicQueenPsuedoMoves() { return isWhiteMove ? wMagicQueenPsuedoMoves() : bMagicQueenPsuedoMoves(); };


    //Moves
    void GenerateWhiteMoves(Moves &movesList) {
        int source, target;
        U64 board, att, moves;

        //pawn pushes
        board = bb[P];
        while(board != Empty) {
            source = GetLSBIndex(board);
            moves = wPawnPushes(precomputtedSingleBit[source]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, P, true, false, enPassantTarget, Empty);
            PopBit(board, source);
        }

        //pawn captures 
        board = bb[P];
        while(board != Empty) {
            source = GetLSBIndex(board);
            moves = wPawnAllCaptures(precomputtedSingleBit[source]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, P, true, true, 0, occ[BLACK]);
            PopBit(board, source);
        }

        //knight moves
        board = bb[K];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = precomputtedKnights[source];
            moves = GetMovesFromPieceAttacks(att, occ[WHITE]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, N, true, false, 0, occ[BLACK]);
            PopBit(board, source);
        }

        //bishop moves
        board = bb[B];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = GetBishopAttacks(source, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, occ[WHITE]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, B, true, false, 0, occ[BLACK]);
            PopBit(board, source);
        }

        //rook moves
        board = bb[R];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = GetRookAttacks(source, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, occ[WHITE]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, R, true, false, 0, occ[BLACK]);
            PopBit(board, source);
        }

        //queen moves
        board = bb[Q];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = GetQueenAttacks(source, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, occ[WHITE]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, Q, true, false, 0, occ[BLACK]);
            PopBit(board, source);
        }

        //king moves
        att = wKingPsuedoMoves();
        moves = GetMovesFromPieceAttacks(att, occ[WHITE]);
        if(moves != Empty) FindAndInsertMoves(movesList, wKingSq, moves, K, true, false, 0, occ[BLACK]);
    };

    void GenerateBlackMoves(Moves &movesList) {
        int source, target;
        U64 board, att, moves;

        //pawn pushes
        board = bb[p];
        while(board != Empty) {
            source = GetLSBIndex(board);
            moves = bPawnPushes(precomputtedSingleBit[source]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, p, true, false, enPassantTarget, Empty);
            PopBit(board, source);
        }

        //pawn captures 
        board = bb[p];
        while(board != Empty) {
            source = GetLSBIndex(board);
            moves = bPawnAllCaptures(precomputtedSingleBit[source]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, p, false, true, 0, occ[WHITE]);
            PopBit(board, source);
        }

        //knight moves
        board = bb[n];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = precomputtedKnights[source];
            moves = GetMovesFromPieceAttacks(att, occ[BLACK]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, n, false, false, 0, occ[WHITE]);
            PopBit(board, source);
        }

        //bishop moves
        board = bb[b];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = GetBishopAttacks(source, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, occ[BLACK]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, b, false, false, 0, occ[WHITE]);
            PopBit(board, source);
        }

        //rook moves
        board = bb[r];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = GetRookAttacks(source, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, occ[BLACK]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, r, false, false, 0, occ[WHITE]);
            PopBit(board, source);
        }

        //queen moves
        board = bb[q];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = GetQueenAttacks(source, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, occ[BLACK]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, q, false, false, 0, occ[WHITE]);
            PopBit(board, source);
        }

        //king moves
        att = bKingPsuedoMoves();
        moves = GetMovesFromPieceAttacks(att, occ[BLACK]);
        if(moves != Empty) FindAndInsertMoves(movesList, bKingSq, moves, k, false, false, 0, occ[WHITE]);
    };

    void GenerateMoves(Moves &movesList) { (isWhiteMove) ? GenerateWhiteMoves(movesList): GenerateBlackMoves(movesList); };

    void GenerateWhitePawnMoves(Moves &movesList) {
        int source, target;
        U64 board, att, moves;

        //pawn pushes
        board = bb[P];
        while(board != Empty) {
            source = GetLSBIndex(board);
            moves = wPawnPushes(precomputtedSingleBit[source]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, P, true, false, 0, Empty);
            PopBit(board, source);
        }

        //pawn captures 
        if(wPawnAllCaptures(bb[P]) == Empty) return;

        board = bb[P];
        while(board != Empty) {
            source = GetLSBIndex(board);
            moves = wPawnAllCaptures(precomputtedSingleBit[source]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, P, true, true, enPassantTarget, occ[BLACK]);
            PopBit(board, source);
        }
    }

    void GenerateWhiteKingMoves(Moves &movesList) {
        int source, target;
        U64 board, att, moves;

        //king moves
        att = wKingPsuedoMoves();
        moves = GetMovesFromPieceAttacks(att, occ[WHITE]);
        if(moves != Empty) FindAndInsertMoves(movesList, wKingSq, moves, K, true, false, 0, occ[BLACK]);
    }

    void GenerateBlackPawnMoves(Moves &movesList) {
        int source, target;
        U64 board, att, moves;

        //pawn pushes
        board = bb[p];
        while(board != Empty) {
            source = GetLSBIndex(board);
            moves = bPawnPushes(precomputtedSingleBit[source]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, p, true, false, 0, Empty);
            PopBit(board, source);
        }

        //pawn captures 
        if(bPawnAllCaptures(bb[p]) == Empty) return;

        board = bb[p];
        while(board != Empty) {
            source = GetLSBIndex(board);
            moves = bPawnAllCaptures(precomputtedSingleBit[source]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, p, false, true, enPassantTarget, occ[WHITE]);
            PopBit(board, source);
        }
    }

    void GenerateBlackKingMoves(Moves &movesList) {
        int source, target;
        U64 board, att, moves;

        //king moves
        att = bKingPsuedoMoves();
        moves = GetMovesFromPieceAttacks(att, occ[BLACK]);
        if(moves != Empty) FindAndInsertMoves(movesList, bKingSq, moves, k, false, false, 0, occ[WHITE]);
    }

    void GenerareKnightMoves(Moves &movesList, bool isWhiteToMove) {
        int source, target;
        U64 board, att, moves;

        int piece = isWhiteToMove ? N : n;
        U64 unableToCapture = isWhiteToMove ? occ[WHITE] : occ[BLACK];
        U64 ableToCapture = isWhiteToMove ? occ[BLACK] : occ[WHITE];
        //knight moves
        board = bb[piece];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = precomputtedKnights[source];
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, n, false, false, 0, ableToCapture);
            PopBit(board, source);
        }
    } 

    void GenerateBishopMoves(Moves &movesList, bool isWhiteToMove) { 
        int source, target;
        U64 board, att, moves;

        int piece = isWhiteToMove ? B : b;
        U64 unableToCapture = isWhiteToMove ? occ[WHITE] : occ[BLACK];
        U64 ableToCapture = isWhiteToMove ? occ[BLACK] : occ[WHITE];

        //bishop moves
        board = bb[piece];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = GetBishopAttacks(source, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, piece, false, false, 0, ableToCapture);
            PopBit(board, source);
        }
    }

    void GenerateRookMoves(Moves &movesList, bool isWhiteToMove) { 
        int source, target;
        U64 board, att, moves;

        int piece = isWhiteToMove ? R : r;
        U64 unableToCapture = isWhiteToMove ? occ[WHITE] : occ[BLACK];
        U64 ableToCapture = isWhiteToMove ? occ[BLACK] : occ[WHITE];

        //bishop moves
        board = bb[piece];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = GetRookAttacks(source, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, piece, false, false, 0, ableToCapture);
            PopBit(board, source);
        }
    }

    void GenerateQueenMoves(Moves &movesList, bool isWhiteToMove) { 
        int source, target;
        U64 board, att, moves;

        int piece = isWhiteToMove ? Q : q;
        U64 unableToCapture = isWhiteToMove ? occ[WHITE] : occ[BLACK];
        U64 ableToCapture = isWhiteToMove ? occ[BLACK] : occ[WHITE];

        //bishop moves
        board = bb[piece];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = GetQueenAttacks(source, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, piece, false, false, 0, ableToCapture);
            PopBit(board, source);
        }
    }

    void GeneratebbMoves(Moves &movesList) {
        movesList.Clear();

        if(isWhiteMove) {
            GenerateWhitePawnMoves(movesList);
            GenerateWhiteKingMoves(movesList);
        } else {
            GenerateBlackPawnMoves(movesList);
            GenerateBlackKingMoves(movesList);
        }
        
        GenerareKnightMoves(movesList, isWhiteMove);
        GenerateBishopMoves(movesList, isWhiteMove);
        GenerateRookMoves(movesList, isWhiteMove);
        GenerateQueenMoves(movesList, isWhiteMove);

    }

    //attacks
    bool BlackAttacksSquare(int sq) {
        if(precomputtedWhitePawnAttacks[sq] & bb[p]) return true;
        if(precomputtedKnights[sq] & bb[n]) return true;
        if(precomputtedKings[sq] & bb[k]) return true;
        if(GetBishopAttacks(sq, occ[BOTH]) & bb[b]) return true;
        if(GetRookAttacks(sq, occ[BOTH]) & bb[r]) return true;
        if(GetQueenAttacks(sq, occ[BOTH]) & bb[q]) return true;
        return false;
    };

    bool WhiteAttacksSquare(int sq) {
        if(precomputtedBlackPawnAttacks[sq] & bb[P]) return true;
        if(precomputtedKnights[sq] & bb[K]) return true;
        if(precomputtedKings[sq] & bb[K]) return true;
        if(GetBishopAttacks(sq, occ[BOTH]) & bb[B]) return true;
        if(GetRookAttacks(sq, occ[BOTH]) & bb[R]) return true;
        if(GetQueenAttacks(sq, occ[BOTH]) & bb[Q]) return true;
        return false;
    };

    bool isSquareAttacked(int sq) { return isWhiteMove ? BlackAttacksSquare(sq) : WhiteAttacksSquare(sq); };

    void PrintAttackedSquares() {
        for(int i = 0; i < 64; i++) {
            if(i != 0 && (i % 8) == 0) cout << endl; 
            string value = isSquareAttacked(i) ? "1 " :  ". "; 
            cout << value;
        }
        cout << endl << endl;
    };

    //make move helpers
    void GetwBoardandResetIndex(int index) {
        PopBit(occ[WHITE], index); PopBit(occ[BOTH], index);
        if(TestBit(bb[P], index)) { PopBit(bb[P], index); RemoveMaterialValue('P'); SetZobristHash(zobrist, index,P); return;}
        if(TestBit(bb[K], index)) { PopBit(bb[K], index); RemoveMaterialValue('N'); SetZobristHash(zobrist, index,N); return;}
        if(TestBit(bb[B], index)) { PopBit(bb[B], index); RemoveMaterialValue('B'); SetZobristHash(zobrist, index,B);return;} 
        if(TestBit(bb[R], index)) { PopBit(bb[R], index); RemoveMaterialValue('R'); SetZobristHash(zobrist, index,R);return;}
        if(TestBit(bb[Q], index)) { PopBit(bb[Q], index); RemoveMaterialValue('Q'); SetZobristHash(zobrist, index,Q);return;}
        if(TestBit(bb[K], index)) { PopBit(bb[K], index); RemoveMaterialValue('K'); SetZobristHash(zobrist, index,K);return;} 
    };

    void GetbBoardandResetIndex(int index) {
        PopBit(occ[BLACK], index); PopBit(occ[BOTH], index);
        if(TestBit(bb[p], index))  { PopBit(bb[p], index); RemoveMaterialValue('p'); SetZobristHash(zobrist, index,p); return; } 
        if(TestBit(bb[n], index))  { PopBit(bb[n], index); RemoveMaterialValue('n'); SetZobristHash(zobrist, index,n); return; } 
        if(TestBit(bb[b], index))  { PopBit(bb[b], index); RemoveMaterialValue('b'); SetZobristHash(zobrist, index,b); return; } 
        if(TestBit(bb[r], index))  { PopBit(bb[r], index); RemoveMaterialValue('q'); SetZobristHash(zobrist, index,r); return; } 
        if(TestBit(bb[q], index))  { PopBit(bb[q], index); RemoveMaterialValue('q'); SetZobristHash(zobrist, index,q); return; }  
        if(TestBit(bb[k], index))  { PopBit(bb[k], index); RemoveMaterialValue('k'); SetZobristHash(zobrist, index,k); return; } 
    };

    void GetwBoardResetStartandSetTarget(int start, int target) {
        PopBit(occ[WHITE], start); SetBit(occ[WHITE], target);
        PopBit(occ[BOTH], start); SetBit(occ[BOTH], target);
        if(TestBit(bb[P], start)) { PopBit(bb[P], start); SetBit(bb[P], target); SetZobristHash(zobrist, start,P); SetZobristHash(zobrist, target,P);return;}
        if(TestBit(bb[K], start)) { PopBit(bb[K], start); SetBit(bb[K], target); SetZobristHash(zobrist, start,N); SetZobristHash(zobrist, target,N); return;}
        if(TestBit(bb[B], start)) { PopBit(bb[B], start); SetBit(bb[B], target); SetZobristHash(zobrist, start,B); SetZobristHash(zobrist, target,B); return;}
        if(TestBit(bb[R], start)) { PopBit(bb[R], start); SetBit(bb[R], target); SetZobristHash(zobrist, start,R); SetZobristHash(zobrist, target,R);return;}
        if(TestBit(bb[Q], start)) { PopBit(bb[Q], start); SetBit(bb[Q], target); SetZobristHash(zobrist, start,Q); SetZobristHash(zobrist, target,Q);return;}
        if(TestBit(bb[K], start)) { PopBit(bb[K], start); SetBit(bb[K], target); SetZobristHash(zobrist, start,K); SetZobristHash(zobrist, target,K);return;}
    };

    void GetbBoardResetStartandSetTarget(int start, int target) {
        PopBit(occ[BLACK], start); SetBit(occ[BLACK], target);
        PopBit(occ[BOTH], start); SetBit(occ[BOTH], target);
        if(TestBit(bb[p], start)) { PopBit(bb[p], start); SetBit(bb[p], target); SetZobristHash(zobrist, start,p); SetZobristHash(zobrist, target,p);return;}
        if(TestBit(bb[n], start)) { PopBit(bb[n], start); SetBit(bb[n], target); SetZobristHash(zobrist, start,n); SetZobristHash(zobrist, target,n);return;}
        if(TestBit(bb[b], start)) { PopBit(bb[b], start); SetBit(bb[b], target); SetZobristHash(zobrist, start,b); SetZobristHash(zobrist, target,b);return;}
        if(TestBit(bb[r], start)) { PopBit(bb[r], start); SetBit(bb[r], target); SetZobristHash(zobrist, start,r); SetZobristHash(zobrist, target,r);return;}
        if(TestBit(bb[q], start)) { PopBit(bb[q], start); SetBit(bb[q], target); SetZobristHash(zobrist, start,q); SetZobristHash(zobrist, target,q);return;}
        if(TestBit(bb[k], start)) { PopBit(bb[k], start); SetBit(bb[k], target); SetZobristHash(zobrist, start,k); SetZobristHash(zobrist, target,k);return;}
    };

    void GetwBoardandSetPromoIndex(int index, int promoP) {
        SetBit(occ[WHITE], index); SetBit(occ[BOTH], index);
        switch(promoP){
            case 1: SetBit(bb[Q], index); AddMaterialValue('Q'); SetZobristHash(zobrist, index,Q); return;
            case 2: SetBit(bb[R], index); AddMaterialValue('R'); SetZobristHash(zobrist, index,R); return;
            case 3: SetBit(bb[B], index); AddMaterialValue('B'); SetZobristHash(zobrist, index,B); return;
            case 4: SetBit(bb[K], index); AddMaterialValue('N'); SetZobristHash(zobrist, index,N); return;
        } 
    };

    void GetbBoardandSetPromoIndex(int index, int promoP) {
        SetBit(occ[BLACK], index); SetBit(occ[BOTH], index);
        switch(promoP){
            case 1: SetBit(bb[q], index); AddMaterialValue('q'); SetZobristHash(zobrist, index,q); return;
            case 2: SetBit(bb[r], index); AddMaterialValue('r'); SetZobristHash(zobrist, index,r); return;
            case 3: SetBit(bb[b], index); AddMaterialValue('b'); SetZobristHash(zobrist, index,b); return;
            case 4: SetBit(bb[n], index); AddMaterialValue('n'); SetZobristHash(zobrist, index,n); return;
        } 
    };

    void GetBoardandResetIndex(int index, bool isCapture) {
        if(isWhiteMove && isCapture) return GetbBoardandResetIndex(index);
        if(isWhiteMove && !isCapture) return GetwBoardandResetIndex(index);

        if(!isWhiteMove && isCapture) return GetwBoardandResetIndex(index);
        if(!isWhiteMove && !isCapture) return GetbBoardandResetIndex(index);
    };

    void GetBoardandSetPromoIndex(int index, int promoP) { return isWhiteMove ? GetwBoardandSetPromoIndex(index, promoP) : GetbBoardandSetPromoIndex(index, promoP); };
    void GetBoardResetStartandSetTarget(int start, int target) { return isWhiteMove ? GetwBoardResetStartandSetTarget(start, target) : GetbBoardResetStartandSetTarget(start, target); };

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

    void CapturePromoUpdate(int startIndex, int targetIndex, int promoP) {
        GetBoardandResetIndex(targetIndex, true);
        GetBoardandResetIndex(startIndex, false);
        GetBoardandSetPromoIndex(targetIndex, promoP);
    };

    void QuietPromoUpdate(int startIndex, int targetIndex, int promoP) {
        GetBoardandResetIndex(startIndex, false);
        GetBoardandSetPromoIndex(targetIndex, promoP);
    };

    void PromotionUpdate(bool isCap, int startIndex, int targetIndex, int promoP) { return isCap ? CapturePromoUpdate(startIndex, targetIndex, promoP) : QuietPromoUpdate(startIndex, targetIndex, promoP); };
    
    void CastleUpdateHelper(U64 &color, U64 &b, int start, int end) { PopBit(color, start); SetBit(color, end); PopBit(b, start); SetBit(b, end); PopBit(occ[BOTH], start); SetBit(occ[BOTH], end); };
    void wCastleLongUpdate() { CastleUpdateHelper(occ[WHITE], bb[R], a1, d1); CastleUpdateHelper(occ[WHITE], bb[K], e1, c1); };
    void wCastleShortUpdate() { CastleUpdateHelper(occ[WHITE], bb[R], g1, e1); CastleUpdateHelper(occ[WHITE], bb[K], e1, g1); };

    void wCastleUpdate(int targetMinusStart) { return (targetMinusStart > 0) ? wCastleShortUpdate() : wCastleLongUpdate(); };

    void bCastleLongUpdate() { CastleUpdateHelper(occ[BLACK], bb[r], a8, d8); CastleUpdateHelper(occ[BLACK], bb[k], e8, c8); };
    void bCastleShortUpdate() { CastleUpdateHelper(occ[BLACK], bb[r], g8, e8); CastleUpdateHelper(occ[BLACK], bb[k], e8, g8); };

    void bCastleUpdate(int targetMinusStart) { return (targetMinusStart > 0) ? bCastleShortUpdate() : bCastleLongUpdate(); };
    void CastleUpdate(int targetMinusStart) { return (isWhiteMove) ? wCastleUpdate(targetMinusStart) : bCastleUpdate(targetMinusStart); };


    bool isPromotionSquare(int sq) { if(isWhiteMove) return (sq < 8); return (sq > 55); };
    bool isCapture(int targetSq) { return TestBit(occ[BOTH], targetSq); };
    bool isKingCapture(int targetSq) { U64 kingb = AllKing(); return TestBit(kingb, targetSq); };
    bool isEnpassant(int startSq, int targetSq) { return isPawnMove(startSq) && (targetSq == enPassantTarget); };
    bool isKingMove(int startSq) { U64 king = isWhiteMove ? bb[K] : bb[k]; return TestBit(king, startSq); };
    bool isCastle(int startSq, int targetSq) { bool isKing = isKingMove(startSq); bool isCastle = abs(startSq-targetSq) == 2; return isCastle && isKing; };
    bool isPawnMove(int startSq) { U64 pawn = isWhiteMove ? bb[P] : bb[p]; return TestBit(pawn, startSq); };
    int enPassantOffset(int enPassantTarget) { return isWhiteMove ? enPassantTarget + 8 : enPassantTarget - 8; };

    bool isDoublePawnMove(int startSq, int targetSq) { return isPawnMove(startSq) && (abs(startSq-targetSq) == 16); };

    void UpdateCastlingRightsFromKing() {
        if(isWhiteMove) {
            SetCastlingZobritst(castlingRights, 'Q', zobrist);
            SetCastlingZobritst(castlingRights, 'K', zobrist);
        } else {
            SetCastlingZobritst(castlingRights, 'q', zobrist);
            SetCastlingZobritst(castlingRights, 'k', zobrist);
        }
    }

    void UpdateCastlingRightsFromRook(int startSq, int targetSq) {
        if(startSq == a8 || targetSq == a8) { SetCastlingZobritst(castlingRights, 'q', zobrist); }
        if(startSq == h8 || targetSq == h8) { SetCastlingZobritst(castlingRights, 'k', zobrist); }
        if(startSq == a1 || targetSq == a1) { SetCastlingZobritst(castlingRights, 'Q', zobrist); }
        if(startSq == h1 || targetSq == h1) { SetCastlingZobritst(castlingRights, 'K', zobrist); }
    };

    //making moves 
    bool PossibleMoveIsACapture(int startSq, int targetSq) { return isCapture(targetSq) || isEnpassant(startSq, targetSq); }; 
    bool isPawn(int sq) { return isWhiteMove ? TestBit(bb[P], sq): TestBit(bb[p], sq); };

    bool MakeMove(int move) {
        U64Bitboard copy = CopyBoard();
        //move data
        int source = getMoveSource(move);
        int target = getMoveTarget(move);
        int piece = getMovePiece(move);
        int promoted = getMovePromoted(move);
        int capture = getMoveCapture(move);
        int doublePush = getMoveDouble(move);
        int enpass = getMoveEnpassant(move);
        int castling = getMoveCastling(move);

        //board updates 
        if(promoted) { PromotionUpdate(capture, source, target, promoted); }
        else if(capture) { CastleUpdate(target - source); }
        else if(enpass) { EnpassantMoveUpdate(source, target, enPassantOffset(enPassantTarget)); }
        else if(capture) { CaptureMoveUpdate(source, target); }
        else { QuietMoveUpdate(source, target); }

        //castling right update
        if (piece == K || piece == k) UpdateCastlingRightsFromKing();
        if (piece == R || piece == r) UpdateCastlingRightsFromRook(source, target);

        //update king sq
        if(piece == K) wKingSq = target;
        if(piece == k) bKingSq = target;
    
        //enpassant target 
        SetZobristHash(zobrist, enPassantTarget);
        if(doublePush && isWhiteMove) { enPassantTarget = source-8; }
        if(doublePush && !isWhiteMove) { enPassantTarget = source+8; }
        if(!doublePush) { enPassantTarget = 0; }
        SetZobristHash(zobrist, enPassantTarget);

        //move side, move num and half clock num 
        (piece == P || piece == p) || capture ? halfMoveClock = 0: halfMoveClock++;
        isWhiteMove = !isWhiteMove;
        if(isWhiteMove) { fullTurnNum++; }
        SetZobristHash(zobrist, isWhiteMove);
        isMoveRepition = UpdateAndCheckZobristHash(zobristFenHash, zobrist);
                
        //if king is in check after move
        bool isKingInCheck = isWhiteMove ? WhiteAttacksSquare(bKingSq) : BlackAttacksSquare(wKingSq);
        if (!isKingInCheck) return 1;

        SetBoardToCopy(copy);
        return 0;
    };

    //misc
    void PrintAllBoards() {
        Print(bb[P], "wPawn");
        Print(bb[p], "bPawn");
        Print(bb[K], "wKnight");
        Print(bb[n], "bKnight");
        Print(bb[B], "wBishop");
        Print(bb[b], "bBishop");
        Print(bb[R], "wRook");
        Print(bb[r], "bRook");
        Print(bb[Q], "wQueen");
        Print(bb[q], "bQueen");
        Print(bb[K], "wKing");
        Print(bb[k], "bKing");

        PrintMasterBoards();
    };

    void PrintMasterBoards() {
        Print(occ[BLACK], "bBoard");
        Print(occ[WHITE], "wBoard");
        Print(occ[BOTH], "BothBoard");
    }
};