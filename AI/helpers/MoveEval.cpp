class MoveEval {
    private:
    int move;
    int eval = -999999;
    public:
    MoveEval(int moveInput) {
        move = moveInput;
    }
    MoveEval(int moveInput, int inputeval) {
        move = moveInput;
        eval=inputeval;
    }
    int GetMove() { return move; };
    int GetEval() { return eval; };
};