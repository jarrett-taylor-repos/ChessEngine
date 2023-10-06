#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
typedef unsigned long long U64;
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

    void bit_set(U64& number, U64 n) { 
        number |= U64(1) << n; 
    };
    bool get_bit(U64 number, U64 n) { return (number >> n) & (U64)1; };
    void bit_clear(U64& number, U64 n) { number &= ~((U64)1 << n); };

    bool fullAdder(bool b1, bool b2, bool& carry) {
        bool sum = (b1 ^ b2) ^ carry;
        carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
        return sum;
    };

    void bitsetAdd(bitset<64>& x, const bitset<64>& y) {
        bool carry = false;
        for (int i = 0; i < 64; i++) {
            x[i] = fullAdder(x[i], y[i], carry);
        }
    };

    void bitsetMultiply(bitset<64>& x, const bitset<64>& y) {
        bitset<64> tmp = x;
        x.reset();
        // we want to minimize the number of times we shift and add
        for (int i=0; i < 64; i++)
            if (tmp[i]) bitsetAdd(x, y << i);
    };

    void PrintU64(U64 u, string name = "") {
        if(name.length() != 0) cout << name << endl;

        for(int i = 0; i < 64; i++) {
            if(i % 8 == 0 && i != 0) cout << endl;
            if(get_bit(u, i)) {
                cout << "1" << " ";
            } else {
                cout << "." << " ";
            }
        }
        cout << endl << endl;
    }

    void PrintBitSet(bitset<64> board, string name = "") {
        if(name.length() != 0) {
            cout << name << endl;
        }
        for(int i = 0; i < 64; i++) {
            if(i % 8 == 0 && i != 0) {
                cout << endl;
            }
            if(board.test(i)) {
                cout << "1" << " ";
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

    vector<int> U64TrueIndexes(U64 b) {
        vector<int> indexes;
        for(int i = 0; i < 64; i++) {
            if(get_bit(b, i)) {
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