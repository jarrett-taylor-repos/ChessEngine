#include "../../src/BitBoard/BitBoard.cpp"

class MoveEval {
    private:
    int move;
    int eval = -999999;
    bool hasBoard = false;
    Bitboard b;

    public:
    MoveEval(int moveInput) {
        move = moveInput;
    }

    MoveEval(int moveInput, Bitboard &inputb, int inputeval) {
        move = moveInput;
        b = inputb;
        hasBoard=true;
        eval=inputeval;
    }

    MoveEval(int moveInput, int inputeval) {
        move = moveInput;
        hasBoard=true;
        eval=inputeval;
    }

    void AssignBoard(Bitboard &inputb) {
        b = inputb;
        hasBoard=true;
    };

    int GetMove() { return move; };
    int GetEval() { return eval; };
    bool HasBoard() { return hasBoard; };
    Bitboard GetBoard() { return b; }; 
};