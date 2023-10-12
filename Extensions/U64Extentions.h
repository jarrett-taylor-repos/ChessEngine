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

    void Print(U64 b, string name = "") {
        if(name.length() != 0) { cout << name << endl;}

        for(int i = 0; i < 64; i++) {
            if(i != 0 && (i % 8) == 0) { cout << endl; }

            TestBit(b, i) ? cout << "1 " : cout << ". ";
        }
        cout << endl << endl;
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

    void PrintVectorInt(vector<int> vect, string name = "") {
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

    vector<string> BoardPopulatedSquares(U64 b) {
        vector<string> coordinates;
        vector<int> indexes = GetTrueBits(b);
        for(int sq : indexes) {
            string enSq = IndexToSquare(sq);
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

    vector<string> BitsetToUCI(U64 a, U64 b, bool isPawn = false, bool isWhite = false) {
        vector<string> start = BoardPopulatedSquares(a);
        vector<string> end = BoardPopulatedSquares(b);
        vector<string> ucimoves;
        for(int i = 0; i < start.size(); i++) {
            string temp;

            bool isPromo = isPawn && ((isWhite && StringtoIndex(end[i]) < 8) || (!isWhite && StringtoIndex(end[i]) > 55));
            if(isPromo) {
                for(int j = 0; j < promotionUci.size(); j++) {
                    temp = start[i] + end[i] + promotionUci[j];
                    ucimoves.push_back(temp);
                }
            } else {
                temp = start[i] + end[i];
                ucimoves.push_back(temp);
            }
        }
        return ucimoves;
    }

    vector<string> BitsetToUCI(string start, U64 b) {
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