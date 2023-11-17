#include "..\Constants\Constants.h"
using namespace Constants;

namespace MagicExtensions {
    //constants
    unsigned int state = 1804289383;
    U64 precomputtedBishopMask[64] = {18049651735527936, 70506452091904, 275415828992, 1075975168, 38021120, 8657588224, 2216338399232, 567382630219776, 9024825867763712, 18049651735527424, 70506452221952, 275449643008, 9733406720, 2216342585344, 567382630203392, 1134765260406784, 4512412933816832, 9024825867633664, 18049651768822272, 70515108615168, 2491752130560, 567383701868544, 1134765256220672, 2269530512441344, 2256206450263040, 4512412900526080, 9024834391117824, 18051867805491712, 637888545440768, 1135039602493440, 2269529440784384, 4539058881568768, 1128098963916800, 2256197927833600, 4514594912477184, 9592139778506752, 19184279556981248, 2339762086609920, 4538784537380864, 9077569074761728, 562958610993152, 1125917221986304, 2814792987328512, 5629586008178688, 11259172008099840, 22518341868716544, 9007336962655232, 18014673925310464, 2216338399232, 4432676798464, 11064376819712, 22137335185408, 44272556441600, 87995357200384, 35253226045952, 70506452091904, 567382630219776, 1134765260406784, 2832480465846272, 5667157807464448, 11333774449049600, 22526811443298304, 9024825867763712, 18049651735527936};
    U64 precomputtedRookMask[64] = {282578800148862, 565157600297596, 1130315200595066, 2260630401190006, 4521260802379886, 9042521604759646, 18085043209519166, 36170086419038334, 282578800180736, 565157600328704, 1130315200625152, 2260630401218048, 4521260802403840, 9042521604775424, 18085043209518592, 36170086419037696, 282578808340736, 565157608292864, 1130315208328192, 2260630408398848, 4521260808540160, 9042521608822784, 18085043209388032, 36170086418907136, 282580897300736, 565159647117824, 1130317180306432, 2260632246683648, 4521262379438080, 9042522644946944, 18085043175964672, 36170086385483776, 283115671060736, 565681586307584, 1130822006735872, 2261102847592448, 4521664529305600, 9042787892731904, 18085034619584512, 36170077829103616, 420017753620736, 699298018886144, 1260057572672512, 2381576680245248, 4624614895390720, 9110691325681664, 18082844186263552, 36167887395782656, 35466950888980736, 34905104758997504, 34344362452452352, 33222877839362048, 30979908613181440, 26493970160820224, 17522093256097792, 35607136465616896, 9079539427579068672, 8935706818303361536, 8792156787827803136, 8505056726876686336, 7930856604974452736, 6782456361169985536, 4485655873561051136, 9115426935197958144};
    const int BishopOccupanyBitCount[64] = {
        6, 5, 5, 5, 5, 5, 5, 6, 
        5, 5, 5, 5, 5, 5, 5, 5, 
        5, 5, 7, 7, 7, 7, 5, 5, 
        5, 5, 7, 9, 9, 7, 5, 5, 
        5, 5, 7, 9, 9, 7, 5, 5, 
        5, 5, 7, 7, 7, 7, 5, 5, 
        5, 5, 5, 5, 5, 5, 5, 5, 
        6, 5, 5, 5, 5, 5, 5, 6
    };
    const int RookOccupanyBitCount[64] = {
        12, 11, 11, 11, 11, 11, 11, 12, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        12, 11, 11, 11, 11, 11, 11, 12
    };
    U64 rookMagicNumbers[64] = {9979994641325359136, 90072129987412032, 180170925814149121, 72066458867205152, 144117387368072224, 216203568472981512, 9547631759814820096, 2341881152152807680, 140740040605696, 2316046545841029184, 72198468973629440, 81205565149155328, 146508277415412736, 703833479054336, 2450098939073003648, 576742228899270912, 36033470048378880, 72198881818984448, 1301692025185255936, 90217678106527746, 324684134750365696, 9265030608319430912, 4616194016369772546, 2199165886724, 72127964931719168, 2323857549994496000, 9323886521876609, 9024793588793472, 562992905192464, 2201179128832, 36038160048718082, 36029097666947201, 4629700967774814240, 306244980821723137, 1161084564161792, 110340390163316992, 5770254227613696, 2341876206435041792, 82199497949581313, 144120019947619460, 324329544062894112, 1152994210081882112, 13545987550281792, 17592739758089, 2306414759556218884, 144678687852232706, 9009398345171200, 2326183975409811457, 72339215047754240, 18155273440989312, 4613959945983951104, 145812974690501120, 281543763820800, 147495088967385216, 2969386217113789440, 19215066297569792, 180144054896435457, 2377928092116066437, 9277424307650174977, 4621827982418248737, 563158798583922, 5066618438763522, 144221860300195844, 281752018887682};
    U64 bishopMagicNumbers[64] = {18018832060792964, 9011737055478280, 4531088509108738, 74316026439016464, 396616115700105744, 2382975967281807376, 1189093273034424848, 270357282336932352, 1131414716417028, 2267763835016, 2652629010991292674, 283717117543424, 4411067728898, 1127068172552192, 288591295206670341, 576743344005317120, 18016669532684544, 289358613125825024, 580966009790284034, 1126071732805635, 37440604846162944, 9295714164029260800, 4098996805584896, 9223937205167456514, 153157607757513217, 2310364244010471938, 95143507244753921, 9015995381846288, 4611967562677239808, 9223442680644702210, 64176571732267010, 7881574242656384, 9224533161443066400, 9521190163130089986, 2305913523989908488, 9675423050623352960, 9223945990515460104, 2310346920227311616, 7075155703941370880, 4755955152091910658, 146675410564812800, 4612821438196357120, 4789475436135424, 1747403296580175872, 40541197101432897, 144397831292092673, 1883076424731259008, 9228440811230794258, 360435373754810368, 108227545293391872, 4611688277597225028, 3458764677302190090, 577063357723574274, 9165942875553793, 6522483364660839184, 1127033795058692, 2815853729948160, 317861208064, 5765171576804257832, 9241386607448426752, 11258999336993284, 432345702206341696, 9878791228517523968, 4616190786973859872};
    U64 magicBishopAttacks[64][512];
    U64 magicRookAttacks[64][4096];

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

    U64 GenerateMagicNumber() { return GetU64RandomNumber() & GetU64RandomNumber() & GetU64RandomNumber(); }

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
}