#include "U64_BitBoardHelper.cpp"
#include "..\MoveEngine\PrecomputtedMoves.h"
using namespace PrecomputtedMoves;

class U64_Bitboard {
    private:
    U64_BitBoardHelper wPawn;
    U64_BitBoardHelper wKnight;
    U64_BitBoardHelper wBishop;
    U64_BitBoardHelper wRook;
    U64_BitBoardHelper wQueen;
    U64_BitBoardHelper wKing;

    U64_BitBoardHelper bPawn;
    U64_BitBoardHelper bKnight;
    U64_BitBoardHelper bBishop;
    U64_BitBoardHelper bRook;
    U64_BitBoardHelper bQueen;
    U64_BitBoardHelper bKing;

    public:
    U64_Bitboard() { LoadFen(startFen); };
    U64_Bitboard(string fen) { LoadFen(fen); };

    void LoadFen(string fen) {
        ClearBoard();

        wPawn.LoadFen(fen, 0b1001);
        wKnight.LoadFen(fen, 0b1010);
        wBishop.LoadFen(fen, 0b1011);
        wRook.LoadFen(fen, 0b1100);
        wQueen.LoadFen(fen, 0b1101);
        wKing.LoadFen(fen, 0b1110);

        bPawn.LoadFen(fen, 0b0001);
        bKnight.LoadFen(fen, 0b0010);
        bBishop.LoadFen(fen, 0b0011);
        bRook.LoadFen(fen, 0b0100);
        bQueen.LoadFen(fen, 0b0101);
        bKing.LoadFen(fen, 0b0110);
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

    U64_BitBoardHelper GetwPawn() { return wPawn; };
    U64_BitBoardHelper GetwKnight() { return wKnight; };
    U64_BitBoardHelper GetwBishop() { return wBishop; };
    U64_BitBoardHelper GetwRook() { return wRook; };
    U64_BitBoardHelper GetwQueen() { return wQueen; };
    U64_BitBoardHelper GetwKing() { return wKing; };
    U64_BitBoardHelper GetbPawn() { return bPawn; };
    U64_BitBoardHelper GetbKnight() { return bKnight; };
    U64_BitBoardHelper GetbBishop() { return bBishop; };
    U64_BitBoardHelper GetbRook() { return bRook; };
    U64_BitBoardHelper GetbQueen() { return bQueen; };
    U64_BitBoardHelper GetbKing() { return bKing; };

    //board functions
    U64 wBoard() { return wPawn.GetBoard() | wKnight.GetBoard() | wBishop.GetBoard() | wRook.GetBoard() | wQueen.GetBoard() | wKing.GetBoard(); };
    U64 bBoard() { return bPawn.GetBoard() | bKnight.GetBoard() | bBishop.GetBoard() | bRook.GetBoard() | bQueen.GetBoard() | bKing.GetBoard(); };
    U64 AllBoard() { return wBoard() | bBoard(); };

    U64 EmptyBoard() { return ~AllBoard(); };
    U64 NotbBoard() { return ~bBoard(); };
    U64 NotwBoard() { return ~wBoard(); };

    //generic movement 
    U64 northOne(U64 b) { return b >> 8; };
    U64 southOne(U64 b) { return b << 8; };
    U64 eastOne(U64 b) { return b << 1 & U64_notAFile; };
    U64 westOne(U64 b) { return b >> 1 & U64_notHFile; };
    U64 northEastOne(U64 b) { return b >> 7 & U64_notAFile; };
    U64 northWestOne(U64 b) { return b >> 9 & U64_notHFile; };
    U64 southEastOne(U64 b) { return b << 9 & U64_notAFile; };
    U64 southWestOne(U64 b) { return b << 7 & U64_notHFile; };
    bool InBounds(int offset) { return (offset >= 0) && (offset < 64); };

    //pawn functions
    U64 wSinglePushTargets(U64 b) { return northOne(b) & EmptyBoard(); };
    U64 wDoublePushTargets(U64 b) { return northOne(wSinglePushTargets(b)) & EmptyBoard() & U64_rank4; };
    U64 bSinglePushTargets(U64 b) { return southOne(b) & EmptyBoard(); };
    U64 bDoublePushTargets(U64 b) { return southOne(bSinglePushTargets(b)) & EmptyBoard() & U64_rank5; };
    U64 wPawnPushes() { return wSinglePushTargets(wPawn.GetBoard()) | wDoublePushTargets(wPawn.GetBoard()); };
    U64 bPawnPushes() { return bSinglePushTargets(bPawn.GetBoard()) | bDoublePushTargets(bPawn.GetBoard()); };

    U64 wPawnWestAtt() { return (wPawn.GetBoard() >> 9) & U64_notHFile; };
    U64 wPawnEastAtt() { return (wPawn.GetBoard() >> 7) & U64_notAFile; };
    U64 wPawnAllAtt() { return wPawnEastAtt() | wPawnWestAtt(); };
    U64 bPawnWestAtt() { return (bPawn.GetBoard() << 9) & U64_notAFile; };
    U64 bPawnEastAtt() { return (bPawn.GetBoard() << 7) & U64_notHFile; };
    U64 bPawnAllAtt() { return bPawnEastAtt() | bPawnWestAtt(); };
    U64 wPawnWestCaptures() { return wPawnWestAtt() & bBoard(); };
    U64 wPawnEastCaptures() { return wPawnEastAtt() & bBoard(); };
    U64 wPawnAllCaptures() { return wPawnEastCaptures() | wPawnWestCaptures(); };
    U64 bPawnWestCaptures() { return bPawnWestAtt() & wBoard(); };
    U64 bPawnEastCaptures() { return bPawnEastAtt() & wBoard(); };
    U64 bPawnAllCaptures() { return bPawnEastCaptures() | bPawnWestCaptures(); };

    U64 wPawnMoves() { return wPawnAllCaptures() | wPawnPushes(); }
    U64 bPawnMoves() { return bPawnAllCaptures() | bPawnPushes(); }


    //knight functions
    U64 KnightMoveHelper(U64 b) { return noNoEa(b) | noEaEa(b) | soEaEa(b) | soSoEa(b) | noNoWe(b) | noWeWe(b) | soWeWe(b) | soSoWe(b); }
    U64 noNoEa(U64 b) {return (b >> 15) & U64_notAFile; }
    U64 noEaEa(U64 b) {return (b >> 6) & (U64_notAFile & U64_notBFile);}
    U64 soEaEa(U64 b) {return (b << 10) & (U64_notAFile & U64_notBFile);}
    U64 soSoEa(U64 b) {return (b << 17) & U64_notAFile; }
    U64 soSoWe(U64 b) {return (b << 15) & U64_notHFile; }
    U64 soWeWe(U64 b) {return (b << 6) & (U64_notGFile & U64_notHFile); }
    U64 noWeWe(U64 b) {return (b >> 10) & (U64_notGFile & U64_notHFile); }
    U64 noNoWe(U64 b) {return (b >> 17) & U64_notHFile; }

    U64 wKnightMoves(){ return KnightMoveHelper(wKnight.GetBoard()) & NotwBoard(); }
    U64 bKnightMoves(){ return KnightMoveHelper(bKnight.GetBoard()) & NotbBoard(); }

    //king moves
    U64 bKingMoves() { 
        U64 b = bKing.GetBoard();
        return (northOne(b) | southOne(b) | eastOne(b) | westOne(b) | northEastOne(b) | northWestOne(b) | southEastOne(b) | southWestOne(b)) & NotbBoard();
    };

    U64 wKingMoves() { 
        U64 b = wKing.GetBoard();
        return (northOne(b) | southOne(b) | eastOne(b) | westOne(b) | northEastOne(b) | northWestOne(b) | southEastOne(b) | southWestOne(b)) & NotwBoard();
    };

    //sliding moves helper
    bool ValidTravel(U64 overlap, int offset) { return InBounds(offset) && get_bit(overlap, offset) && get_bit(!AllBoard(), offset); };
    bool ValidTravelAtt(U64 overlap, int offset) { return InBounds(offset) && get_bit(overlap, offset) && get_bit(AllBoard(), offset); };

    U64 SlidingAttacks(U64 overlap, int sq, int direction) {
        U64 moves;
        int offset = sq + direction;
        while(ValidTravel(overlap, offset)) {
            get_bit(moves, offset);
            offset += direction;
        }
        if(ValidTravelAtt(overlap, offset)) {
            get_bit(moves, offset);
        }
        return moves;
    }

    U64 BishopAttacks(U64 bishopBoard) {
        U64 moves;
        vector<int> indexes = U64TrueIndexes(bishopBoard);
        for(int sq : indexes) {
            //travel northEastOne
            moves |= SlidingAttacks(U64_notAFile, sq, -7);
            //travel northWestOne
            moves |= SlidingAttacks(U64_notHFile, sq, -9);
            //travel southEastOne
            moves |= SlidingAttacks(U64_notAFile, sq, 9);
            //travle southWestOne
            moves |= SlidingAttacks(U64_notHFile, sq, 7);
        }
        return moves;
    }

    U64 RookAttacks(U64 rBoard) {
        U64 moves;
        vector<int> indexes = U64TrueIndexes(rBoard);
        for(int sq : indexes) {
            //travel northOne
            moves |= SlidingAttacks(U64_notRank1, sq, -8);
            //travel eastOne
            moves |= SlidingAttacks(U64_notAFile, sq, 1);
            //travel westOne
            moves |= SlidingAttacks(U64_notAFile, sq, -1);
            //travle southOne
            moves |= SlidingAttacks(U64_notRank8, sq, 8);
        }
        return moves;
    }

    //sliding moves
    U64 wBishopAttacks() { return BishopAttacks(wBishop.GetBoard()); };
    U64 bBishopAttacks() { return BishopAttacks(bBishop.GetBoard()); };
    U64 wBishopMoves() { U64 wbatt = wBishopAttacks(); return wbatt & ~(wbatt & wBoard()); };
    U64 bBishopMoves() { U64 bbatt = bBishopAttacks(); return bbatt & ~(bbatt & bBoard()); };

    U64 wRookAttacks() { return RookAttacks(wRook.GetBoard()); };
    U64 bRookAttacks() { return RookAttacks(bRook.GetBoard()); };
    U64 wRookMoves() { U64 wratt = wRookAttacks(); return wratt & ~(wratt & wBoard()); };
    U64 bRookMoves() { U64 bratt = bRookAttacks(); return bratt & ~(bratt & bBoard()); };

    U64 wQueenAttacks() { return RookAttacks(wQueen.GetBoard()) | BishopAttacks(wQueen.GetBoard()); };
    U64 bQueenAttacks() { return RookAttacks(bQueen.GetBoard()) | BishopAttacks(bQueen.GetBoard()); };
    U64 wQueenMoves() { U64 wqatt = wQueenAttacks(); return wqatt & ~(wqatt & wBoard()); };
    U64 bQueenMoves() { U64 bqatt = bQueenAttacks(); return bqatt & ~(bqatt & bBoard()); };

    //attacks
    U64 wAttacks() { return wPawnAllAtt() | wKingMoves() | wKnightMoves() | wQueenAttacks() | wBishopAttacks() | wRookAttacks(); };
    U64 bAttacks() { return bPawnAllAtt() | bKingMoves() | bKnightMoves() | bQueenAttacks() | bBishopAttacks() | bRookAttacks(); };

    //precomputted 
    U64 precomputtedBishopAttacks(int sq) {
        U64 free = EmptyBoard();
        U64 msk = maskBishop[sq].to_ullong();
        U64 bb  = diagonals[sq].to_ullong();
        U64 at  = bb;
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

    U64 wprecomputtedBishopMoves() {
        U64 moves;
        vector<int> indexes = U64TrueIndexes(wBishop.GetBoard());
        for(int sq : indexes) { moves |= precomputtedBishopAttacks(sq); }
        return moves;
    }

    U64 bprecomputtedBishopMoves() {
        U64 moves;
        vector<int> indexes = U64TrueIndexes(bBishop.GetBoard());
        for(int sq : indexes) { moves |= precomputtedBishopAttacks(sq); }
        return moves;
    }

    U64 precomputtedRookAttacks(int sq) {
        U64 free = EmptyBoard();
        U64 msk = maskRook[sq].to_ullong();   
        U64 bb = horizVert[sq].to_ullong();   
        U64 at = bb;          
        U64 cl = at;          
        U64 ov;                
                                    
        int repsLeft = 0; 
        bb &= free;
        bb >>= 8;
        switch (repsR[sq]) {
            case 8:
                bb *= rMutl8.to_ullong();
                repsLeft = 5;
                break;
            case 7:
                bb *= rMutl7.to_ullong();
                repsLeft = 5;
                break;
            case 6:
                bb *= rMutl3456.to_ullong();
                bb &= ~(1 << (sq - 6));      
                repsLeft = 5;
                break;
            case 5:
                bb *= rMutl3456.to_ullong();
                ov = (1 << (sq + 3));
                bb |= (bb & ov) >> 1;
                bb &= ~ov;
                bb &= ~(1 << (sq - 6));      
                repsLeft = 4;
                break;
            case 4:
                bb *= rMutl3456.to_ullong();
                ov = (1 << (sq + 3));
                bb |= (bb & ov) >> 1;
                bb &= ~ov;
                repsLeft = 3;
                break;
            case 3:
                bb *= rMutl3456.to_ullong();
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
            bb >>= 8; bb *= rMutl3456.to_ullong(); bb &= msk; at |= bb;
        }
        if ( sq < 8 ) // or do this before the switch
        {
            int a1a8 = free;
            at |= rankR[sq][a1a8 & 0x7e];
        }
        return at;
    }

    U64 wprecomputtedRookMoves() {
        U64 moves;
        vector<int> indexes = U64TrueIndexes(wRook.GetBoard());
        for(int sq : indexes) { moves |= precomputtedRookAttacks(sq); }
        return moves;
    }

    U64 bprecomputtedRookMoves() {
        U64 moves;
        vector<int> indexes = U64TrueIndexes(bRook.GetBoard());
        for(int sq : indexes) { moves |= precomputtedRookAttacks(sq); }
        return moves;
    }


    //misc
    void PrintAllBoards() {
        wPawn.PrintU64();
        bPawn.PrintU64();
        wKnight.PrintU64();
        bKnight.PrintU64();
        wBishop.PrintU64();
        bBishop.PrintU64();
        wRook.PrintU64();
        bRook.PrintU64();
        wQueen.PrintU64();
        bQueen.PrintU64();
        wKing.PrintU64();
        bKing.PrintU64();
    }
};