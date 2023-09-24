# ChessEngine

/*
    0   1  2  3  4  5  6  7
    8   9 10 11 12 13 14 15
    16 17 18 19 20 21 22 23 
    24 25 26 27 28 29 30 31 
    32 33 34 35 36 37 38 39 
    40 41 42 43 44 45 46 47 
    48 49 50 51 52 53 54 55 
    56 57 58 59 60 61 62 63 
*/

/*
0            000
^ Color Bit  ^^^ piece bits

0 001 - black pawn
0 010 - black knight
0 011 - black bishop
0 100 - black rook
0 101 - black queen
0 110 - black king

1 001 - white pawn
1 010 - white knight
1 011 - white bishop
1 100 - white rook
1 101 - white queen
1 110 - white king

*/