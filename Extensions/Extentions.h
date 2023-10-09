#include "BitboardConstants.h"
#include <sstream>
#include <vector>
using namespace std;
using namespace BitboardConstants;

namespace Extensions {

    vector<string> Split(string str, const char token = ' '){
        string tmp; 
        stringstream ss(str);
        vector<string> words;

        while(getline(ss, tmp, token))  {
            words.push_back(tmp);
        }
        return words;
    }

    short int CharToInt(char ch) {
        string str(1, ch);
        return stoi(str);
    };

    void PrintBitSet(bitset<64> board, string name = "") {
        if(name.length() != 0) {
            cout << name << endl;
        }
        for(int i = 0; i < 64; i++) {
            if(i % 8 == 0 && i != 0) {
                cout << endl;
            }
            if(board.test(i)) {
                cout << board[i] << " ";
            } else {
                cout << "." << " ";
            }
        }
        cout << endl << endl;
    };

    void PrintBitSetList(bitset<64>* board) {
        for(int sq = 0; sq < 64; sq++) {
            PrintBitSet(board[sq], to_string(sq));
        }
    };

    vector<int> BitSetTrueIndexes(bitset<64> b) {
        vector<int> indexes;
        for(int i = 0; i < 64; i++) {
            if(b.test(i)) indexes.push_back(i);
        }
        return indexes;
    }

    void PrintVectorInt(vector<int> vect, string name = "") {
        if(name.length() != 0) {
            cout << name << endl;
        }
        for(int i = 0; i < vect.size(); i++) {
            cout << to_string(vect[i]) << " ";
        }
        cout << endl << endl;
    }

    string IndexToEnumSquare(int index) { return IntToEnumMap.at(index); }

    vector<string> BoardPopulatedSquares(bitset<64> b) {
        vector<string> coordinates;
        vector<int> indexes = BitSetTrueIndexes(b);
        for(int sq : indexes) {
            string enSq = IndexToEnumSquare(sq);
            coordinates.push_back(enSq);
        }
        return coordinates;
    }
}