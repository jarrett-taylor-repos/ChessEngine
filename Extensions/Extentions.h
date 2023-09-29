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

    void PrintBitSet(bitset<64> board) {
        for(int i = 0; i < 64; i++) {
            if(i % 8 == 0 && i != 0) {
                cout << endl;
            }
            cout << board[i] << " ";
        }
    };
}