#include "BoardData.cpp"   

class Bitboard {
    private:
    U64 bb[12];
    U64 occ[3];

    BoardData boardData;

    public:
    ~Bitboard() { ClearBoard(); };
    Bitboard() { ClearBoard(); };
    Bitboard(string fen) { LoadFen(fen); };
    Bitboard(const Bitboard& other) {
        // Copy the U64 members
        for(int i = P; i <= k; i++) this->bb[i] = other.bb[i];
        for(int i = WHITE; i <= BOTH; i++) this->occ[i] = other.occ[i]; 

        // Copy Board Data
        this->boardData = other.boardData;
    };

    Bitboard& operator=(const Bitboard& other) {
        // Copy the U64 members
        for(int i = P; i <= k; i++) this->bb[i] = other.bb[i];
        for(int i = WHITE; i <= BOTH; i++) this->occ[i] = other.occ[i]; 

        // Copy Board Data
        this->boardData = other.boardData;
        return *this;
    };

    bool operator==(const Bitboard& other) {
        for(int i = P; i <= k; i++) {
            bool boardsEqual = this->bb[i] == other.bb[i];
            if(!boardsEqual) return false;
        }

        for(int i = WHITE; i <= BOTH; i++) {
            bool boardsEqual = this->occ[i] == other.occ[i]; 
            if(!boardsEqual) return false;
        }

        return this->boardData == other.boardData;
    };

    void ClearBoard() {
        memset(bb, 0, sizeof(bb));
        memset(occ, 0, sizeof(occ));
        boardData.Clear();
    };

    void SetBoard(char c, int sq) {
        int piece = CharToPiece(c);
        
        AddToBoard(piece, sq);
        SetBit(occ[BOTH], sq);
        piece > K ? SetBit(occ[BLACK], sq): SetBit(occ[WHITE], sq);

        if(piece == K) boardData.SetwKingSq(sq);
        if(piece == k) boardData.SetbKingSq(sq);
    };

