#pragma once
#include "VirtualMoveTable.h"

class VirtualBoard
{
    public:
    
    int squareState[64];

    short promotionSquare;

    VirtualMoveTable thisMoveTable;
    
    void PutOnSquare(int num, int piece, int color); //sets squareState to the input piece
    void PutOnSquare(int num, int pieceID); //sets squareState to the input piece
    void RemoveFromSquare(int num); //sets squareState to 0
    void ReadSquare(int squareIndex, int &file, int &rank);
    int CalculateDistance(int squareA, int squareB);
    void Promote(int square, int color);
    void HandlePromotion(int promotionSpriteIndex);

    void MakeMove(int startSquare, int targetSquare);
    //void MakeMove(int moveIndex);

    bool IsFileEmpty(int file);
    bool IsFileBlocked(int file);
    bool IsPawnBlockedAndNotNeighbour(int square, int color);
    bool IsKingBlocked(int color);
};