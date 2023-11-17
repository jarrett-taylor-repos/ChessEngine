#include "..\Bitboard\Bitboard.cpp"

long long nodes;

static inline void perft_driver(Bitboard b, int depth) {
    if (depth == 0) { nodes++; return; }
    
    Moves move_list;
    b.GenerateMoves(move_list); 

    for (int i = 0; i < move_list.GetCount(); i++) { 
        Bitboard temp = b;
        if (!temp.MakeMove(move_list.GetMove(i))) continue;
        
        perft_driver(temp, depth - 1);
    }
}

// perft test
void perft_test(Bitboard b, int depth) {
    printf("\n     Performance test\n\n");
    
    Moves move_list;
    b.GenerateMoves(move_list); 
    
    long start = clock();

    for (int i = 0; i < move_list.GetCount(); i++) { 
        Bitboard temp = b;
        if (!temp.MakeMove(move_list.GetMove(i))) continue;
        
        long cummulative_nodes = nodes;
        perft_driver(temp, depth - 1);
        long old_nodes = nodes - cummulative_nodes;
        
        PrintMoveUci(move_list.GetMove(i), old_nodes);
    }
    
    printf("\n    Depth: %d\n", depth);
    printf("    Nodes: %ld\n", nodes);
    printf("    Time(ms): %ld\n\n", clock() - start);
}

int main() {
    InitAll();
    string testPos = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    // 1 - 44
    // 2 - 1,486
    // 3 - 62,379
    // 4 - 2,103,487
    // 5 - 89,941,194
    Bitboard b;
    //b.LoadFen(testPos);
    b.LoadFen(startFen);

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
    perft_test(b, depth);
}
