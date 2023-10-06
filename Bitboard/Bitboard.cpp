#include "bitboardhelper.cpp"
#include "..\MoveEngine\PrecomputtedMoves.h"
using namespace PrecomputtedMoves;

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
    Bitboard() { LoadFen(startFen); };
    Bitboard(string fen) { LoadFen(fen); };

    void LoadFen(string fen) {
        ClearBoard();

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
    bitset<64> northOne(bitset<64> b) { return b >> 8; };
    bitset<64> southOne(bitset<64> b) { return b << 8; };
    bitset<64> eastOne(bitset<64> b) { return b << 1 & ~aFile; };
    bitset<64> westOne(bitset<64> b) { return b >> 1 & ~hFile; };
    bitset<64> northEastOne(bitset<64> b) { return b >> 7 & ~aFile; };
    bitset<64> northWestOne(bitset<64> b) { return b >> 9 & ~hFile; };
    bitset<64> southEastOne(bitset<64> b) { return b << 9 & ~aFile; };
    bitset<64> southWestOne(bitset<64> b) { return b << 7 & ~hFile; };
    bool InBounds(int offset) { return (offset >= 0) && (offset < 64); };

    //pawn functions
    bitset<64> wSinglePushTargets(bitset<64> b) { return northOne(b) & EmptyBoard(); };
    bitset<64> wDoublePushTargets(bitset<64> b) { return northOne(wSinglePushTargets(b)) & EmptyBoard() & rank4; };
    bitset<64> bSinglePushTargets(bitset<64> b) { return southOne(b) & EmptyBoard(); };
    bitset<64> bDoublePushTargets(bitset<64> b) { return southOne(bSinglePushTargets(b)) & EmptyBoard() & rank5; };
    bitset<64> wPawnPushes() { return wSinglePushTargets(wPawn.GetBoard()) | wDoublePushTargets(wPawn.GetBoard()); };
    bitset<64> bPawnPushes() { return bSinglePushTargets(bPawn.GetBoard()) | bDoublePushTargets(bPawn.GetBoard()); };

    bitset<64> wPawnWestAtt() { return (wPawn.GetBoard() >> 9) & ~hFile; };
    bitset<64> wPawnEastAtt() { return (wPawn.GetBoard() >> 7) & ~aFile; };
    bitset<64> wPawnAllAtt() { return wPawnEastAtt() | wPawnWestAtt(); };
    bitset<64> bPawnWestAtt() { return (bPawn.GetBoard() << 9) & ~aFile; };
    bitset<64> bPawnEastAtt() { return (bPawn.GetBoard() << 7) & ~hFile; };
    bitset<64> bPawnAllAtt() { return bPawnEastAtt() | bPawnWestAtt(); };
    bitset<64> wPawnWestCaptures() { return wPawnWestAtt() & bBoard(); };
    bitset<64> wPawnEastCaptures() { return wPawnEastAtt() & bBoard(); };
    bitset<64> wPawnAllCaptures() { return wPawnEastCaptures() | wPawnWestCaptures(); };
    bitset<64> bPawnWestCaptures() { return bPawnWestAtt() & wBoard(); };
    bitset<64> bPawnEastCaptures() { return bPawnEastAtt() & wBoard(); };
    bitset<64> bPawnAllCaptures() { return bPawnEastCaptures() | bPawnWestCaptures(); };

    bitset<64> wPawnMoves() { return wPawnAllCaptures() | wPawnPushes(); }
    bitset<64> bPawnMoves() { return bPawnAllCaptures() | bPawnPushes(); }


    //knight functions
    bitset<64> KnightMoveHelper(bitset<64> b) { return noNoEa(b) | noEaEa(b) | soEaEa(b) | soSoEa(b) | noNoWe(b) | noWeWe(b) | soWeWe(b) | soSoWe(b); }
    bitset<64> noNoEa(bitset<64> b) {return (b >> 15) & ~aFile; }
    bitset<64> noEaEa(bitset<64> b) {return (b >> 6) & (~aFile & ~bFile);}
    bitset<64> soEaEa(bitset<64> b) {return (b << 10) & (~aFile & ~bFile);}
    bitset<64> soSoEa(bitset<64> b) {return (b << 17) & ~aFile; }
    bitset<64> soSoWe(bitset<64> b) {return (b << 15) & ~hFile; }
    bitset<64> soWeWe(bitset<64> b) {return (b << 6) & (~gFile & ~hFile); }
    bitset<64> noWeWe(bitset<64> b) {return (b >> 10) & (~gFile & ~hFile); }
    bitset<64> noNoWe(bitset<64> b) {return (b >> 17) & ~hFile; }

    bitset<64> wKnightMoves(){ return KnightMoveHelper(wKnight.GetBoard()) & NotwBoard(); }
    bitset<64> bKnightMoves(){ return KnightMoveHelper(bKnight.GetBoard()) & NotbBoard(); }

    //king moves
    bitset<64> bKingMoves() { 
        bitset<64> b = bKing.GetBoard();
        return (northOne(b) | southOne(b) | eastOne(b) | westOne(b) | northEastOne(b) | northWestOne(b) | southEastOne(b) | southWestOne(b)) & NotbBoard();
    };

    bitset<64> wKingMoves() { 
        bitset<64> b = wKing.GetBoard();
        return (northOne(b) | southOne(b) | eastOne(b) | westOne(b) | northEastOne(b) | northWestOne(b) | southEastOne(b) | southWestOne(b)) & NotwBoard();
    };

    //sliding moves helper
    bool ValidTravel(bitset<64> overlap, int offset) { return InBounds(offset) && overlap.test(offset) && !AllBoard().test(offset); };
    bool ValidTravelAtt(bitset<64> overlap, int offset) { return InBounds(offset) && overlap.test(offset) && AllBoard().test(offset); };

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

    bitset<64> BishopAttacks(bitset<64> bishopBoard) {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(bishopBoard);
        for(int sq : indexes) {
            //travel northEastOne
            moves |= SlidingAttacks(~aFile, sq, -7);
            //travel northWestOne
            moves |= SlidingAttacks(~hFile, sq, -9);
            //travel southEastOne
            moves |= SlidingAttacks(~aFile, sq, 9);
            //travle southWestOne
            moves |= SlidingAttacks(~hFile, sq, 7);
        }
        return moves;
    }

    bitset<64> RookAttacks(bitset<64> rBoard) {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(rBoard);
        for(int sq : indexes) {
            //travel northOne
            moves |= SlidingAttacks(~rank1, sq, -8);
            //travel eastOne
            moves |= SlidingAttacks(~aFile, sq, 1);
            //travel westOne
            moves |= SlidingAttacks(~hFile, sq, -1);
            //travle southOne
            moves |= SlidingAttacks(~rank8, sq, 8);
        }
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

    //attacks
    bitset<64> wAttacks() { return wPawnAllAtt() | wKingMoves() | wKnightMoves() | wQueenAttacks() | wBishopAttacks() | wRookAttacks(); };
    bitset<64> bAttacks() { return bPawnAllAtt() | bKingMoves() | bKnightMoves() | bQueenAttacks() | bBishopAttacks() | bRookAttacks(); };

    //precomputted 
    bitset<64> precomputtedBishopAttacks(int sq) {
        bitset<64> free = EmptyBoard().to_ullong();
        bitset<64> msk = maskBishop[sq].to_ullong();
        bitset<64> bb  = diagonals[sq].to_ullong();
        bitset<64> at  = bb;
        bb &= free;
        switch ( repsB[sq] )
        {
        case 6:
            bb >>= 9; bitsetMultiply(bb, bMult); bb &= msk; at |= bb; bb &= free;
        case 5:
            bb >>= 9; bitsetMultiply(bb, bMult); bb &= msk; at |= bb; bb &= free;
        case 4:
            bb >>= 9; bitsetMultiply(bb, bMult); bb &= msk; at |= bb; bb &= free;
        case 3:
            bb >>= 9; bitsetMultiply(bb, bMult); bb &= msk; at |= bb; bb &= free;
            bb >>= 9; bitsetMultiply(bb, bMult); bb &= msk; at |= bb; bb &= free;
            bb >>= 9; bitsetMultiply(bb, bMult); bb &= msk; at |= bb;
        }
        return at;
    }

    bitset<64> wprecomputtedBishopMoves() {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(wBishop.GetBoard());
        for(int sq : indexes) { moves |= precomputtedBishopAttacks(sq); }
        return moves;
    }

    bitset<64> bprecomputtedBishopMoves() {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(bBishop.GetBoard());
        for(int sq : indexes) { moves |= precomputtedBishopAttacks(sq); }
        return moves;
    }

    bitset<64> precomputtedRookAttacks(int sq) {
        bitset<64> free = EmptyBoard().to_ullong();
        bitset<64> msk = maskRook[sq].to_ullong();   
        bitset<64> bb = horizVert[sq].to_ullong();   
        bitset<64> at = bb;          
        bitset<64> cl = at;          
        bitset<64> ov;                
                                    
        int repsLeft = 0; 
        bb &= free;
        bb >>= 8;
        switch (repsR[sq]) {
            case 8:
                bitsetMultiply(bb, rMutl8);
                repsLeft = 5;
                break;
            case 7:
                bitsetMultiply(bb, rMutl7);
                repsLeft = 5;
                break;
            case 6:
                bitsetMultiply(bb, rMutl3456);
                bb &= ~(1 << (sq - 6));      
                repsLeft = 5;
                break;
            case 5:
                bitsetMultiply(bb, rMutl3456);
                ov = (1 << (sq + 3));
                bb |= (bb & ov) >> 1;
                bb &= ~ov;
                bb &= ~(1 << (sq - 6));      
                repsLeft = 4;
                break;
            case 4:
                bitsetMultiply(bb, rMutl3456);
                ov = (1 << (sq + 3));
                bb |= (bb & ov) >> 1;
                bb &= ~ov;
                repsLeft = 3;
                break;
            case 3:
                bitsetMultiply(bb, rMutl3456);
                ov = (1 << (sq + 3));
                bb |= (bb & ov) >> 1;
                bb &= ~ov;
                repsLeft = 2;
                break;
        }
        bb &= msk; at |= bb;
        while(repsLeft-- > 0)
        {
            bb &= free; bb &= ~cl;
            cl = at;
            bb >>= 8; bitsetMultiply(bb, rMutl3456);; bb &= msk; at |= bb;
        }
        if ( sq < 8 ) // or do this before the switch
        {
            int a1a8 = free.to_ullong();
            at |= rankR[sq][a1a8 & 0x7e];
        }
        return at;
    }

    bitset<64> wprecomputtedRookMoves() {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(wRook.GetBoard());
        for(int sq : indexes) { moves |= precomputtedRookAttacks(sq); }
        return moves;
    }

    bitset<64> bprecomputtedRookMoves() {
        bitset<64> moves;
        vector<int> indexes = BitSetTrueIndexes(bRook.GetBoard());
        for(int sq : indexes) { moves |= precomputtedRookAttacks(sq); }
        return moves;
    }


    //uint64_t
    uint64_t precomputtedBishopAttacks_u64(int sq) {
        uint64_t free = EmptyBoard().to_ullong();
        uint64_t msk = maskBishop[sq].to_ullong();
        uint64_t bb = diagonals[sq].to_ullong();
        uint64_t at = bb;
        bb &= free;
        switch ( repsB[sq] )
        {
        case 6:
            bb >>= 9; bb *= U64_bMult; bb &= msk; at |= bb; bb &= free;
        case 5:
            bb >>= 9; bb *= U64_bMult; bb &= msk; at |= bb; bb &= free;
        case 4:
            bb >>= 9; bb *= U64_bMult; bb &= msk; at |= bb; bb &= free;
        case 3:
            bb >>= 9; bb *= U64_bMult; bb &= msk; at |= bb; bb &= free;
            bb >>= 9; bb *= U64_bMult; bb &= msk; at |= bb; bb &= free;
            bb >>= 9; bb *= U64_bMult; bb &= msk; at |= bb;
        }
        return at;
    }

    uint64_t wprecomputtedBishopMoves_u64() {
        uint64_t moves;
        vector<int> indexes = BitSetTrueIndexes(wBishop.GetBoard());
        for(int sq : indexes) { moves |= precomputtedBishopAttacks_u64(sq); }
        return moves;
    }

    uint64_t bprecomputtedBishopMoves_u64() {
        uint64_t moves;
        vector<int> indexes = BitSetTrueIndexes(bBishop.GetBoard());
        for(int sq : indexes) { moves |= precomputtedBishopAttacks_u64(sq); }
        return moves;
    }

    uint64_t precomputtedRookAttacks_u64(int sq) {
        uint64_t free = EmptyBoard().to_ullong();
        uint64_t msk = maskRook[sq].to_ullong();   
        uint64_t bb = horizVert[sq].to_ullong();   
        uint64_t at = bb;          
        uint64_t cl = at;          
        uint64_t ov;                
                                    
        int repsLeft = 0; 
        bb &= free;
        bb >>= 8;
        switch (repsR[sq]) {
            case 8:
                bb *= U64_rMutl8;
                repsLeft = 5;
                break;
            case 7:
                bb *= U64_rMutl7;
                repsLeft = 5;
                break;
            case 6:
                bb *= U64_rMutl3456;
                bb &= ~(1 << (sq - 6));      
                repsLeft = 5;
                break;
            case 5:
                bb *= U64_rMutl3456;
                ov = (1 << (sq + 3));
                bb |= (bb & ov) >> 1;
                bb &= ~ov;
                bb &= ~(1 << (sq - 6));      
                repsLeft = 4;
                break;
            case 4:
                bb *= U64_rMutl3456;
                ov = (1 << (sq + 3));
                bb |= (bb & ov) >> 1;
                bb &= ~ov;
                repsLeft = 3;
                break;
            case 3:
                bb *= U64_rMutl3456;
                ov = (1 << (sq + 3));
                bb |= (bb & ov) >> 1;
                bb &= ~ov;
                repsLeft = 2;
                break;
        }
        bb &= msk; at |= bb;
        while(repsLeft-- > 0)
        {
            bb &= free; bb &= ~cl;
            cl = at;
            bb >>= 8; bb *= U64_rMutl3456; bb &= msk; at |= bb;
        }
        if ( sq < 8 ) // or do this before the switch
        {
            int a1a8 = free;
            at |= rankR[sq][a1a8 & 0x7e];
        }
        return at;
    }

    uint64_t wprecomputtedRookMoves_u64() {
        uint64_t moves;
        vector<int> indexes = BitSetTrueIndexes(wRook.GetBoard());
        for(int sq : indexes) { moves |= precomputtedRookAttacks_u64(sq); }
        return moves;
    }

    uint64_t bprecomputtedRookMoves_u64() {
        uint64_t moves;
        vector<int> indexes = BitSetTrueIndexes(bRook.GetBoard());
        for(int sq : indexes) { moves |= precomputtedRookAttacks_u64(sq); }
        return moves;
    }


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