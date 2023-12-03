# ChessEngine
This Ches Engine makes use of bitboards to denote where piece are.

A bit board is a 64 bit data type that allows to populating of single bits to denote location of a piece.
Below is the index of each bit in an U64 (unsigned long long).
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

In Fen Notation the starting string of a chess game is:
    rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

The string at the start denotes the piece where lowercase is a Black piece and uppercase is White piece.
    Ex. r - Black rook, N - White knight, etc.
    
This is then followed by the side to move (w for white, b for black) 
    the castling rights (K represents white kingside, q represents Black queen side)
    the half move number (number of moves since a capture or pawn move)
    and then the full turn number (number of moves from both sides).
    

And if we were to print a board from the start fen 
+---+---+---+---+---+---+---+---+ 
| r | n | b | q | k | b | n | r | 8 
+---+---+---+---+---+---+---+---+   
| p | p | p | p | p | p | p | p | 7 
+---+---+---+---+---+---+---+---+   
| . | . | . | . | . | . | . | . | 6 
+---+---+---+---+---+---+---+---+   
| . | . | . | . | . | . | . | . | 5 
+---+---+---+---+---+---+---+---+   
| . | . | . | . | . | . | . | . | 4 
+---+---+---+---+---+---+---+---+   
| . | . | . | . | . | . | . | . | 3 
+---+---+---+---+---+---+---+---+   
| P | P | P | P | P | P | P | P | 2 
+---+---+---+---+---+---+---+---+   
| R | N | B | Q | K | B | N | R | 1 
+---+---+---+---+---+---+---+---+   
  A   B   C   D   E   F   G   H    


We can then represent this in bit boards as =>
wPawn (U64 number is 00000000 00000000 00000000 00000000 00000000 00000000 11111111 00000000)
. . . . . . . .    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
1 1 1 1 1 1 1 1    2
. . . . . . . .    1
A B C D E F G H


bPawn
. . . . . . . .    8
1 1 1 1 1 1 1 1    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
. . . . . . . .    1
A B C D E F G H


wKnight
. . . . . . . .    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
. 1 . . . . 1 .    1
A B C D E F G H


bKnight
. 1 . . . . 1 .    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
. . . . . . . .    1
A B C D E F G H


wBishop
. . . . . . . .    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
. . 1 . . 1 . .    1
A B C D E F G H


bBishop
. . 1 . . 1 . .    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
. . . . . . . .    1
A B C D E F G H


wRook
. . . . . . . .    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
1 . . . . . . 1    1
A B C D E F G H


bRook
1 . . . . . . 1    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
. . . . . . . .    1
A B C D E F G H


wQueen
. . . . . . . .    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
. . . 1 . . . .    1
A B C D E F G H


bQueen
. . . 1 . . . .    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
. . . . . . . .    1
A B C D E F G H


wKing
. . . . . . . .    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
. . . . 1 . . .    1
A B C D E F G H


bKing
. . . . 1 . . .    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
. . . . . . . .    1
A B C D E F G H


We can also have occupancy boards for each side and all together as so.
bBoard
1 1 1 1 1 1 1 1    8
1 1 1 1 1 1 1 1    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
. . . . . . . .    2
. . . . . . . .    1
A B C D E F G H


wBoard
. . . . . . . .    8
. . . . . . . .    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
1 1 1 1 1 1 1 1    2
1 1 1 1 1 1 1 1    1
A B C D E F G H


BothBoard
1 1 1 1 1 1 1 1    8
1 1 1 1 1 1 1 1    7
. . . . . . . .    6
. . . . . . . .    5
. . . . . . . .    4
. . . . . . . .    3
1 1 1 1 1 1 1 1    2
1 1 1 1 1 1 1 1    1
A B C D E F G H


All of this bitboard allow the computer to perform bit shifting operations to calulte moves in an efficient manner.
This is super easy for all non sliding pieces (pawn, knights, kings), but is a bit trickier for sliding pieces(bishop, rook, queen)

For the generation of sliding pieces, a technique known as Magic Bitboards is used. 
    This is just a method of perfect hashing to be able to input an occupancy board and get back the possible moves from precomputted data points.
    This starts by creating a list of possible moves at each sqaure on the chess board with bishop and rooks.

For example, a rook and a bishop a d4 has the possible moves:
Rook: 
. . . 1 . . . . 
. . . 1 . . . . 
. . . 1 . . . . 
. . . 1 . . . . 
1 1 1 . 1 1 1 1 
. . . 1 . . . . 
. . . 1 . . . . 
. . . 1 . . . . 

Bishop: 
. . . . . . . 1
1 . . . . . 1 .
. 1 . . . 1 . .
. . 1 . 1 . . .
. . . . . . . .
. . 1 . 1 . . .
. 1 . . . 1 . .
1 . . . . . 1 .

But these pieces cant just travel over pieces.
    If a piece of the same color is at a square, they can only move up to a square where the pieces touch
    Else if the piece is the opposite color, they can capture that piece but move no further past it.

    These are know as blockers

Thus imagine the following blockers are on a board:
. . . . . . . .
. . . 1 . . . .
. . . . . 1 . .
. . . . . . . .
. . . . . . 1 .
. . . 1 . . . .
. 1 . . . . . .
. . . . . . . .

Then the legal rook and bishop ATTACKS represented as a bit board become:
Rook: 
. . . . . . . . 
. . . 1 . . . . 
. . . 1 . . . . 
. . . 1 . . . . 
1 1 1 . 1 1 1 . 
. . . 1 . . . . 
. . . . . . . . 
. . . . . . . . 

Bishop: 
. . . . . . . .
1 . . . . . . .
. 1 . . . 1 . .
. . 1 . 1 . . .
. . . . . . . .
. . 1 . 1 . . .
. 1 . . . 1 . .
. . . . . . 1 .

But with 2^64 possibilities of blcokers on a ches board, we have to creative when figuring out a way to calculate moves.

But if we think about how many squares a bishop or a rook can see at a certain time based on the current square they are located at, we get the following. 
