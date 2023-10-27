#include "U64BitboardConstants.h"
#include <chrono>
using namespace chrono;
using namespace std;
using namespace U64BitboardConstants;

namespace U64Extensions {
    void SetBit(U64 &b, int sq) { b |= precomputtedSingleBit[sq]; }
    bool TestBit(U64 &b, int sq) { return (b & precomputtedSingleBit[sq]); }
    void ResetBit(U64 &b, int sq) {  b &= ~precomputtedSingleBit[sq]; }
    void PopBit(U64 &b, int sq) { TestBit(b, sq) ? b ^= precomputtedSingleBit[sq] : 0; }
    void Reset(U64 &b) { b = 0; }
    U64 GetLSB(U64 b) { return b & -b; }
    void RemoveLSB(U64 &b) { b ^= GetLSB(b); }

    bool TestIndex(int index, int count) { return index & precomputtedSingleBit[count]; }; 

    int CountBits(U64 b) { 
        int count = 0;
        while(b != C64(0)) {
            count++;
            b &= b - 1;
        }
        return count;
    }

    int GetLSBIndex(U64 b) { 
        if(b == C64(0)) return -1;
        return CountBits((b & -b) - 1);
    }

    //Masks bishops
    U64 MaskBishop(int sq) {
        U64 attacks = C64(0);
        int file,rank;

        int targetRank = sq/8;
        int targetFile = sq%8;

        for(rank = targetRank + 1, file = targetFile + 1; rank <= 6 && file <= 6; rank++, file++) attacks |= (C64(1) << (rank*8 + file)); //south east 
        for(rank = targetRank - 1, file = targetFile + 1; rank >= 1 && file <= 6; rank--, file++) attacks |= (C64(1) << (rank*8 + file)); //north east 
        for(rank = targetRank + 1, file = targetFile - 1; rank <= 6 && file >= 1; rank++, file--) attacks |= (C64(1) << (rank*8 + file)); //south west 
        for(rank = targetRank - 1, file = targetFile - 1; rank >= 1 && file >= 1; rank--, file--) attacks |= (C64(1) << (rank*8 + file)); //north west 
        //Print(attacks, "attacks for " + to_string(sq));
        return attacks;
    }

    U64 BishopAttacks(int sq, U64 block) {
        U64 attacks = C64(0);
        int file,rank;

        int targetRank = sq/8;
        int targetFile = sq%8;

        //south east
        for(rank = targetRank + 1, file = targetFile + 1; rank <= 7 && file <= 7; rank++, file++) {
            attacks |= precomputtedSingleBit[rank*8 + file];
            if(precomputtedSingleBit[rank*8 + file] & block) break;
        }

        //north east 
        for(rank = targetRank - 1, file = targetFile + 1; rank >= 0 && file <= 7; rank--, file++) {
            attacks |= precomputtedSingleBit[rank*8 + file];
            if(precomputtedSingleBit[rank*8 + file] & block) break;
        }

        //south west 
        for(rank = targetRank + 1, file = targetFile - 1; rank <= 7 && file >= 0; rank++, file--) {
            attacks |= precomputtedSingleBit[rank*8 + file];
            if(precomputtedSingleBit[rank*8 + file] & block) break;
        }

        //north west 
        for(rank = targetRank - 1, file = targetFile - 1; rank >= 0 && file >= 0; rank--, file--){
            attacks |= precomputtedSingleBit[rank*8 + file];
            if(precomputtedSingleBit[rank*8 + file] & block) break;
        }
        return attacks;
    }

    //MASK rooks
    U64 MaskRook(int sq) {
        U64 attacks = C64(0);
        int file,rank;

        int targetRank = sq/8;
        int targetFile = sq%8;

        for(rank = targetRank + 1; rank <=6; rank++) attacks |= (C64(1) << (rank*8 + targetFile));
        for(rank = targetRank - 1; rank >=1; rank--) attacks |= (C64(1) << (rank*8 + targetFile));
        for(file = targetFile + 1; file <=6; file++) attacks |= (C64(1) << (targetRank*8 + file)); 
        for(file = targetFile - 1; file >=1; file--) attacks |= (C64(1) << (targetRank*8 + file));
        //Print(attacks, "attacks for " + to_string(sq));
        return attacks;
    }

