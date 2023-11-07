#include "../../Bitboard/U64Bitboard.cpp"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void main() {
    InitAll();
    ifstream FENfile;
    ofstream outFile;

    FENfile.open("../test_files/validFENs.txt");
    outFile.open("../test_files/jert_possible_moves.txt");

    U64Bitboard b;
    
    if (FENfile.is_open()) {
        string FEN;
        int i = 1;
        while ( FENfile ) {
            getline(FENfile, FEN);
            outFile << FEN << "|";
            b.LoadFen(FEN);
            outFile<<b.GetFen()<<"|";

            Moves allMoves;
            b.GenerateMoves(allMoves);
            for(int i = 0; i < allMoves.GetCount(); i++){
                int move = allMoves.GetMove(i);
                U64Bitboard temp = b;
                if (!temp.MakeMove(move)) continue;
                outFile<<GetMoveUci(move)<<", ";
            }

            outFile<<endl;
            i++;
        }
        // cout<<i<<endl;
    }

}

// void main() {
//     InitAll();
//     U64Bitboard b;
//     b.LoadFen("r2qkbnr/ppp1pppp/2n5/3p4/3P1B2/4Pb2/PPP2PPP/RN1QKB1R w KQkq - 0 5");
//     Moves allMoves;
//     b.GenerateMoves(allMoves);
//     PrintMoveListUci(allMoves);
// }