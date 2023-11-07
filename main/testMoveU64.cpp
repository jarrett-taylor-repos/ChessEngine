#include "..\Bitboard\U64Bitboard.cpp"

int moveGenerationTest(U64Bitboard b, int depth) {
    if(depth == 0) return 1;

    int numPos = 0;
    Moves move_list;
    b.GeneratebbMoves(move_list);

    for(int i = 0; i < move_list.GetCount(); i++){
        U64Bitboard temp = b;
        bool mademove = temp.MakeMove(move_list.GetMove(i));
        if(mademove) {
            numPos += moveGenerationTest(temp, depth-1);
        }
    }
    return numPos;
}

int main() {
    InitAll();
    string testPos = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    // 1 - 44
    // 2 - 1,486
    // 3 - 62,379
    // 4 - 2,103,487
    // 5 - 89,941,194
    U64Bitboard b(startFen);
    b.LoadFen(testPos);

    // 1	20
    // 2	400
    // 3	8,902
    // 4	197,281
    // 5	4,865,609
    // 6	119,060,324
    // 7	3,195,901,860
    // 8	84,998,978,956
    // 9	2,439,530,234,167
    // 10	69,352,859,712,417
    int depth = 5;
    int nummoves = 0;
    clock_t tStart = clock();
    cout << "Depth - num" << endl;
    for(int i = 1; i <= depth; i++) {
        nummoves =  moveGenerationTest(b, i);
        cout << i << " - " << nummoves << endl;
        printf("Time taken: %.4fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    }
    return 0;
}