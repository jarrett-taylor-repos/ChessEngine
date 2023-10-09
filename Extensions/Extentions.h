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

    vector<string> SquareToUCI(vector<string> start, vector<string> end) {
        vector<string> ucimoves;
        for(int i = 0; i < start.size(); i++) {
            string temp = start[i] + end[i];
            ucimoves.push_back(temp);
        }
        return ucimoves;
    }

    vector<string> SquareToUCI(string start, vector<string> end) {
        vector<string> ucimoves;
        for(int i = 0; i < end.size(); i++) {
            string temp = start + end[i];
            ucimoves.push_back(temp);
        }
        return ucimoves;
    }

    vector<string> BitsetToUCI(bitset<64> a, bitset<64> b) {
        vector<string> start = BoardPopulatedSquares(a);
        vector<string> end = BoardPopulatedSquares(b);
        vector<string> ucimoves;
        for(int i = 0; i < start.size(); i++) {
            string temp = start[i] + end[i];
            ucimoves.push_back(temp);
        }
        return ucimoves;
    }

    vector<string> BitsetToUCI(string start, bitset<64> b) {
        vector<string> end = BoardPopulatedSquares(b);
        vector<string> ucimoves;
        for(int i = 0; i < end.size(); i++) {
            string temp = start + end[i];
            ucimoves.push_back(temp);
        }
        return ucimoves;
    }

    vector<string> Combine(vector<string> a, vector<string> b) {
        vector<string> ab = a;
        ab.insert(ab.end(), b.begin(), b.end());
        return ab;
    }

    void Append(vector<string> v1, vector<string> v2) {
        v1.insert(v1.end(), v2.begin(), v2.end());
    }

    void PrintVector(vector<string> a, string name = "") {
        if(name.length() != 0) {
            cout << name << endl;
        }
        for(string str : a) {
            cout << str << endl;
        }
        cout << endl;
    }
}