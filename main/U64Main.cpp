#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    // U64Bitboard b("rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2");
    // cout << b.GetZobrist() <<  " " << b.GetFen() << endl;
    U64Bitboard test(startFen);
    Moves move_list;
    test.GenerateMoves(move_list);
    int move = GetMoveByUci(move_list, "e2e3 ");
    test.MakeMove(move);
    test.PrintZobristAndFields();

    U64Bitboard test2("rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
    test2.PrintZobristAndFields();

    /* ZOBRIST DRAW 
        U64Bitboard temp(startFen);
        temp.PrintZobristAndFields();
        Moves move_list;
        temp.GenerateMoves(move_list);
        int move = GetMoveByUci(move_list, "e2e3 ");
        cout << "Move made: " << GetMoveUci(move) << " " << temp.MakeMove(move) << endl;
        temp.PrintZobristAndFields();

        temp.GenerateMoves(move_list);
        move = GetMoveByUci(move_list, "e7e6 ");
        cout << "Move made: " << GetMoveUci(move) << " " << temp.MakeMove(move) << endl;
        temp.PrintZobristAndFields();

        temp.GenerateMoves(move_list);
        move = GetMoveByUci(move_list, "e1e2 ");
        cout << "Move made: " << GetMoveUci(move) << " " << temp.MakeMove(move) << endl;
        temp.PrintZobristAndFields();
        
        temp.GenerateMoves(move_list);
        move = GetMoveByUci(move_list, "e8e7 ");
        cout << "Move made: " << GetMoveUci(move) << " " << temp.MakeMove(move) << endl;
        temp.PrintZobristAndFields();
        
        temp.GenerateMoves(move_list);
        move = GetMoveByUci(move_list, "e2e1 ");
        cout << "Move made: " << GetMoveUci(move) << " " << temp.MakeMove(move) << endl;
        temp.PrintZobristAndFields();

        temp.GenerateMoves(move_list);
        move = GetMoveByUci(move_list, "e7e8 ");
        cout << "Move made: " << GetMoveUci(move) << " " << temp.MakeMove(move) << endl;
        temp.PrintZobristAndFields();

        temp.GenerateMoves(move_list);
        move = GetMoveByUci(move_list, "e1e2 ");
        cout << "Move made: " << GetMoveUci(move) << " " << temp.MakeMove(move) << endl;
        temp.PrintZobristAndFields();

        temp.GenerateMoves(move_list);
        move = GetMoveByUci(move_list, "e8e7 ");
        cout << "Move made: " << GetMoveUci(move) << " " << temp.MakeMove(move) << endl;
        temp.PrintZobristAndFields();
    */

}