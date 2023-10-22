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

    void Print(vector<int> vect, string name = "") {
        if(name.length() != 0) { cout << name << endl; }
        for(int i = 0; i < vect.size(); i++) {
            cout << to_string(vect[i]) << " ";
        }
        cout << endl << endl;
    }

    void Print(map<int, U64> m, string name = "") {
        if(name.length() != 0) { cout << name << endl;}

        for(map<int, U64>::const_iterator it = m.begin(); it != m.end(); ++it){
            string pinnedMovesStr = "Pinned moves for " + to_string(it->first);
            Print(it->second, pinnedMovesStr);
        }
        cout << endl << endl;
    }

    void Print(map<string, int> m, string name = "") {
        if(name.length() != 0) { cout << name << endl;}

        for(map<string, int>::const_iterator it = m.begin(); it != m.end(); ++it){
            cout << it->first << " - " << to_string(it->second) << endl;
        }
        cout << endl << endl;
    }

    void Print(vector<string> uci, string name = "") {
        if(name.length() != 0) { cout << name << endl;}
        
        for(string move : uci) {
            cout << move << endl;
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

    int CastlingRightsToZobristIndex(char c) {
        switch(c) {
            case 'K': return 0;
            case 'Q': return 1;
            case 'k': return 2;
            case 'q': return 3;
        }
        return -1;
    }

    map<char, bool> SetCastlingRights(string str, U64 &zobrist) {
        map<char, bool> m = castlingRightsDefault;

        for(char c : str) {
            auto it = m.find(c);
            it->second = true;
            int index = CastlingRightsToZobristIndex(c);
            zobrist ^= castlingNumbers[index];
        }
        return m;
    }

    bool IsIntInVector(vector<int> v, int key) {
        return count(v.begin(), v.end(), key) > 0;
    }

    void SetCastlingRightsFalse(map<char, bool> &m, char c, U64 &zobrist) {
        auto it = m.find(c);
        it->second = false;
        int index = CastlingRightsToZobristIndex(c);
        zobrist ^= castlingNumbers[index];
    }

    bool GetCastlingRightsValueByChar(map<char, bool> m, char c) {
        auto it = m.find(c);
        return it->second;
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

    vector<int> GetTrueBits(U64 b) {
        vector<int> indexes;
        for(int i = 0; i < 64; i++) {
            if(TestBit(b, i)) indexes.push_back(i);
        }
        return indexes;
    }

    string IndexToSquare(int index) { return IntToSquareMap.at(index); }
    int StringtoIndex(string str) {return SquaretoIntMap.at(str); }

    string EnpassantTargetToString(int index) { 
        if(index == 0) return "-";
        return IndexToSquare(index);
    }

    void InsertPin(map<int, U64> &m, int blockerSq, U64 pinnedMoves) {
        pair<int, U64> pinnedToKing = pair<int, U64> (blockerSq, pinnedMoves); 
        m.insert(pinnedToKing);
    }
    
    void InsertCheck(map<int, vector<int>> &m, int key, vector<int> blocks) {
        pair<int, vector<int>> blocksForKing = pair<int, vector<int>> (key, blocks); 
        m.insert(blocksForKing);
    }

    void InsertMove(multimap<int, pair<int, char>> &m, int start, int target, char promo) {
        pair<int, char> helper = pair<int, char>(target, promo);
        pair<int, pair<int, char>> temp = pair<int, pair<int, char>>(start, helper); 
        m.insert(temp);
    }

    vector<int> GetBlocksFromChecks(map<int, vector<int>> &m, int key) {
        if (m.find(key) != m.end()) {
            return m[key];
        }
        return vector<int>{-1};
    }

    U64 GetPinnedMoves(map<int, U64> &m, int key) {
        if (m.find(key) != m.end()) {
            return m[key];
        }
        return Universe;
    }

    bool FindMoveInMap(multimap<int, pair<int, char>> m, int key) {
        if (m.find(key) != m.end()) return true;
        return false;
    }

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

    vector<string> MapMovesToUCI(multimap<int, pair<int, char>> m) {
        vector<string> uci;
        for(multimap<int, pair<int, char>>::const_iterator it = m.begin(); it != m.end(); ++it){
            string move = IndexToSquare(it->first) + IndexToSquare(it->second.first) + it->second.second;
            uci.push_back(move);
        }
        return uci;
    }

    //zobrist 
    int PieceToZobristIndex(char c) {
        switch(c) {
            case 'p': return 0;
            case 'b': return 1;
            case 'n': return 2;
            case 'r': return 3;
            case 'q': return 4;
            case 'k': return 5;
            case 'P': return 6;
            case 'B': return 7;
            case 'N': return 8;
            case 'R': return 9;
            case 'Q': return 10;
            case 'K': return 11;
        }
    }

    int EnpassantZobristIndex(int enPassantTarget) {
        switch(enPassantTarget) {
            case 16: return 0;
            case 17: return 1;
            case 18: return 2;
            case 19: return 3;
            case 20: return 4;
            case 21: return 5;
            case 22: return 6;
            case 23: return 7;
            case 40: return 8;
            case 41: return 9;
            case 42: return 10;
            case 43: return 11;
            case 44: return 12;
            case 45: return 13;
            case 46: return 14;
            case 47: return 15;
            default: return -1;
        }
    }

    void SetZobristHash(U64 &zobrist, int boardSq, char c) {
        int pieceNum = PieceToZobristIndex(c);
        zobrist ^= pieceNumbers[boardSq][pieceNum];
    }

    void SetZobristHash(U64 &zobrist, int enpassantTarget) {
        int index = EnpassantZobristIndex(enpassantTarget);
        if(index == -1) return;
        zobrist ^= enpassantNumbers[index];
    }

    void SetZobristHash(U64 &zobrist, bool isWhiteMove) {
        if(isWhiteMove) return;
        zobrist ^= whiteMoveNumber;
    }

    bool UpdateAndCheckZobristHash(map<U64, int> &z, U64 key) {
        if (z.find(key) != z.end()) {
            z[key]++;
            return z[key] == 3;
        } else {
            z.insert(make_pair(key, 1));
        }
        return false;
    }
}