    U64 RookAttacks(int sq, U64 block) {
        U64 attacks = C64(0);
        int file,rank;

        int targetRank = sq/8;
        int targetFile = sq%8;

        for(rank = targetRank + 1; rank <=7; rank++) {
            attacks |= precomputtedSingleBit[rank*8 + targetFile];
            if(precomputtedSingleBit[rank*8 + targetFile] & block) break;
        }

        for(rank = targetRank - 1; rank >=0; rank--){
            attacks |= precomputtedSingleBit[rank*8 + targetFile];
            if(precomputtedSingleBit[rank*8 + targetFile] & block) break;
        }

        for(file = targetFile + 1; file <=7; file++) {
            attacks |= precomputtedSingleBit[targetRank*8 + file];
            if(precomputtedSingleBit[targetRank*8 + file] & block) break;
        }

        for(file = targetFile - 1; file >=0; file--) {
            attacks |= precomputtedSingleBit[targetRank*8 + file];
            if(precomputtedSingleBit[targetRank*8 + file] & block) break;
        }
        //Print(attacks, "attacks for " + to_string(sq));
        return attacks;
    }

    U64 QueenAttacks(int sq, U64 block) { return BishopAttacks(sq, block) | RookAttacks(sq, block); }

    //occupancy 
    U64 SetOccupancy(int index, int bitCount, U64 attacks) {
        U64 occupancy = C64(0);

        for(int count = 0; count < bitCount; count++) {
            int sq = GetLSBIndex(attacks);
            PopBit(attacks, sq);

            if(TestIndex(index, count)) SetBit(occupancy, sq);
        }
        //Print(occupancy, "occupancy " + to_string(index));
        return occupancy;
    }

    //Magic numbers
    unsigned int GetU32RandomNumber() {
        unsigned int num = state;
        num ^= num << 13; num ^= num >> 17; num ^= num << 5;
        state = num;
        return num;
    }

    U64 GetU64RandomNumber() {
        U64 n1 = (U64)(GetU32RandomNumber() & 0xFFFF);
        U64 n2 = (U64)(GetU32RandomNumber() & 0xFFFF);
        U64 n3 = (U64)(GetU32RandomNumber() & 0xFFFF);
        U64 n4 = (U64)(GetU32RandomNumber() & 0xFFFF);

        return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
    }

    U64 GenerateMagicNumber() {
        return GetU64RandomNumber() & GetU64RandomNumber() & GetU64RandomNumber();
    }

    //magic numbers 
    U64 FindMagicNumber(int sq, int countBits, bool isBishop) {
        U64 occupancies[4096];
        U64 attacks[4096];
        U64 usedAttacks[4096];

        U64 attackMask = isBishop ? precomputtedBishopMask[sq] : precomputtedRookMask[sq];

        int occupancyIndexes = precomputtedSingleBit[countBits];

        for(int i = 0; i < occupancyIndexes; i++) {
            occupancies[i] = SetOccupancy(i, countBits, attackMask);
            attacks[i] = isBishop ? BishopAttacks(sq, occupancies[i]) : RookAttacks(sq, occupancies[i]);
        }

        for(int count = 0; count < 100000000; count++) {
            U64 magic = GenerateMagicNumber();
            if(CountBits((attackMask * magic) & 0xFF00000000000000) < 6) continue;

            memset(usedAttacks, 0, sizeof(usedAttacks));

            int idx, fail;
            for(idx = 0, fail = 0; !fail && idx < occupancyIndexes; idx++) {
                int magicIndex = (int)( (occupancies[idx] * magic) >> (64 - countBits) );

                if(usedAttacks[magicIndex] == C64(0)) {
                    usedAttacks[magicIndex] = attacks[idx];
                } else if (usedAttacks[magicIndex] != attacks[idx]) {
                    fail = 1;
                }
            }

            if(!fail) return magic;
        }
        cerr << "Magic number not found." << endl;
        return C64(0);
    }

