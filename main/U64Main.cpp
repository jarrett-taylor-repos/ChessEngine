#include "..\Bitboard\U64Bitboard.cpp"

int main () {
    InitAll();
    // U64Bitboard b("rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2");
    // cout << b.GetZobrist() <<  " " << b.GetFen() << endl;
    // U64Bitboard test(startFen);
    // U64 zobristStartFen = test.GetZobrist();

    // Moves move_list;
    // test.GenerateMoves(move_list);
    // int move = GetMoveByUci(move_list, "e2e3 ");
    // test.MakeMove(move);

    // U64 zobristAfter_e2e3 = test.GetZobrist();

    // U64 zobristManualAfter_e2e3 = zobristStartFen;
    // zobristManualAfter_e2e3 ^= pieceNumbers[e2][P];
    // zobristManualAfter_e2e3 ^= pieceNumbers[e3][P];
    // cout << zobristStartFen << ", " << zobristAfter_e2e3 << ", " << zobristManualAfter_e2e3 << endl;


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

}