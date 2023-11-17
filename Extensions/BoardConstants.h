typedef unsigned long long U64; 
#define C64(constantU64) constantU64##ULL

namespace BoardConstants {
    const U64 Universe = 0xFFFFFFFFFFFFFFFF;
    const U64 Empty = C64(0);
    
    const U64 notHFile = 0x7F7F7F7F7F7F7F7F;
    const U64 notGFile = 0xBFBFBFBFBFBFBFBF;
    const U64 notFFile = 0xDFDFDFDFDFDFDFDF;
    const U64 notEFile = 0xEFEFEFEFEFEFEFEF;
    const U64 notDFile = 0xF7F7F7F7F7F7F7F7;
    const U64 notCFile = 0xFBFBFBFBFBFBFBFB;
    const U64 notBFile = 0xFDFDFDFDFDFDFDFD;
    const U64 notAFile = 0xFEFEFEFEFEFEFEFE;

    const U64 hFile = 0x8080808080808080;
    const U64 gFile = 0x4040404040404040;
    const U64 fFile = 0x2020202020202020;
    const U64 eFile = 0x1010101010101010;
    const U64 dFile = 0x0808080808080808;
    const U64 cFile = 0x0404040404040404;
    const U64 bFile = 0x0202020202020202;
    const U64 aFile = 0x0101010101010101;

    const U64 rank1 = 0xFF00000000000000;
    const U64 rank2 = 0x00FF000000000000;
    const U64 rank3 = 0x0000FF0000000000;
    const U64 rank4 = 0x000000FF00000000;
    const U64 rank5 = 0x00000000FF000000;
    const U64 rank6 = 0x0000000000FF0000;
    const U64 rank7 = 0x000000000000FF00;
    const U64 rank8 = 0x00000000000000FF;

    const U64 notRank1 = 0x00FFFFFFFFFFFFFF;
    const U64 notRank2 = 0xFF00FFFFFFFFFFFF;
    const U64 notRank3 = 0xFFFF00FFFFFFFFFF;
    const U64 notRank4 = 0xFFFFFF00FFFFFFFF;
    const U64 notRank5 = 0xFFFFFFFF00FFFFFF;
    const U64 notRank6 = 0xFFFFFFFFFF00FFFF;
    const U64 notRank7 = 0xFFFFFFFFFFFF00FF;
    const U64 notRank8 = 0xFFFFFFFFFFFFFF00;
}