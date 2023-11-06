#include "../../Bitboard/U64Bitboard.cpp"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void main() {
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

            multimap<int, pair<int, char>> m = b.GetMapMoves();
            for(multimap<int, pair<int, char>>::const_iterator it = m.begin(); it != m.end(); ++it){
                int sq = it->first;
                int end = it->second.first;
                char promo = it->second.second;
                outFile<<IndexToSquare(sq)<<IndexToSquare(end)<<promo<<", ";
            }

            outFile<<endl;
            i++;
        }
        // cout<<i<<endl;
    }

}

// void main() {
//     U64Bitboard b;
//     b.LoadFen("r1bq1rk1/pp1p1ppp/2n2n2/4p3/1b2P3/2NB1N2/PPP2PPP/R1BQK2R w KQ - 6 8");

//     multimap<int, pair<int, char>> m = b.GetMapMoves();
//     for(multimap<int, pair<int, char>>::const_iterator it = m.begin(); it != m.end(); ++it){
//         int sq = it->first;
//         int end = it->second.first;
//         char promo = it->second.second;
//         cout<<IndexToSquare(sq)<<IndexToSquare(end)<<promo<<endl;
//     }
// }