    void LoadFenHelper(vector<string> arguments) {
        ClearBoard();
        boardData.SetDataFromFen(arguments);
    
        boardData.SetCastlingZobrist();
        boardData.SetZobristHashEnpassant();
        boardData.SetZobristHashForMove();
    };

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
        boardData.UpdateZobristTable();
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
        if(temp) fen += to_string(temp);
        return fen;
    }

    string GetFen() {
        string moveColor = boardData.IsWhiteMove() ? " w " : " b ";
        string fen = GetFenHelper() + boardData.GetFenDataString();
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
        if(TestBit(bb[P], index)) return PieceValue[P];
        if(TestBit(bb[N], index)) return PieceValue[N];
        if(TestBit(bb[B], index)) return PieceValue[B];
        if(TestBit(bb[R], index)) return PieceValue[R];
        if(TestBit(bb[Q], index)) return PieceValue[Q];
        if(TestBit(bb[K], index)) return PieceValue[K];

        if(TestBit(bb[p], index)) return PieceValue[p];
        if(TestBit(bb[n], index)) return PieceValue[n];
        if(TestBit(bb[b], index)) return PieceValue[b];
        if(TestBit(bb[r], index)) return PieceValue[r];
        if(TestBit(bb[q], index)) return PieceValue[q];
        if(TestBit(bb[k], index)) return PieceValue[k];
        return 0;
    };

    int GetAbsValueAtIndex(int index) {
        int value = GetValueAtIndex(index);
        return value >= 0 ? value : value*=-1; 
    };

    int GetEvaluation() { return boardData.GetEvaluation(); };
    int GetEvaluationWithMultiplier() { return boardData.GetEvaluation()*GetMoveMultiplier(); };
    string GetCastlingRights() { return boardData.GetCastlingRightsString(); };

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

    U64 GetOpponentBoard() {return boardData.IsWhiteMove() ? occ[BLACK] : occ[WHITE]; };
    U64 GetZobrist() { return boardData.GetZobrist(); };

    bool IsWhiteMove() { return boardData.IsWhiteMove(); };
    int GetMoveMultiplier() { return (boardData.IsWhiteMove()) ? 1 : -1; };

    U64 GetPawn() { return (boardData.IsWhiteMove()) ? bb[P] : bb[p]; };
    U64 GetKnight() { return (boardData.IsWhiteMove()) ? bb[N] : bb[n]; };
    U64 GetBishop() { return (boardData.IsWhiteMove()) ? bb[B] : bb[b]; };
    U64 GetRook() { return (boardData.IsWhiteMove()) ? bb[R] : bb[r]; };
    U64 GetQueen() { return (boardData.IsWhiteMove()) ? bb[Q] : bb[q]; };
    U64 GetKing() { return (boardData.IsWhiteMove()) ? bb[K] : bb[k]; };

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
    U64 wPawnWestCaptures(U64 b) { return wPawnWestAtt(b) & (occ[BLACK] | (boardData.GetEnPassantTarget() == 0 ? Empty : precomputtedSingleBit[boardData.GetEnPassantTarget()]) ); };
    U64 wPawnEastCaptures(U64 b) { return wPawnEastAtt(b) & (occ[BLACK] | (boardData.GetEnPassantTarget() == 0 ? Empty : precomputtedSingleBit[boardData.GetEnPassantTarget()]) ); };
    U64 wPawnAllCaptures(U64 b) { return wPawnEastCaptures(b) | wPawnWestCaptures(b); };
    U64 bPawnWestCaptures(U64 b) { return bPawnWestAtt(b) & (occ[WHITE] | (boardData.GetEnPassantTarget() == 0 ? Empty : precomputtedSingleBit[boardData.GetEnPassantTarget()]) ); };
    U64 bPawnEastCaptures(U64 b) { return bPawnEastAtt(b) & (occ[WHITE] | (boardData.GetEnPassantTarget() == 0 ? Empty : precomputtedSingleBit[boardData.GetEnPassantTarget()]) ); };
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
        bool canCastle = boardData.GetCastlingRights() & wk;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], f1) || TestBit(occ[BOTH], g1);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(e1) || isSquareAttacked(f1) || isSquareAttacked(g1);
        if(isAttacked) return Empty;
        return precomputtedSingleBit[g1];
    };

    U64 wKingCastleLong() {
        bool canCastle = boardData.GetCastlingRights() & wq;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], b1) || TestBit(occ[BOTH], c1) || TestBit(occ[BOTH], d1);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(c1)|| isSquareAttacked(d1) || isSquareAttacked(e1);
        if(isAttacked) return Empty;
        return precomputtedSingleBit[c1];
    };

    U64 wKingCastle() { return wKingCastleShort() | wKingCastleLong(); };

    U64 bKingCastleShort() {
        bool canCastle = boardData.GetCastlingRights() & bk;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], f8) || TestBit(occ[BOTH], g8);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(e8) || isSquareAttacked(f8) || isSquareAttacked(g8);
        if(isAttacked) return Empty;
        return precomputtedSingleBit[g8];
    };

    U64 bKingCastleLong() {
        bool canCastle = boardData.GetCastlingRights() & bq;
        if(!canCastle) return Empty;
        bool hasBlocker = TestBit(occ[BOTH], b8) || TestBit(occ[BOTH], c8) || TestBit(occ[BOTH], d8);
        if(hasBlocker) return Empty;
        bool isAttacked = isSquareAttacked(c8) || isSquareAttacked(d8) || isSquareAttacked(e8);
        if(isAttacked) return Empty;
        return precomputtedSingleBit[c8];
    };

    U64 bKingCastle() { return bKingCastleShort() | bKingCastleLong(); };

    U64 wKingPsuedoMoves() { return precomputtedKings[boardData.GetwKingSq()] | wKingCastle(); };
    U64 bKingPsuedoMoves() { return precomputtedKings[boardData.GetbKingSq()] | bKingCastle(); };

    U64 wKingAtt() { return precomputtedKings[boardData.GetwKingSq()]; };
    U64 bKingAtt() { return precomputtedKings[boardData.GetbKingSq()]; };

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
    };

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
    };

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

    U64 MagicBishopPsuedoMoves() { return boardData.IsWhiteMove() ? wMagicBishopPsuedoMoves() : bMagicBishopPsuedoMoves(); };
    U64 MagicRookPsuedoMoves() { return boardData.IsWhiteMove() ? wMagicRookPsuedoMoves() : bMagicRookPsuedoMoves(); };
    U64 MagicQueenPsuedoMoves() { return boardData.IsWhiteMove() ? wMagicQueenPsuedoMoves() : bMagicQueenPsuedoMoves(); };

    U64 wPsuedoMoves() { return wPawnPsuedoMoves(bb[P]) | wKnightPsuedoMoves() | wMagicBishopPsuedoMoves() | wMagicRookPsuedoMoves() | wMagicQueenPsuedoMoves(); };
    U64 bPsuedoMoves() { return bPawnPsuedoMoves(bb[p]) | bKnightPsuedoMoves() | bMagicBishopPsuedoMoves() | bMagicRookPsuedoMoves() | bMagicQueenPsuedoMoves(); };

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
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, P, true, true, boardData.GetEnPassantTarget(), occ[BLACK]);
            PopBit(board, source);
        }
    };

    void GenerateWhiteKingMoves(Moves &movesList) {
        int source, target;
        U64 board, att, moves;

        //king moves
        att = wKingPsuedoMoves();
        moves = GetMovesFromPieceAttacks(att, occ[WHITE]);
        if(moves != Empty) FindAndInsertMoves(movesList, boardData.GetwKingSq(), moves, K, true, false, 0, occ[BLACK]);
    };

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
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, p, false, true, boardData.GetEnPassantTarget(), occ[WHITE]);
            PopBit(board, source);
        }
    };

    void GenerateBlackKingMoves(Moves &movesList) {
        int source, target;
        U64 board, att, moves;

        //king moves
        att = bKingPsuedoMoves();
        moves = GetMovesFromPieceAttacks(att, occ[BLACK]);
        if(moves != Empty) FindAndInsertMoves(movesList, boardData.GetbKingSq(), moves, k, false, false, 0, occ[WHITE]);
    };

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
    };

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
    };

    void GenerateRookMoves(Moves &movesList, bool isWhiteToMove) { 
        int source, target;
        U64 board, att, moves;

        int piece = isWhiteToMove ? R : r;
        U64 unableToCapture = isWhiteToMove ? occ[WHITE] : occ[BLACK];
        U64 ableToCapture = isWhiteToMove ? occ[BLACK] : occ[WHITE];

        //rook moves
        board = bb[piece];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = GetRookAttacks(source, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, piece, false, false, 0, ableToCapture);
            PopBit(board, source);
        }
    };

    void GenerateQueenMoves(Moves &movesList, bool isWhiteToMove) { 
        int source, target;
        U64 board, att, moves;

        int piece = isWhiteToMove ? Q : q;
        U64 unableToCapture = isWhiteToMove ? occ[WHITE] : occ[BLACK];
        U64 ableToCapture = isWhiteToMove ? occ[BLACK] : occ[WHITE];

        //queen moves
        board = bb[piece];
        while(board != Empty) {
            source = GetLSBIndex(board);
            att = GetQueenAttacks(source, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, source, moves, piece, false, false, 0, ableToCapture);
            PopBit(board, source);
        }
    };

    void GenerateMoves(Moves &movesList) {
        movesList.Clear();
        bool white = boardData.IsWhiteMove();

        if(white) {
            GenerateWhitePawnMoves(movesList);
            GenerateWhiteKingMoves(movesList);
        } else {
            GenerateBlackPawnMoves(movesList);
            GenerateBlackKingMoves(movesList);
        }
        
        GenerareKnightMoves(movesList, white);
        GenerateBishopMoves(movesList, white);
        GenerateRookMoves(movesList, white);
        GenerateQueenMoves(movesList, white);
    };

    Moves GenerateMoves() {
        Moves movesList; 
        GenerateMoves(movesList);
        return movesList;
    };

    //attacks
    bool BlackAttacksSquare(int sq) {
        if(precomputtedWhitePawnAttacks[sq] & bb[p]) return true;
        if(precomputtedKnights[sq] & bb[n]) return true;
        if(precomputtedKings[sq] & bb[k]) return true;
        if(GetBishopAttacks(sq, occ[BOTH]) & (bb[b] | bb[q])) return true;
        if(GetRookAttacks(sq, occ[BOTH]) & (bb[r] | bb[q])) return true;
        return false;
    };

    bool WhiteAttacksSquare(int sq) {
        if(precomputtedBlackPawnAttacks[sq] & bb[P]) return true;
        if(precomputtedKnights[sq] & bb[N]) return true;
        if(precomputtedKings[sq] & bb[K]) return true;
        if(GetBishopAttacks(sq, occ[BOTH]) & (bb[B] | bb[Q])) return true;
        if(GetRookAttacks(sq, occ[BOTH]) & (bb[R] | bb[Q])) return true;
        return false;
    };

    bool isSquareAttacked(int sq) { return boardData.IsWhiteMove() ? BlackAttacksSquare(sq) : WhiteAttacksSquare(sq); };
    bool isInCheck() { return isSquareAttacked(boardData.IsWhiteMove() ? boardData.GetwKingSq() : boardData.GetbKingSq()); };

    //making move helpers
    void AddToBoard(int piece, int sq) { SetBit(bb[piece], sq); boardData.AddMaterialValue(piece, sq); boardData.SetZobristHashForSquareAndPiece(sq, piece); };
    void RemoveFromBoard(int piece, int sq) { PopBit(bb[piece], sq); boardData.RemoveMaterialValue(piece, sq); boardData.SetZobristHashForSquareAndPiece(sq, piece); };
    bool PossibleMoveIsACapture(int move) { return getMoveCapture(move); }; 
    bool IsDraw() { return boardData.IsDraw(); };

    Moves GetMovesByIndex(int index) {
        Moves movesList;
        if(!TestBit(occ[BOTH], index)) return movesList; 

        U64 att, moves, unableToCapture, ableToCapture;

        if(TestBit(bb[P], index)) {
            moves = wPawnPushes(precomputtedSingleBit[index]);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, P, true, false, 0, Empty);

            //pawn captures 
            moves = wPawnAllCaptures(precomputtedSingleBit[index]);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, P, true, true, boardData.GetEnPassantTarget(), occ[BLACK]);

            return movesList;
        }
        if(TestBit(bb[N], index)) {
            unableToCapture = occ[WHITE];
            ableToCapture = occ[BLACK];

            att = precomputtedKnights[index];
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, N, true, false, 0, ableToCapture);

            return movesList;
        }
        if(TestBit(bb[B], index)) {
            unableToCapture = occ[WHITE];
            ableToCapture = occ[BLACK];

            att = GetBishopAttacks(index, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, B, true, false, 0, ableToCapture);

            return movesList;
        }
        if(TestBit(bb[R], index)) {
            unableToCapture = occ[WHITE];
            ableToCapture = occ[BLACK];

            att = GetRookAttacks(index, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, R, true, false, 0, ableToCapture);

            return movesList;
        }
        if(TestBit(bb[Q], index)) {
            unableToCapture = occ[WHITE];
            ableToCapture = occ[BLACK];

            att = GetQueenAttacks(index, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, Q, true, false, 0, ableToCapture);

            return movesList;
        }
        if(TestBit(bb[K], index)) {
            unableToCapture = occ[WHITE];
            ableToCapture = occ[BLACK];

            att = wKingPsuedoMoves();
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, K, true, false, 0, ableToCapture);

            return movesList;
        }

        if(TestBit(bb[p], index)) {
            unableToCapture = occ[BLACK];
            ableToCapture = occ[WHITE];
            moves = bPawnPushes(precomputtedSingleBit[index]);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, p, false, false, 0, Empty);

            //pawn captures 
            moves = bPawnAllCaptures(precomputtedSingleBit[index]);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, p, false, true, boardData.GetEnPassantTarget(), occ[BLACK]);

            return movesList;
        }
        if(TestBit(bb[n], index)) {
            unableToCapture = occ[BLACK];
            ableToCapture = occ[WHITE];

            att = precomputtedKnights[index];
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, n, false, false, 0, ableToCapture);

            return movesList;
        }
        if(TestBit(bb[b], index)) {
            unableToCapture = occ[BLACK];
            ableToCapture = occ[WHITE];

            att = GetBishopAttacks(index, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, b, false, false, 0, ableToCapture);

            return movesList;
        }
        if(TestBit(bb[r], index)) {
            unableToCapture = occ[BLACK];
            ableToCapture = occ[WHITE];

            att = GetRookAttacks(index, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, r, false, false, 0, ableToCapture);

            return movesList;
        }
        if(TestBit(bb[q], index)) {
            unableToCapture = occ[BLACK];
            ableToCapture = occ[WHITE];

            att = GetQueenAttacks(index, occ[BOTH]);
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, q, false, false, 0, ableToCapture);

            return movesList;
        }
        if(TestBit(bb[k], index)) {
            unableToCapture = occ[BLACK];
            ableToCapture = occ[WHITE];

            att = bKingPsuedoMoves();
            moves = GetMovesFromPieceAttacks(att, unableToCapture);
            if(moves != Empty) FindAndInsertMoves(movesList, index, moves, k, false, false, 0, ableToCapture);

            return movesList;
        }
    };

    bool MakeMoveFromUci(string uci) {
        string start = uci.substr(0, 2);
        int index = StringtoIndex(start);
        if(index == -1) return 0;

        Moves movesList = GetMovesByIndex(index);
        int move = GetMoveByUci(movesList, uci);
        return MakeMove(move);
    }

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
        RemoveFromBoard(piece, source); AddToBoard(piece, target);

        if(capture) {
            int startP, endP;
            if(boardData.IsWhiteMove()) {
                startP = p;
                endP = k;
            } else {
                startP = P;
                endP = K;
            }

            for(int bbPiece = startP; bbPiece <= endP; bbPiece++) {
                if(TestBit(bb[bbPiece], target)) {
                    RemoveFromBoard(bbPiece, target);
                    break;
                }
            }
        }

        if(promotedP) {
            //remove pawn
            boardData.IsWhiteMove() ? RemoveFromBoard(P, target) : RemoveFromBoard(p, target);

            //add promotoed piece to board
            int promoToPiece = GetPromoPiece(promotedP, boardData.IsWhiteMove());
            AddToBoard(promoToPiece, target);    
        }

        boardData.SetZobristHashEnpassant();
        if(enpass && boardData.IsWhiteMove()) { RemoveFromBoard(p, target+8); }
        if(enpass && !boardData.IsWhiteMove()) { RemoveFromBoard(P, target-8); }
        boardData.SetEnpassantTarget(0);
        if(doublePush) boardData.IsWhiteMove() ? boardData.SetEnpassantTarget(target+8) : boardData.SetEnpassantTarget(target-8);
        boardData.SetZobristHashEnpassant();

        if(castling) {
            switch (target) {
                // white castles king side
                case (g1):
                    // move H rook
                    PopBit(bb[R], h1); SetBit(bb[R], f1);
                    boardData.SetZobristHashForSquareAndPiece(h1, R);boardData.SetZobristHashForSquareAndPiece(f1, R);
                    break;
                
                // white castles queen side
                case (c1):
                    // move A rook
                    PopBit(bb[R], a1); SetBit(bb[R], d1);
                    boardData.SetZobristHashForSquareAndPiece(a1, R);boardData.SetZobristHashForSquareAndPiece(d1, R);
                    break;
                
                // black castles king side
                case (g8):
                    // move H rook
                    PopBit(bb[r], h8); SetBit(bb[r], f8);
                    boardData.SetZobristHashForSquareAndPiece(h8, r);boardData.SetZobristHashForSquareAndPiece(f8, r);
                    break;
                
                // black castles queen side
                case (c8):
                    // move A rook
                    PopBit(bb[r], a8); SetBit(bb[r], d8);
                    boardData.SetZobristHashForSquareAndPiece(a8, r);boardData.SetZobristHashForSquareAndPiece(d8, r);
                    break;
            }
        }

        //castling right update
        boardData.UpdateCastlingRights(source, target);

        //update king sq
        if(piece == K) boardData.SetwKingSq(target);
        if(piece == k) boardData.SetbKingSq(target);

        //Update occupancies
        memset(occ, 0, sizeof(occ));
        for (int bbPiece = P; bbPiece <= K; bbPiece++) occ[WHITE] |= bb[bbPiece];
        for (int bbPiece = p; bbPiece <= k; bbPiece++) occ[BLACK] |= bb[bbPiece];

        occ[BOTH] |= occ[WHITE];
        occ[BOTH] |= occ[BLACK];

        //move side, move num and half clock num 
        boardData.UpdateHalfMoveClock(pawnMove || capture);
        boardData.ChangeWhiteMove();
        boardData.CheckFullTurnNum();
        boardData.SetZobristHashForMove(true);
                
        //if king is in check after move
        bool isKingInCheck = boardData.IsWhiteMove() ? WhiteAttacksSquare(boardData.GetbKingSq()) : BlackAttacksSquare(boardData.GetwKingSq());
        if (isKingInCheck) return 0;

        //update zobrist for repetition 
        boardData.ClearOrUpdateZobristTable(pawnMove || castling || capture);
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
            if(i != 0 && (i % 8) == 0) printf("\n");
            char value = isSquareAttacked(i) ? '1' :  '.';
            printf("%c ",value);
        }
        printf("\n\n");
    };

    void PrintPretty() {
        for(int file = 0; file < 8; file++) {
            printf("%s \n", horizontalPrint);
            for(int rank = 0; rank < 8; rank++) {
                int square = file*8 + rank;
                string piece = GetPieceAtIndex(square);
                string temp = piece != "" ? piece: ".";
                printf("| %c ", temp);
            }
            int filenum = 8-file;
            printf("| %d \n", filenum);
        }
        printf("%s \n", horizontalPrint);
        printf("%s \n\n\n", filesPrint);
    }
};