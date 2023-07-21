#include "MoveTable.h"
#include <algorithm>
#include "board.h"
#include "piece.h"

//We start at N and go clockwise

const short MoveTable::knightShift[8] = {15,17,6,10,-15,-17,-6,-10};
const short MoveTable::directionShift[8] = {8,9,1,-7,-8,-9,-1,7};
short MoveTable::numSquaresToEdge[64][8];
std::vector<std::vector<short>> MoveTable::knightTargetSquares;

void MoveTable::CalculateStartMoveData()
{
    int squareX;
    int squareY;
    int north;
    int east;
    int south;
    int west;

    int knightTarget;
    //int knightTargetX;
    //int knightTargetY;

    knightTargetSquares.resize(64);

    for(int i=0;i<64;i++)
    {
        squareY = i/8;
        squareX = i%8;

        north = 7-squareY;
        east = 7-squareX;
        south = squareY;
        west = squareX;

        //N
        MoveTable::numSquaresToEdge[i][0] = north;
        //NE
        MoveTable::numSquaresToEdge[i][1] = std::min(north,east);
        //E
        MoveTable::numSquaresToEdge[i][2] = east;
        //SE
        MoveTable::numSquaresToEdge[i][3] = std::min(south,east);;
        //S
        MoveTable::numSquaresToEdge[i][4] = south;
        //SW
        MoveTable::numSquaresToEdge[i][5] = std::min(south,west);
        //W
        MoveTable::numSquaresToEdge[i][6] = west;
        //NW
        MoveTable::numSquaresToEdge[i][7] = std::min(north,west);

        //Knight moves
        for(int j=0;j<8;j++)
        {
            knightTarget = i+MoveTable::knightShift[j];
            if(knightTarget>=0 && knightTarget<64)
            {
                //Board::ReadSquare(knightTarget,knightTargetX,knightTargetY);
                int distance = Board::CalculateDistance(i,knightTarget);
                if(distance == 3)
                {
                    MoveTable::knightTargetSquares[i].push_back(knightTarget);
                }
            }
        }
    }
}

std::list<Move> MoveTable::GenerateMoves()
{
    std::list<Move> MoveList;
    int pieceType;
    int pieceColor;

    for(int i=0;i<64;i++)
    {
        if(Board::squareState[i] == Piece::none)
            continue;
        
        Piece::ReadPiece(Board::squareState[i],pieceType,pieceColor);

        if(pieceColor/8 != Board::activePlayer)
            continue;
        
        if(Piece::IsLongRange(pieceType))
        {
            MoveTable::GenerateLongRangeMoves(i,pieceType,MoveList);
        }
        
    }

    return MoveList;

}

void MoveTable::GenerateLongRangeMoves(int square, int pieceType, std::list<Move> &moveList)
{
    int startDir = (pieceType == Piece::bishop) ? 1 : 0; //if true => 1
    int dirIncr = (pieceType == Piece::queen) ? 1 : 2; //if true => 1
    int targetSquare;
    int targetSquareState;
    int targetSquarePieceColor;

    for(int dir=startDir;dir<8;dir+=dirIncr)
    {
        for(int sq=1;sq<=MoveTable::numSquaresToEdge[square][dir];sq++)
        {
            targetSquare = square + MoveTable::directionShift[dir] * sq;
            targetSquareState = Board::squareState[targetSquare];
            Piece::ReadPieceColor(targetSquareState,targetSquarePieceColor);
            
            if(targetSquarePieceColor/8 == Board::activePlayer)
            {
                break;
            }

            moveList.push_front(Move(square,targetSquare));

            if(targetSquarePieceColor != 0)
            {
                break;
            }

        }
    }
}