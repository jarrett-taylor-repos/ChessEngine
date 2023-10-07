#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;

namespace Extensions {
    vector<string> Split(string str, const char delim) {
        vector<string> out;
        stringstream ss(str); 

        string s; 
        while (getline(ss, s, delim)) { 
            out.push_back(s); 
        } 
        return out;
    };

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
            if(board[i] == 1) {
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
            if(b[i] == 1) {
                indexes.push_back(i);
            }
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
}