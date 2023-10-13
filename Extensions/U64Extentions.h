#include "U64BitboardConstants.h"
#include <chrono>
using namespace chrono;
using namespace std;
using namespace U64BitboardConstants;

namespace U64Extensions {
    void SetBit(U64 &b, int sq) {
        U64 singleBitset = C64(1) << sq;
        b |= singleBitset;
    }

    bool TestBit(U64 &b, int sq) {
        U64 singleBitset = C64(1) << sq;
        return (b & singleBitset);
    }

    void ResetBit(U64 &b, int sq) {
        U64 singleBitset = C64(1) << sq;
        b &= ~singleBitset;
    }

    void Reset(U64 &b) { b = 0; }

    U64 SingleBitBoard(int sq) {
        U64 temp = 0;
        SetBit(temp, sq);
        return temp;
    }

    void Print(U64 b, string name = "") {
        if(name.length() != 0) { cout << name << endl;}

        for(int i = 0; i < 64; i++) {
            if(i != 0 && (i % 8) == 0) { cout << endl; }

            TestBit(b, i) ? cout << "1 " : cout << ". ";
        }
        cout << endl << endl;
    }

    void Print(multimap<int, pair<int, char>> m, string name = "") {
        if(name.length() != 0) { cout << name << endl;}

        for(multimap<int, pair<int, char>>::const_iterator it = m.begin(); it != m.end(); ++it){
            cout << it->first << " " << it->second.first << " " << it->second.second << endl;
        }
        cout << endl << endl;
    }

    string CastlingRightsString(map<char, bool> m) {
        string temp = "";
        for(map<char, bool>::const_iterator it = m.begin(); it != m.end(); ++it){
            if(it->second) { temp += it->first; }
        }
        return temp;
    }

    map<char, bool> SetCastlingRights(string str) {
        map<char, bool> m = castlingRightsDefault;

        for(char c : str) {
            auto it = m.find(c);
            it->second = true;
        }
        return m;
    }

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

    void Print(vector<int> vect, string name = "") {
        if(name.length() != 0) { cout << name << endl; }
        for(int i = 0; i < vect.size(); i++) {
            cout << to_string(vect[i]) << " ";
        }
        cout << endl << endl;
    }

    vector<int> GetTrueBits(U64 b) {
        vector<int> indexes;
        for(int i = 0; i < 64; i++) {
            if(TestBit(b, i)) indexes.push_back(i);
        }
        return indexes;
    }

    string IndexToSquare(int index) { return IntToSquareMap.at(index); }
    int StringtoIndex(string str) {return SquaretoIntMap.at(str); }

    void U64ToMapMoves(multimap<int, pair<int, char>> &moves, int sq, U64 b, bool isPawnMove = false, bool isWhitePawn = false) {
        //Print(b, "U64ToMapMoves");
        vector<int> end = GetTrueBits(b);
        for(int i = 0; i < end.size(); i++) {
            bool isPromo = (isWhitePawn ? end[i] < 8 : end[i] > 55) && isPawnMove;
            if(isPromo) {
                for(int j = 0; j < promotionUci.length(); j++) {
                    char curr = promotionUci[j];
                    pair<int, char> chartemp = pair<int, char>(end[i], curr);
                    pair<int, pair<int, char>> temp = pair<int, pair<int, char>>(sq, chartemp);
                    moves.insert(temp);
                }
            } else {
                pair<int, char> chartemp = pair<int, char>(end[i], ' ');
                pair<int, pair<int, char>> temp = pair<int, pair<int, char>>(sq, chartemp);
                moves.insert(temp);
            }
        }
    }
}