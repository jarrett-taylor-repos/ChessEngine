#include "..\Extensions\U64Extentions.h"
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
    bool isMoveRepetition;
    int evaluation;


    U64 zobristTable[50];
    int zobristTableIndex;

    public:
    ~U64Bitboard() { ClearBoard(); };
    U64Bitboard() { ClearBoard(); };
    U64Bitboard(string fen) { LoadFen(fen); };
    U64Bitboard(const U64Bitboard& other) {
        // Copy the scalar members
        this->halfMoveClock = other.halfMoveClock;
        this->fullTurnNum = other.fullTurnNum;
        this->enPassantTarget = other.enPassantTarget;
        this->isWhiteMove = other.isWhiteMove;
        this->evaluation = other.evaluation;
        this->isMoveRepetition = other.isMoveRepetition;
        this->castlingRights = other.castlingRights;

        // Copy King squares
        this->wKingSq = other.wKingSq;
        this->bKingSq = other.bKingSq;

        // Copy the U64 members
        for(int i = P; i <= k; i++) this->bb[i] = other.bb[i];
        for(int i = WHITE; i <= BOTH; i++) this->occ[i] = other.occ[i]; 

        //zobrist
        this->zobristTableIndex = other.zobristTableIndex;
        for(int i = 0; i <= zobristTableIndex; i++) this->zobristTable[i] = other.zobristTable[i];
        this->zobrist = other.zobrist;
    };

    U64Bitboard& operator=(const U64Bitboard& other) {
        // Copy the scalar members
        this->halfMoveClock = other.halfMoveClock;
        this->fullTurnNum = other.fullTurnNum;
        this->enPassantTarget = other.enPassantTarget;
        this->isWhiteMove = other.isWhiteMove;
        this->evaluation = other.evaluation;
        this->isMoveRepetition = other.isMoveRepetition;
        this->castlingRights = other.castlingRights;

        // Copy King squares
        this->wKingSq = other.wKingSq;
        this->bKingSq = other.bKingSq;

        // Copy the U64 members
        for(int i = P; i <= k; i++) this->bb[i] = other.bb[i];
        for(int i = WHITE; i <= BOTH; i++) this->occ[i] = other.occ[i]; 

        //zobrist
        this->zobristTableIndex = other.zobristTableIndex;
        for(int i = 0; i <= zobristTableIndex; i++) this->zobristTable[i] = other.zobristTable[i];
        this->zobrist = other.zobrist;
        return *this;
    };

    //need to update for zobrist
    bool operator==(const U64Bitboard& other) {
        for(int i = P; i <= k; i++) {
            bool boardsEqual = this->bb[i] == other.bb[i];
            if(!boardsEqual) return false;
        }

        for(int i = WHITE; i <= BOTH; i++) {
            bool boardsEqual = this->occ[i] == other.occ[i]; 
            if(!boardsEqual) return false;
        }

        bool scalarsMatch = 
            this->halfMoveClock == other.halfMoveClock && 
            this->fullTurnNum == other.fullTurnNum && 
            this->enPassantTarget == other.enPassantTarget && 
            this->isWhiteMove == other.isWhiteMove && 
            this->evaluation == other.evaluation &&
            this->isMoveRepetition == other.isMoveRepetition && 
            this->castlingRights == other.castlingRights;

        bool kingSqMatch = 
            this->wKingSq == other.wKingSq && 
            this->bKingSq == other.bKingSq;

        bool zobristMatch = this->zobrist == other.zobrist;

        return scalarsMatch && kingSqMatch && zobristMatch;
    }

    void ClearBoard() {
        memset(bb, 0, sizeof(bb));
        memset(occ, 0, sizeof(occ));
        Reset(zobrist);

        ClearZobristTable();
        wKingSq = -1; bKingSq = -1;
    };

    void ClearZobristTable() {
        memset(zobristTable, 0, sizeof(zobristTable));
        zobristTableIndex = 0;
    };

    void UpdateZobristTable() {
        if(!zobristTableIndex) {
            zobristTable[zobristTableIndex] = zobrist;
            zobristTableIndex++;
            return;
        }

        for(int i = zobristTableIndex; i >= 0; i--) {
            if(zobrist == zobristTable[i]) {
                isMoveRepetition = true;
                break;
            }
        }
        if(isMoveRepetition) return;

        zobristTable[zobristTableIndex] = zobrist;
        zobristTableIndex++;
    };
    
    void LoadFenHelper(vector<string> arguments) {
        ClearBoard();
        isMoveRepetition = false;
        evaluation = 0;
        isWhiteMove = arguments[1] == "w";
        castlingRights = SetCastlingRights(arguments[2]);
        enPassantTarget = (arguments[3] == "-") ? 0 : StringtoIndex(arguments[3]);
        halfMoveClock = stoi(arguments[4]);
        fullTurnNum = stoi(arguments[5]);

        SetCastlingZobrist(zobrist, castlingRights);
        SetZobristHash(zobrist, enPassantTarget);
        SetZobristHash(zobrist, isWhiteMove);
    };

    void SetBoard(char c, int sq) {
        SetBit(occ[BOTH], sq);
        
        int piece = CharToPiece(c);

        SetBit(bb[piece], sq);
        piece > K ? SetBit(occ[BLACK], sq): SetBit(occ[WHITE], sq);

        if(piece == K) wKingSq = sq;
        if(piece == k) bKingSq = sq;
        
        AddMaterialValue(piece, sq); SetZobristHash(zobrist, sq, piece);

        if(piece == K) wKingSq = sq;
        if(piece == k) bKingSq = sq;
    }

    void AddMaterialValue(int piece, int sq) {
        evaluation += PieceValue[piece];
        evaluation += PieceSquareTables[piece][sq];
    }

    void RemoveMaterialValue(int piece, int sq) {
        evaluation -= PieceValue[piece];
        evaluation -= PieceSquareTables[piece][sq];
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
        UpdateZobristTable();
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
        if(TestBit(bb[N], index)) return "N";
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
        if(TestBit(bb[N], index)) return 3;
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
        if(TestBit(bb[N], index)) return 3;
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

    int GetEvaluation() { return evaluation; };
    string GetCastlingRights() { return CastlingRightsString(castlingRights); };

    U64 GetwPawn() { return bb[P]; };
    U64 GetwKnight() { return bb[N]; };
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
    U64* GetBitBoards() {return bb; };

    U64 GetwBoard() { return occ[WHITE]; };
    U64 GetbBoard() { return occ[BLACK]; };
    U64 GetBothBoard() { return occ[BOTH]; };
    U64* GetOccupancies() { return occ; };

    U64 EmptyBoard() { return ~occ[BOTH]; };
    U64 NotbBoard() { return ~occ[BLACK]; };
    U64 NotwBoard() { return ~occ[WHITE]; };

    U64 GetOpponentBoard() {return isWhiteMove ? occ[BLACK] : occ[WHITE]; };
    U64 GetZobrist() { return zobrist; };

    bool IsWhiteMove() { return isWhiteMove; };
    int GetMoveMultiplier() { return (isWhiteMove) ? 1 : -1; };

    U64 GetPawn() { return (isWhiteMove) ? bb[P] : bb[p]; };
    U64 GetKnight() { return (isWhiteMove) ? bb[N] : bb[n]; };
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
    U64 wPawnWestCaptures(U64 b) { return wPawnWestAtt(b) & (occ[BLACK] | (enPassantTarget == 0 ? Empty : precomputtedSingleBit[enPassantTarget]) ); };
    U64 wPawnEastCaptures(U64 b) { return wPawnEastAtt(b) & (occ[BLACK] | (enPassantTarget == 0 ? Empty : precomputtedSingleBit[enPassantTarget]) ); };
    U64 wPawnAllCaptures(U64 b) { return wPawnEastCaptures(b) | wPawnWestCaptures(b); };
    U64 bPawnWestCaptures(U64 b) { return bPawnWestAtt(b) & (occ[WHITE] | (enPassantTarget == 0 ? Empty : precomputtedSingleBit[enPassantTarget]) ); };
    U64 bPawnEastCaptures(U64 b) { return bPawnEastAtt(b) & (occ[WHITE] | (enPassantTarget == 0 ? Empty : precomputtedSingleBit[enPassantTarget]) ); };
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

    U64 wKnightAtt(){ return KnightAttacks(bb[N]); };
    U64 bKnightAtt(){ return KnightAttacks(bb[n]); };

    U64 wKnightPsuedoMoves(){ return KnightAttacks(bb[N]) & NotwBoard(); };
    U64 bKnightPsuedoMoves(){ return KnightAttacks(bb[n]) & NotbBoard(); };

    U64 wKingCastleShort() {
        bool canCastle = castlingRights & wk;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], f1) || TestBit(occ[BOTH], g1);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(e1) || isSquareAttacked(f1) || isSquareAttacked(g1);
        if(isAttacked) return Empty;
        return precomputtedSingleBit[g1];
    };

    U64 wKingCastleLong() {
        bool canCastle = castlingRights & wq;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], b1) || TestBit(occ[BOTH], c1) || TestBit(occ[BOTH], d1);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(c1)|| isSquareAttacked(d1) || isSquareAttacked(e1);
        if(isAttacked) return Empty;
        return precomputtedSingleBit[c1];
    };

    U64 wKingCastle() { return wKingCastleShort() | wKingCastleLong(); };

    U64 bKingCastleShort() {
        bool canCastle = castlingRights & bk;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], f8) || TestBit(occ[BOTH], g8);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(e8) || isSquareAttacked(f8) || isSquareAttacked(g8);
        if(isAttacked) return Empty;
        return precomputtedSingleBit[g8];
    };

    U64 bKingCastleLong() {
        bool canCastle = castlingRights & bq;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], b8) || TestBit(occ[BOTH], c8) || TestBit(occ[BOTH], d8);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(c8) || isSquareAttacked(d8) || isSquareAttacked(e8);
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
            moves |= GetRookAttacks(sq, occupied);
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

        //pawn single pushes
        board = bb[p];
        while(board != Empty) {
            source = GetLSBIndex(board);
            moves = bPawnPushes(precomputtedSingleBit[source]);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, p, false, false, 0, Empty);
            PopBit(board, source);
        }

        //pawn captures 
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
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, piece, false, false, 0, ableToCapture);
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

    void GenerateMoves(Moves &movesList) {
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
        if(precomputtedKnights[sq] & bb[N]) return true;
        if(precomputtedKings[sq] & bb[K]) return true;
        if(GetBishopAttacks(sq, occ[BOTH]) & bb[B]) return true;
        if(GetRookAttacks(sq, occ[BOTH]) & bb[R]) return true;
        if(GetQueenAttacks(sq, occ[BOTH]) & bb[Q]) return true;
        return false;
    };

    bool isSquareAttacked(int sq) { return isWhiteMove ? BlackAttacksSquare(sq) : WhiteAttacksSquare(sq); };
    bool isInCheck() { return isSquareAttacked(isWhiteMove ? wKingSq : bKingSq); };

    //make move helpers
    bool StartOrTargetEqual(int start, int target, int sq) { return start == sq || target == sq; };

    void UpdateCastlingRights(int startSq, int targetSq) {
        if((StartOrTargetEqual(startSq, targetSq, a8) || StartOrTargetEqual(startSq, targetSq, e8)) && (castlingRights & bq) ) { castlingRights ^= bq; SetCastlingZobrist(q, zobrist); }
        if((StartOrTargetEqual(startSq, targetSq, h8) || StartOrTargetEqual(startSq, targetSq, e8)) && (castlingRights & bk) ) { castlingRights ^= bk; SetCastlingZobrist(k, zobrist); }
        if((StartOrTargetEqual(startSq, targetSq, a1) || StartOrTargetEqual(startSq, targetSq, e1)) && (castlingRights & wq) ) { castlingRights ^= wq; SetCastlingZobrist(Q, zobrist); }
        if((StartOrTargetEqual(startSq, targetSq, h1) || StartOrTargetEqual(startSq, targetSq, e1)) && (castlingRights & wk) ) { castlingRights ^= wk; SetCastlingZobrist(K, zobrist); }
    };

    //making moves 
    bool PossibleMoveIsACapture(int move) { return getMoveCapture(move); }; 
    bool IsDraw() { return isMoveRepetition || (halfMoveClock >= 50); };

    bool MakeMove(int move) {
        if(!move) return 0;
        //move data
        int source = getMoveSource(move);
        int target = getMoveTarget(move);
        int piece = getMovePiece(move);
        int promotedP = getMovePromoted(move);
        int capture = getMoveCapture(move);
        int doublePush = getMoveDouble(move);
        int enpass = getMoveEnpassant(move);
        int castling = getMoveCastling(move);
        bool pawnMove = piece == P || piece == p;

        //board updates 
        PopBit(bb[piece], source); SetBit(bb[piece], target);
        SetZobristHash(zobrist, source, piece); SetZobristHash(zobrist, target, piece);

        if(capture) {
            int startP, endP;
            if(isWhiteMove) {
                startP = p;
                endP = k;
            } else {
                startP = P;
                endP = K;
            }

            for(int bbPiece = startP; bbPiece <= endP; bbPiece++) {
                if(TestBit(bb[bbPiece], target)) {
                    PopBit(bb[bbPiece], target);
                    SetZobristHash(zobrist, target, bbPiece);
                    RemoveMaterialValue(bbPiece, target);
                    break;
                }
            }
        }

        if(promotedP) {
            isWhiteMove ? PopBit(bb[P], target) : PopBit(bb[p], target);
            isWhiteMove ? RemoveMaterialValue(P, target) : RemoveMaterialValue(p, target);

            int promoToPiece = GetPromoPiece(promotedP, isWhiteMove);
            SetBit(bb[promoToPiece], target);
            SetZobristHash(zobrist, target, promoToPiece);
            AddMaterialValue(promoToPiece, target);            
        }

        SetZobristHash(zobrist, enPassantTarget);
        if(enpass && isWhiteMove) { PopBit(bb[p], target+8); RemoveMaterialValue(p, target+8); SetZobristHash(zobrist, target+8, p); }
        if(enpass && !isWhiteMove) { PopBit(bb[P], target-8); RemoveMaterialValue(P, target-8); SetZobristHash(zobrist, target-8, P); }
        enPassantTarget = 0;
        if(doublePush) isWhiteMove ? enPassantTarget = target+8 : enPassantTarget = target-8;
        SetZobristHash(zobrist, enPassantTarget);

        if(castling) {
            switch (target) {
                // white castles king side
                case (g1):
                    // move H rook
                    PopBit(bb[R], h1); SetBit(bb[R], f1);
                    SetZobristHash(zobrist, h1, R);SetZobristHash(zobrist, f1, R);
                    break;
                
                // white castles queen side
                case (c1):
                    // move A rook
                    PopBit(bb[R], a1); SetBit(bb[R], d1);
                    SetZobristHash(zobrist, a1, R);SetZobristHash(zobrist, d1, R);
                    break;
                
                // black castles king side
                case (g8):
                    // move H rook
                    PopBit(bb[r], h8); SetBit(bb[r], f8);
                    SetZobristHash(zobrist, h8, r);SetZobristHash(zobrist, f8, r);
                    break;
                
                // black castles queen side
                case (c8):
                    // move A rook
                    PopBit(bb[r], a8); SetBit(bb[r], d8);
                    SetZobristHash(zobrist, a8, r);SetZobristHash(zobrist, d8, r);
                    break;
            }
        }

        //castling right update
        UpdateCastlingRights(source, target);

        //update king sq
        if(piece == K) wKingSq = target;
        if(piece == k) bKingSq = target;

        //Update occupancies
        memset(occ, 0, 24);
        for (int bbPiece = P; bbPiece <= K; bbPiece++) occ[WHITE] |= bb[bbPiece];
        for (int bbPiece = p; bbPiece <= k; bbPiece++) occ[BLACK] |= bb[bbPiece];

        occ[BOTH] |= occ[WHITE];
        occ[BOTH] |= occ[BLACK];

        //move side, move num and half clock num 
        pawnMove || capture ? halfMoveClock = 0: halfMoveClock++;
        isWhiteMove = !isWhiteMove;
        if(isWhiteMove) { fullTurnNum++; }
        SetZobristHash(zobrist, isWhiteMove);
                
        //if king is in check after move
        bool isKingInCheck = isWhiteMove ? WhiteAttacksSquare(bKingSq) : BlackAttacksSquare(wKingSq);
        if (isKingInCheck) return 0;

        //update zobrist for repetition 
        pawnMove || castling || capture ? ClearZobristTable() : UpdateZobristTable();
        return 1;
    };

    //misc
    void PrintAllBoards() {
        Print(bb[P], "wPawn");
        Print(bb[p], "bPawn");
        Print(bb[N], "wKnight");
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

    void PrintAttackedSquares() {
        for(int i = 0; i < 64; i++) {
            if(i != 0 && (i % 8) == 0) cout << endl; 
            string value = isSquareAttacked(i) ? "1 " :  ". "; 
            cout << value;
        }
        cout << endl << endl;
    };

    void PrintPretty() {
        for(int file = 0; file < 8; file++) {
            cout << horizontalPrint << endl;
            for(int rank = 0; rank < 8; rank++) {
                int square = file*8 + rank;
                string piece = GetPieceAtIndex(square);
                cout << "| ";
                string temp = piece != "" ? piece: ".";
                cout << temp << " ";
            }
            cout << "|  " << 8-file << endl;
        }
        cout << horizontalPrint << endl << filesPrint << endl;
        cout << endl << endl;
    }

    void PrintZobristAndFields() {
        cout << "Zobrist: " << zobrist << ", IsWhiteMove: " << isWhiteMove << ", enPassantTarget: " << enPassantTarget << ", CastlingRights: " << GetCastlingRights() << ", IsDraw: " << IsDraw() << endl;
        cout << "   Fen: " << GetFen() << endl << endl;
    };
};