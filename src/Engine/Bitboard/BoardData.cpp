#pragma once
#include "..\Utils\Extensions\Extensions.h"
using namespace Extensions;

class BoardData {
    private:
    int wKingSq;
    int bKingSq;

    int halfMoveClock;
    int fullTurnNum;
    int enPassantTarget;
    int castlingRights;
    bool isWhiteMove;
    bool isMoveRepetition;

    int evaluation;

    U64 zobrist;
    U64 zobristTable[50];
    int zobristTableIndex;

    public:
    ~BoardData() { Clear(); };
    BoardData() { Clear(); };
    BoardData(const BoardData& other) {
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

        //zobrist
        this->zobristTableIndex = other.zobristTableIndex;
        for(int i = 0; i <= zobristTableIndex; i++) this->zobristTable[i] = other.zobristTable[i];
        this->zobrist = other.zobrist;
    };

    BoardData& operator=(const BoardData& other) {
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

        //zobrist
        this->zobristTableIndex = other.zobristTableIndex;
        for(int i = 0; i <= zobristTableIndex; i++) this->zobristTable[i] = other.zobristTable[i];
        this->zobrist = other.zobrist;

        return *this;
    };

    bool operator==(const BoardData& other) {
         bool zobIndexMatch = this->zobristTableIndex == other.zobristTableIndex;
        if(!zobIndexMatch) return false;

        for(int i = 0; i <= this->zobristTableIndex; i++) {
            bool zobristTableEqual = this->zobristTable[i] == other.zobristTable[i];
            if(!zobristTableEqual) return false;
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
    };


    void Clear() {
        Reset(zobrist);

        ClearZobristTable();
        wKingSq = -1; bKingSq = -1;
        evaluation = 0;    
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

    //get
    U64 GetZobrist() { return zobrist; };
    int GetwKingSq() { return wKingSq; };
    int GetbKingSq() { return bKingSq; };
    int GetHalfMoveClock() { return halfMoveClock; };
    int GetFullTurnNum() { return fullTurnNum; };
    int GetEnPassantTarget() { return enPassantTarget; };
    int GetCastlingRights() { return castlingRights; };
    bool IsWhiteMove() { return isWhiteMove; };
    bool IsMoveRepition() { return isMoveRepetition; };
    int GetEvaluation() { return evaluation; };

    string GetMoveColorString() { return isWhiteMove ? "w" : "b"; }; 
    string GetCastlingRightsString() { return CastlingRightsString(castlingRights); };
    string GetEnPassantString() { return EnpassantTargetToString(enPassantTarget); };
    string GetHalfMoveString() { return to_string(halfMoveClock); };
    string GetFullTurnString() { return to_string(fullTurnNum); };
    string GetFenDataString() { return " " + GetMoveColorString() + " " + GetCastlingRightsString() + " " + GetEnPassantString() + " " + GetHalfMoveString() + " " + GetFullTurnString(); };

    //Set
    void SetwKingSq(int sq) { wKingSq = sq; };
    void SetbKingSq(int sq) { bKingSq = sq; };
    void SetEnpassantTarget(int sq) { enPassantTarget = sq; };

    void SetDataFromFen(vector<string> arguments) {
        isMoveRepetition = false;
        evaluation = 0;
        isWhiteMove = arguments[1] == "w";
        castlingRights = SetCastlingRights(arguments[2]);
        enPassantTarget = (arguments[3] == "-") ? 0 : StringtoIndex(arguments[3]);
        halfMoveClock = stoi(arguments[4]);
        fullTurnNum = stoi(arguments[5]);
    };

    //Update
    void AddMaterialValue(int piece, int sq) {
        AddEvaluation(PieceValue[piece]);
        UpdateEvaluation(piece < K, PieceSquareTables[piece][sq]);
    };

    void RemoveMaterialValue(int piece, int sq) {
        SubtractEvaluation(PieceValue[piece]);
        UpdateEvaluation(piece > K, PieceSquareTables[piece][sq]);
    };
    void AddEvaluation(int value) { evaluation += value; };
    void SubtractEvaluation(int value) { evaluation -= value; };
    void UpdateEvaluation(bool add, int value) { add ? AddEvaluation(value) : SubtractEvaluation(value); }; 

    void ChangeWhiteMove() { isWhiteMove = !isWhiteMove; };
    void UpdateHalfMoveClock(bool isPawnMoveOrCapture) { isPawnMoveOrCapture ? halfMoveClock = 0: halfMoveClock++; };
    void CheckFullTurnNum() { if(isWhiteMove) { fullTurnNum++; } };

    void ClearOrUpdateZobristTable(bool irreversibleMove) { irreversibleMove ? ClearZobristTable() : UpdateZobristTable(); };
    void XorZobrist(U64 value) { zobrist ^= value; }; 

    void UpdateCastlingRights(int startSq, int targetSq) {
        if((StartOrTargetEqual(startSq, targetSq, a8) || StartOrTargetEqual(startSq, targetSq, e8)) && (castlingRights & bq) ) { castlingRights ^= bq; SetCastlingZobrist(q); }
        if((StartOrTargetEqual(startSq, targetSq, h8) || StartOrTargetEqual(startSq, targetSq, e8)) && (castlingRights & bk) ) { castlingRights ^= bk; SetCastlingZobrist(k); }
        if((StartOrTargetEqual(startSq, targetSq, a1) || StartOrTargetEqual(startSq, targetSq, e1)) && (castlingRights & wq) ) { castlingRights ^= wq; SetCastlingZobrist(Q); }
        if((StartOrTargetEqual(startSq, targetSq, h1) || StartOrTargetEqual(startSq, targetSq, e1)) && (castlingRights & wk) ) { castlingRights ^= wk; SetCastlingZobrist(K); }
    };

    //helpers
    bool StartOrTargetEqual(int start, int target, int sq) { return start == sq || target == sq; };
    bool IsDraw() { return isMoveRepetition || (halfMoveClock >= 50); };

    //Print
    void PrintZobristAndFields() {
        string castlingStr = GetCastlingRightsString();
        printf("Zobrist: %llu, IsWhiteMove: %d, enPassantTarget: %d, CastlingRights %s, IsDraw: %d \n", zobrist, isWhiteMove, enPassantTarget, castlingStr.c_str(), IsDraw());
    };

    //zobrist 
    void SetZobristHashForSquareAndPiece(int boardSq, int piece) { zobrist ^= zobristPieceSquareNumber[boardSq][piece]; }

    int EnpassantZobristIndex() {
        if(enPassantTarget < 16 || enPassantTarget > 47) return -1;
        if(enPassantTarget < 24) return enPassantTarget - 16;
        return enPassantTarget - 32;
    };

    void SetZobristHashEnpassant() {
        int index = EnpassantZobristIndex();
        if(index == -1) return;
        zobrist ^= enpassantNumbers[index];
    };

    void SetZobristHashOnMakeMove() {
        if(isWhiteMove) {
            zobrist ^= moveNumbers[BLACK];
            zobrist ^= moveNumbers[WHITE];
        } else {
            zobrist ^= moveNumbers[WHITE];
            zobrist ^= moveNumbers[BLACK];
        }
    };

    void SetZobristHashForMove(bool onMakeMove = false) {
        if(onMakeMove) {
            SetZobristHashOnMakeMove();
            return;
        }

        zobrist ^= moveNumbers[isWhiteMove ? WHITE : BLACK];
    };

    int CastlingRightsToZobristIndex(int piece) {
        if(piece == K) return 0;
        if(piece == Q) return 1;
        if(piece == k) return 2;
        if(piece == q) return 3;
        return -1;
    };

    void SetCastlingZobrist(int piece) {
        int index = CastlingRightsToZobristIndex(piece);
        if(index == -1) return;
        zobrist ^= castlingNumbers[index];
    };

    void SetCastlingZobrist() {
        if(!castlingRights) return;

        if(castlingRights & wk) zobrist ^= castlingNumbers[0];
        if(castlingRights & wq) zobrist ^= castlingNumbers[1];
        if(castlingRights & bk) zobrist ^= castlingNumbers[2];
        if(castlingRights & bq) zobrist ^= castlingNumbers[3];
    };
};