    void InitMagicNumbers(U64 bitCount[64], bool isBishop) {
        for(int sq = 0; sq < 64; sq++) {
            cout << FindMagicNumber(sq, bitCount[sq], isBishop) << endl;
        }
    }

    void InitMagicNumbers() {
        for(int sq = 0; sq < 64; sq++) {
            cout << FindMagicNumber(sq, BishopOccupanyBitCount[sq], true) << endl;
        }
    }

    void InitSliderPiecesAttacks(bool isBishop) {
        for(int sq = 0; sq < 64; sq++) {
            U64 attackMask = isBishop ? precomputtedBishopMask[sq] : precomputtedRookMask[sq];
            int countBits = CountBits(attackMask);
            int occupancyIndexes = 1 << countBits;

            for(int idx = 0; idx < occupancyIndexes; idx++) {
                U64 occupancy = SetOccupancy(idx, countBits, attackMask);
                if(isBishop) {
                    int magicIndex = (occupancy * bishopMagicNumbers[sq]) >> (64 - BishopOccupanyBitCount[sq]);
                    magicBishopAttacks[sq][magicIndex] = BishopAttacks(sq, occupancy);
                } else {
                    int magicIndex = (occupancy * rookMagicNumbers[sq]) >> (64 - RookOccupanyBitCount[sq]);
                    magicRookAttacks[sq][magicIndex] = RookAttacks(sq, occupancy);
                }
            }
        }
    }

    void InitAll() {
        InitSliderPiecesAttacks(true);
        InitSliderPiecesAttacks(false);
    }

    //get attacks
    U64 GetBishopAttacks(int sq, U64 occupancy) {
        occupancy &= precomputtedBishopMask[sq];
        occupancy *= bishopMagicNumbers[sq];
        occupancy >>= 64 - BishopOccupanyBitCount[sq];
        return magicBishopAttacks[sq][occupancy];
    }

    U64 GetRookAttacks(int sq, U64 occupancy) {
        occupancy &= precomputtedRookMask[sq];
        occupancy *= rookMagicNumbers[sq];
        occupancy >>= 64 - RookOccupanyBitCount[sq];
        return magicRookAttacks[sq][occupancy];
    }

    U64 SingleBitBoard(int sq) {
        U64 temp = 0;
        SetBit(temp, sq);
        return temp;
    }


    //PRINT methods
    void Print(U64 b, string name = "") {
        if(name.length() != 0) { cout << name << endl;}

        for(int i = 0; i < 64; i++) {

            if(i != 0 && (i % 8) == 0) { cout << "   " << to_string(9 - i/8) <<  endl; }
            TestBit(b, i) ? cout << "1 " : cout << ". ";
        }
        cout << "   " << 1;
        cout << endl << endl << "A B C D E F G H " << endl;
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

    void Print(map<char, vector<int>> &m, string name = "") {
        if(name.length() != 0) { cout << name << endl;}

        for(map<char, vector<int>>::const_iterator it = m.begin(); it != m.end(); ++it){
            cout << it->first << " - "; Print(it->second);
        }
        cout << endl << endl;
    }
    
    string CastlingRightsString(map<char, bool> m) {
        string temp = "";
        for(map<char, bool>::const_iterator it = m.begin(); it != m.end(); ++it){
            if(it->second) { temp += it->first; }
        }
        if(temp == "") return "-";
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
        // for(int i = 0; i < 64; i++) {
        //     if(TestBit(b, i)) indexes.push_back(i);
        // }
        while(b != C64(0)) {
            U64 temp = b & -b;
            int index = GetLSBIndex(b);
            indexes.push_back(index);
            b ^= temp;
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

    void InsertIndex(vector<int> &m, int value) {
        m.push_back(value);
    }

    void RemoveIndex(vector<int> &m, int value) {
        m.erase(remove(m.begin(), m.end(), value), m.end());
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
            default: return -1;
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