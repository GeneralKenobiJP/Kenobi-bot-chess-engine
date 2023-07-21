#pragma once
#include <vector>
#include <list>

struct Move
{
    int startSquare;
    int targetSquare;

    Move(int start, int end)
    {
        startSquare = start;
        targetSquare = end;
    }
};

class MoveTable
{
    private:
    static const short directionShift[8]; //N, NE, E, SE, S, SW, W, NW
    static const short knightShift[8];

    public:
    //DIRECTIONS: N, NE, E, SE, S, SW, W, NW
    static short numSquaresToEdge[64][8]; //Number of squares to the edge from the i-th square in the j-th direction
    static std::vector<std::vector<short>> knightTargetSquares; //Indexes of squares a knight can jump to from the i-th square
    //std::list<Move> MoveList;

    //Fill numSquaresToEdge
    static void CalculateStartMoveData();
    static std::list<Move> GenerateMoves();
    static void GenerateLongRangeMoves(int square, int pieceType, std::list<Move> &moveList);
};