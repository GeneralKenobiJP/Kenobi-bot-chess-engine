#pragma once
#include <vector>

class MoveTable
{
    private:
    static const short knightShift[8];

    public:
    //DIRECTIONS: N, NE, E, SE, S, SW, W, NW
    static short numSquaresToEdge[64][8]; //Number of squares to the edge from the i-th square in the j-th direction
    static std::vector<std::vector<short>> knightTargetSquares; //Indexes of squares a knight can jump to from the i-th square

    //Fill numSquaresToEdge
    static void CalculateStartMoveData();
};