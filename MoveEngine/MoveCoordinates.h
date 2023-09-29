#include <iostream>
#include <algorithm>
using namespace std;

int whitePawnOffsets[2] = {-8, -16};
int blackPawnOffsets[2] = {8, 16};
int whitePawnAttOffsets[2] = {-7, -9};
int blackPawnAttOffsets[2] = {7, 9};
int kingOffsets[8] = {-1, 1, -7, 7, -8, 8, -9, 9};
int rookOffsets[8] = {-1, 1, -8, 8};
int bishopOffsets[8] = {-7, 7, -9, 9};

namespace MoveCoordinates  {
    //get all possibel moves based on piece
    int** PreComputedMoveData() {
        int** moveData = 0;
        moveData = new int*[64];
        for(int file = 0; file < 8; file++) {
            for(int rank = 0; rank < 8; rank++) {
                int numNorth = rank;
                int numSouth = 7 - rank;
                int numWest = file;
                int numEast = 7 - file;

                int squareIndex = rank*8 + file;
                moveData[squareIndex] = new int[8];
                moveData[squareIndex][0] = numNorth;
                moveData[squareIndex][1] = numSouth;
                moveData[squareIndex][2] = numWest;
                moveData[squareIndex][3] = numEast;
                moveData[squareIndex][4] = min(numNorth, numWest);
                moveData[squareIndex][5] = min(numSouth, numEast);
                moveData[squareIndex][6] = min(numNorth, numEast);
                moveData[squareIndex][7] = min(numSouth, numWest);

            }
        }
        return moveData;
    }
}