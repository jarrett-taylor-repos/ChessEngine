#include "bitboardhelper.cpp"
#include <unordered_map>

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

    void LoadFen(string fen) {
        LoadFenHelper(Split(fen));

        wPawn.LoadFen(currFen, 0b1001);
        wKnight.LoadFen(currFen, 0b1010);
        wBishop.LoadFen(currFen, 0b1011);
        wRook.LoadFen(currFen, 0b1100);
        wQueen.LoadFen(currFen, 0b1101);
        wKing.LoadFen(currFen, 0b1110);

        bPawn.LoadFen(currFen, 0b0001);
        bKnight.LoadFen(currFen, 0b0010);
        bBishop.LoadFen(currFen, 0b0011);
        bRook.LoadFen(currFen, 0b0100);
        bQueen.LoadFen(currFen, 0b0101);
        bKing.LoadFen(currFen, 0b0110);
    };

    void ClearBoard() {
        wPawn.ClearBoard();
        wKnight.ClearBoard();
        wBishop.ClearBoard();
        wRook.ClearBoard();
        wQueen.ClearBoard();
        wKing.ClearBoard();

        bPawn.ClearBoard();
        bKnight.ClearBoard();
        bBishop.ClearBoard();
        bRook.ClearBoard();
        bQueen.ClearBoard();
        bKing.ClearBoard();
    }

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

    //board functions
    bitset<64> wBoard() { return wPawn.GetBoard() | wKnight.GetBoard() | wBishop.GetBoard() | wRook.GetBoard() | wQueen.GetBoard() | wKing.GetBoard(); };
    bitset<64> bBoard() { return bPawn.GetBoard() | bKnight.GetBoard() | bBishop.GetBoard() | bRook.GetBoard() | bQueen.GetBoard() | bKing.GetBoard(); };
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
    bitset<64> wPawnPushes() { return wSinglePushTargets(wPawn.GetBoard()) | wDoublePushTargets(wPawn.GetBoard()); };
    bitset<64> bPawnPushes() { return bSinglePushTargets(bPawn.GetBoard()) | bDoublePushTargets(bPawn.GetBoard()); };

    bitset<64> wPawnWestAtt() { return (wPawn.GetBoard() >> 9) & notHFile; };
    bitset<64> wPawnEastAtt() { return (wPawn.GetBoard() >> 7) & notAFile; };
    bitset<64> wPawnAllAtt() { return wPawnEastAtt() | wPawnWestAtt(); };
    bitset<64> bPawnWestAtt() { return (bPawn.GetBoard() << 9) & notAFile; };
    bitset<64> bPawnEastAtt() { return (bPawn.GetBoard() << 7) & notHFile; };
    bitset<64> bPawnAllAtt() { return bPawnEastAtt() | bPawnWestAtt(); };
    bitset<64> wPawnWestCaptures() { return wPawnWestAtt() & bBoard(); };
    bitset<64> wPawnEastCaptures() { return wPawnEastAtt() & bBoard(); };
    bitset<64> wPawnAllCaptures() { return wPawnEastCaptures() | wPawnWestCaptures(); };
    bitset<64> bPawnWestCaptures() { return bPawnWestAtt() & wBoard(); };
    bitset<64> bPawnEastCaptures() { return bPawnEastAtt() & wBoard(); };
    bitset<64> bPawnAllCaptures() { return bPawnEastCaptures() | bPawnWestCaptures(); };

    bitset<64> wPawnMoves() { return wPawnAllCaptures() | wPawnPushes(); }
    bitset<64> bPawnMoves() { return bPawnAllCaptures() | bPawnPushes(); }

    vector<string> wPawnWestCapturesUCI() { 
        bitset<64> wPawnWestCap = wPawnWestAtt() & bBoard(); 
        bitset<64> wPawnOrigin = wPawnWestCap << 7; 
        return BitsetToUCI(wPawnOrigin, wPawnWestCap);
    };
    vector<string> wPawnEastCapturesUCI() {  
        bitset<64> wPawnEastCap = wPawnEastAtt() & bBoard(); 
        bitset<64> wPawnOrigin = wPawnEastCap << 9; 
        return BitsetToUCI(wPawnOrigin, wPawnEastCap);
    };

    vector<string> wPawnMovesAllCapturesUCI() { return Combine(wPawnWestCapturesUCI(), wPawnEastCapturesUCI()); };

    vector<string> wPawnSinglePushesUCI() {
        bitset<64> wPawnBoard = wPawn.GetBoard();
        bitset<64> wPawnSinglePushes = wSinglePushTargets(wPawnBoard);
        bitset<64> wPawnSinglePushesOriginal = wPawnSinglePushes << 8;
        return BitsetToUCI(wPawnSinglePushesOriginal, wPawnSinglePushes);
    }
    vector<string> wPawnDoublePushesUCI() {
        bitset<64> wPawnBoard = wPawn.GetBoard();
        bitset<64> wPawnDoublePushes = wDoublePushTargets(wPawnBoard);
        bitset<64> wPawDoublePushesOriginal = wPawnDoublePushes << 16;
        return BitsetToUCI(wPawDoublePushesOriginal, wPawnDoublePushes);
    }

    vector<string> wPawnMovesPushesUCI() { return Combine(wPawnSinglePushesUCI(), wPawnDoublePushesUCI()); };
    vector<string> wPawnMovesUCI() { return Combine(wPawnMovesAllCapturesUCI(), wPawnMovesPushesUCI()); };

    vector<string> bPawnWestCapturesUCI() { 
        bitset<64> bPawnWestCap = bPawnWestAtt() & wBoard(); 
        bitset<64> bPawnOrigin = bPawnWestCap >> 9; 
        return BitsetToUCI(bPawnOrigin, bPawnWestCap);
    };
    vector<string> bPawnEastCapturesUCI() {  
        bitset<64> bPawnEastCap = bPawnEastAtt() & wBoard(); 
        bitset<64> bPawnOrigin = bPawnEastCap >> 7; 
        return BitsetToUCI(bPawnOrigin, bPawnEastCap);
    };

    vector<string> bPawnMovesAllCapturesUCI() { return Combine(bPawnWestCapturesUCI(), bPawnEastCapturesUCI()); };

    vector<string> bPawnSinglePushesUCI() {
        bitset<64> bPawnBoard = bPawn.GetBoard();
        bitset<64> bPawnSinglePushes = bSinglePushTargets(bPawnBoard);
        bitset<64> bPawnSinglePushesOriginal = bPawnSinglePushes >> 8;
        return BitsetToUCI(bPawnSinglePushesOriginal, bPawnSinglePushes);
    }
    vector<string> bPawnDoublePushesUCI() {
        bitset<64> bPawnBoard = bPawn.GetBoard();
        bitset<64> bPawnDoublePushes = bDoublePushTargets(bPawnBoard);
        bitset<64> bPawnDoublePushesOriginal = bPawnDoublePushes >> 16;
        return BitsetToUCI(bPawnDoublePushesOriginal, bPawnDoublePushes);
    }

    vector<string> bPawnMovesPushesUCI() { return Combine(bPawnSinglePushesUCI(), bPawnDoublePushesUCI()); };
    vector<string> bPawnMovesUCI() { return Combine(bPawnMovesAllCapturesUCI(), bPawnMovesPushesUCI()); };


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

    vector<string> wKnightMovesUCI() { 
        vector<string> ucimoves;
        vector<int> indexes = BitSetTrueIndexes(wKnight.GetBoard());
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
        vector<int> indexes = BitSetTrueIndexes(bKnight.GetBoard());
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
    bitset<64> wKingMoves() { return OneInAllDirection(wKing.GetBoard()) & NotwBoard(); };
    bitset<64> bKingMoves() { return OneInAllDirection(bKing.GetBoard()) & NotbBoard(); };

    vector<string> wKingMovesUCI() {
        int index = BitSetTrueIndexes(wKing.GetBoard()).at(0);
        bitset<64> wkingmoves = wKingMoves();
        return BitsetToUCI(IndexToSquare(index), wkingmoves);
    };

    vector<string> bKingMovesUCI() {
        int index = BitSetTrueIndexes(bKing.GetBoard()).at(0);
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
    }

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
    }

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
    }

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
    }

    bitset<64> BishopAttacks(bitset<64> bishopBoard) {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(bishopBoard);
        for(int sq : indexes) moves |= BishopAttacksBySquare(sq); 
        return moves;
    }

    bitset<64> BishopAttacks(bitset<64> bishopBoard, bitset<64> occ) {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(bishopBoard);
        for(int sq : indexes) moves |= BishopAttacksBySquare(occ, sq); 
        return moves;
    }

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
    }

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
    }

    bitset<64> RookAttacks(bitset<64> rBoard) {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(rBoard);
        for(int sq : indexes) moves |= RookAttacksBySquare(sq);
        return moves;
    }

    bitset<64> RookAttacks(bitset<64> rBoard, bitset<64> occ) {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(rBoard);
        for(int sq : indexes) moves |= RookAttacksBySquare(occ, sq);
        return moves;
    }

    //sliding moves
    bitset<64> wBishopAttacks() { return BishopAttacks(wBishop.GetBoard()); };
    bitset<64> bBishopAttacks() { return BishopAttacks(bBishop.GetBoard()); };
    bitset<64> wBishopMoves() { bitset<64> wbatt = wBishopAttacks(); return wbatt & ~(wbatt & wBoard()); };
    bitset<64> bBishopMoves() { bitset<64> bbatt = bBishopAttacks(); return bbatt & ~(bbatt & bBoard()); };

    bitset<64> wRookAttacks() { return RookAttacks(wRook.GetBoard()); };
    bitset<64> bRookAttacks() { return RookAttacks(bRook.GetBoard()); };
    bitset<64> wRookMoves() { bitset<64> wratt = wRookAttacks(); return wratt & ~(wratt & wBoard()); };
    bitset<64> bRookMoves() { bitset<64> bratt = bRookAttacks(); return bratt & ~(bratt & bBoard()); };

    bitset<64> wQueenAttacks() { return RookAttacks(wQueen.GetBoard()) | BishopAttacks(wQueen.GetBoard()); };
    bitset<64> bQueenAttacks() { return RookAttacks(bQueen.GetBoard()) | BishopAttacks(bQueen.GetBoard()); };
    bitset<64> wQueenMoves() { bitset<64> wqatt = wQueenAttacks(); return wqatt & ~(wqatt & wBoard()); };
    bitset<64> bQueenMoves() { bitset<64> bqatt = bQueenAttacks(); return bqatt & ~(bqatt & bBoard()); };


    vector<string> wBishopMovesUCI() {
        vector<string> ucimoves;
        bitset<64> wbishop = wBishop.GetBoard();
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
    }

    vector<string> bBishopMovesUCI() {
        vector<string> ucimoves;
        bitset<64> bbishop = bBishop.GetBoard();
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
    }

    vector<string> wRookMovesUCI() {
        vector<string> ucimoves;
        bitset<64> wrook = wRook.GetBoard();
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
    }

    vector<string> bRookMovesUCI() {
        vector<string> ucimoves;
        bitset<64> brook = bRook.GetBoard();
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
    }

    vector<string> wQueenMovesUCI() {
        vector<string> ucimoves;
        bitset<64> wqueen = wQueen.GetBoard();
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
    }

    vector<string> bQueenMovesUCI() {
        vector<string> ucimoves;
        bitset<64> bqueen = bQueen.GetBoard();
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
    }

    //attacks
    bitset<64> wAttacks() { return wPawnAllAtt() | wKingMoves() | wKnightMoves() | wQueenAttacks() | wBishopAttacks() | wRookAttacks(); };
    bitset<64> bAttacks() { return bPawnAllAtt() | bKingMoves() | bKnightMoves() | bQueenAttacks() | bBishopAttacks() | bRookAttacks(); };

    bitset<64> xRaywRookAttacks() {
        bitset<64> wrookmoves = wRookMoves();
        bitset<64> occ = AllBoard();
        bitset<64> blockers = wrookmoves & occ;
        blockers &= wrookmoves;
        bitset<64> occAttacks = RookAttacks(wRook.GetBoard(), occ ^ blockers);
        bitset<64> xrayattcks = (wrookmoves ^ occAttacks) & bBoard();
        return xrayattcks;
    }

    bitset<64> xRaybRookAttacks() {
        bitset<64> brookmoves = bRookMoves();
        bitset<64> occ = AllBoard();
        bitset<64> blockers = brookmoves & occ;
        blockers &= brookmoves;
        bitset<64> occAttacks = RookAttacks(bRook.GetBoard(), occ ^ blockers);
        bitset<64> xrayattcks = (brookmoves ^ occAttacks) & wBoard();
        return xrayattcks;
    }

    bitset<64> xRaywBishopAttacks() {
        bitset<64> wbishopmoves =  wBishopMoves();
        bitset<64> occ = AllBoard();
        bitset<64> blockers = wbishopmoves & occ;
        blockers &= wbishopmoves;
        bitset<64> occAttacks = BishopAttacks(wBishop.GetBoard(), occ ^ blockers);
        bitset<64> xrayattcks = (wbishopmoves ^ occAttacks) & bBoard();
        return xrayattcks;
    }

    bitset<64> xRaybBishopAttacks() {
        bitset<64> bbishopmoves =  bBishopMoves();
        bitset<64> occ = AllBoard();
        bitset<64> blockers = bbishopmoves & occ;
        blockers &= bbishopmoves;
        bitset<64> occAttacks = BishopAttacks(bBishop.GetBoard(), occ ^ blockers);
        bitset<64> xrayattcks = (bbishopmoves ^ occAttacks) & wBoard();
        return xrayattcks;
    }

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
    }
    

    //making moves helpers 
    void GetwBoardandResetIndex(int index) {
        if(wPawn.GetBit(index)) wPawn.ClearBit(index); 
        if(wKnight.GetBit(index)) wKnight.ClearBit(index); 
        if(wBishop.GetBit(index)) wBishop.ClearBit(index); 
        if(wRook.GetBit(index)) wRook.ClearBit(index); 
        if(wKing.GetBit(index)) wKing.ClearBit(index); 
        if(wQueen.GetBit(index)) wQueen.ClearBit(index); 
    }

    void GetbBoardandResetIndex(int index) {
        if(bPawn.GetBit(index))  bPawn.ClearBit(index); 
        if(bKnight.GetBit(index))  bKnight.ClearBit(index); 
        if(bBishop.GetBit(index))  bBishop.ClearBit(index); 
        if(bRook.GetBit(index))  bRook.ClearBit(index); 
        if(bKing.GetBit(index))  bKing.ClearBit(index); 
        if(bQueen.GetBit(index))  bQueen.ClearBit(index); 
    }

    void GetBoardandResetIndex(int index, bool isCapture) {
        if(isWhiteMove) {
            if(isCapture)  {
                GetbBoardandResetIndex(index); 
            } else {
                GetwBoardandResetIndex(index); 
            } 
        }
        if(isCapture) {
            GetbBoardandResetIndex(index); 
        } else {
            GetwBoardandResetIndex(index); 
        }
    };

    void GetwBoardandSetIndex(int index) {
        if(wPawn.GetBit(index)) wPawn.SetBit(index); 
        if(wKnight.GetBit(index)) wKnight.SetBit(index); 
        if(wBishop.GetBit(index)) wBishop.SetBit(index); 
        if(wRook.GetBit(index)) wRook.SetBit(index); 
        if(wKing.GetBit(index)) wKing.SetBit(index); 
        if(wQueen.GetBit(index)) wQueen.SetBit(index); 
    }

    void GetbBoardandSetIndex(int index) {
        if(bPawn.GetBit(index)) bPawn.SetBit(index); 
        if(bKnight.GetBit(index)) bKnight.SetBit(index); 
        if(bBishop.GetBit(index)) bBishop.SetBit(index); 
        if(bRook.GetBit(index)) bRook.SetBit(index); 
        if(bKing.GetBit(index)) bKing.SetBit(index); 
        if(bQueen.GetBit(index)) bQueen.SetBit(index); 
    }

    void GetBoardandSetIndex(int index) {
        if(isWhiteMove)  {
            GetwBoardandSetIndex(index); 
        } else {
            GetbBoardandSetIndex(index); 
        }
    };

    void CaptureMoveUpdate(int startIndex, int targetIndex) {
        GetBoardandResetIndex(targetIndex, true);
        GetBoardandResetIndex(startIndex, false);
        GetBoardandSetIndex(targetIndex);
    }

    void QuietMoveUpdate(int startIndex, int targetIndex) {
        GetBoardandResetIndex(startIndex, false);
        GetBoardandSetIndex(targetIndex);
    }

    //making moves 
    void MakeMove(string move) {
        vector<string> ucimoves = GetUciMoves();

        //if move is not in moves
        auto it = find(ucimoves.begin(), ucimoves.end(), move);
        if (it == ucimoves.end()) return;
        
        //determine board and moves
        string startSquare = move.substr(0, 2);
        string targetSquare = move.substr(2, 2);
        int startIndex = StringtoIndex(startSquare);
        int targetIndex = StringtoIndex(targetSquare);

        //tets if capture and update
        bool isCaptureMove = AllBoard().test(targetIndex);
        if(isCaptureMove) { 
            CaptureMoveUpdate(startIndex, targetIndex); 
        } else {
            QuietMoveUpdate(startIndex, targetIndex);
        }

        //after move is made 
    };




    //misc
    void PrintAllBoards() {
        wPawn.PrintBitBoard();
        bPawn.PrintBitBoard();
        wKnight.PrintBitBoard();
        bKnight.PrintBitBoard();
        wBishop.PrintBitBoard();
        bBishop.PrintBitBoard();
        wRook.PrintBitBoard();
        bRook.PrintBitBoard();
        wQueen.PrintBitBoard();
        bQueen.PrintBitBoard();
        wKing.PrintBitBoard();
        bKing.PrintBitBoard();
    }
};