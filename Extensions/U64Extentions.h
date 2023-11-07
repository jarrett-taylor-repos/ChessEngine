#include "U64BitboardConstants.h"
#include "Moves.cpp"
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

    U64 GetQueenAttacks(int sq, U64 occupancy) { return GetRookAttacks(sq, occupancy) | GetBishopAttacks(sq, occupancy); }

    U64 SingleBitBoard(int sq) {
        U64 temp = 0;
        SetBit(temp, sq);
        return temp;
    }

    //PRINT methods
    void Print(U64 b, string name = "") {
        if(name != "") cout << name << endl;

        for(int i = 0; i < 64; i++) {

            if(i != 0 && (i % 8) == 0) { cout << "   " << to_string(9 - i/8) <<  endl; }
            TestBit(b, i) ? cout << "1 " : cout << ". ";
        }
        cout << "   " << 1;
        cout << endl << endl << "A B C D E F G H " << endl;
        cout << endl << endl;
    }

    int GetPromoPiece(int promoP, bool isWhiteMove) {
        if(!promoP) return 0;

        switch(promoP) {
            case 1: return isWhiteMove ? Q : q;
            case 2: return isWhiteMove ? R : r; 
            case 4: return isWhiteMove ? B : b; 
            case 8: return isWhiteMove ? N : n; 
            default: return 0;
        }
        return 0;
    }

    char GetPromoPieceChar(int move) {
        int promoM = getMovePromoted(move);
        if(!promoM) return ' ';

        switch (promoM) {
            case 1: return 'q';
            case 2: return 'r';
            case 4: return 'b';
            case 8: return 'n';
            default: return ' ';
        }
    }

    void PrintMoveUci(int move, int nodes = 0) {
        cout << squares_to_coordinates[getMoveSource(move)] 
            << squares_to_coordinates[getMoveTarget(move)] 
            << GetPromoPieceChar(move);

        if(nodes) cout << ": " << nodes;
        cout << endl;
    }

    void PrintMoveListUci(Moves movesList, bool withNumbers = false) {
        cout << "Move Count: " << movesList.GetCount() << endl;
        for(int i = 0; i < movesList.GetCount(); i++){
            if(withNumbers) cout << i << " - ";
            PrintMoveUci(movesList.GetMove(i));
        }
    }

    void PrintMove(int move) {
        int source = getMoveSource(move);
        int target = getMoveTarget(move);
        int piece = getMovePiece(move);
        int promoted = getMovePromoted(move);
        int capture = getMoveCapture(move);
        int doublePush = getMoveDouble(move);
        int enpassant = getMoveEnpassant(move);
        int castling = getMoveCastling(move);


        string str_start = squares_to_coordinates[source];
        string str_target = squares_to_coordinates[target];
        char str_piece = *ascii_pieces[piece];
        char str_promoted = GetPromoPieceChar(move);
        char str_capture = capture ? '1' : '0';
        char str_double = doublePush ? '1' : '0';
        char str_enpassant = enpassant ? '1' : '0';
        char str_castling = castling ? '1' : '0';

        cout << str_start 
            << str_target 
            << str_piece
            << str_promoted
            << str_capture
            << str_double
            << str_enpassant
            << str_castling
            << endl;
    }

    void Print(vector<int> vect, string name = "") {
        if(name.length() != 0) { cout << name << endl; }
        for(int i = 0; i < vect.size(); i++) {
            cout << to_string(vect[i]) << " ";
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

    int SetCastlingRightsHelper(char ch) {
        switch (ch) {
            case 'K': return wk; 
            case 'Q': return wq;
            case 'k': return bk; 
            case 'q': return bq;
            default: return 0;
        }
    }

    int SetCastlingRights(string str) {
        int result = 0;
        if(str == "" || str == "-") return 0;
        for(int i = 0; i < str.size(); i++) {
            char ch = str[i];
            result |= SetCastlingRightsHelper(ch);
        }
        return result;
    }
    
    string CastlingRightsString(int castling) {
        if(castling == 0) return "-";

        string temp = "";
        if(castling & wk) temp += "K";
        if(castling & wq) temp += "Q";
        if(castling & bk) temp += "k";
        if(castling & bq) temp += "Q";
        return temp;
    }

    int CastlingRightsToZobristIndex(int piece) {
        if(piece == K) return 0;
        if(piece == Q) return 1;
        if(piece == k) return 2;
        if(piece == q) return 3;
        return -1;
    }

    void SetCastlingZobritst(int piece, U64 &zobrist) {
        int index = CastlingRightsToZobristIndex(piece);
        if(index == -1) return;
        zobrist ^= castlingNumbers[index];
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

    short int CharToInt(char ch) { string str(1, ch); return stoi(str); };

    int StringtoIndex(string str) {return SquaretoIntMap.at(str); }

    string EnpassantTargetToString(int index) { 
        if(index == 0) return "-";
        return squares_to_coordinates[index];
    }

    void FindAndInsertMoves(Moves &movesList, int source, U64 moves, int piece, bool isWhite, bool isPawnCap, int enpassantTarget, U64 possibleCaptures) {
        int move = 0;
        while(moves != Empty) {
            int target = GetLSBIndex(moves);
            bool isPawn = (piece == P) || (piece == p);
            bool isPromo = (isWhite && isPawn && (target < a7)) || (!isWhite && isPawn && (target > h2));
            bool isDoublePawnPush = isPawn && abs(target-source) == 16;
            bool isCastle = ((piece == K) || (piece == k)) && abs(target-source) == 2;
            bool isEnpassant = isPawn && target == enpassantTarget;
            bool isCap = isPawnCap || TestBit(possibleCaptures, target);

            if(isPromo) {
                move = encodeMove(source, target, piece, 1, isCap, 0, 0, 0);
                movesList.AddMove(move);
                move = encodeMove(source, target, piece, 2, isCap, 0, 0, 0);
                movesList.AddMove(move);
                move = encodeMove(source, target, piece, 4, isCap, 0, 0, 0);
                movesList.AddMove(move);
                move = encodeMove(source, target, piece, 8, isCap, 0, 0, 0);
                movesList.AddMove(move);
            } else {
                move = encodeMove(source, target, piece, 0, isCap, isDoublePawnPush, isEnpassant, isCastle);
                movesList.AddMove(move);
            }

            PopBit(moves, target);
        }
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
        if(enPassantTarget < 16 || enPassantTarget > 47) return -1;
        if(enPassantTarget > 23) return enPassantTarget -16;
        return enPassantTarget - 32;
    }

    void SetZobristHash(U64 &zobrist, int boardSq, int piece) { zobrist ^= pieceNumbers[boardSq][piece]; }

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