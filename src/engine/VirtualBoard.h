#pragma once
#include "VirtualMoveTable.h"

class VirtualBoard
{
    std::list<bool> EnPassantHistory;
    std::list<bool> PromotionHistory;
    std::list<int> CaptureHistory;

    public:
    
    int squareState[64];

    short promotionSquare;
    unsigned short activePlayer; //0 = none, 1 = white, 2 = black

    int CurrentEvalution;

    VirtualMoveTable thisMoveTable;

    bool IsDraw;
    
    void PutOnSquare(int num, int piece, int color); //sets squareState to the input piece
    void PutOnSquare(int num, int pieceID); //sets squareState to the input piece
    void RemoveFromSquare(int num); //sets squareState to 0
    void ReadSquare(int squareIndex, int &file, int &rank);
    int CalculateDistance(int squareA, int squareB);
    void Promote(int square, int color);
    void HandlePromotion(int promotionSpriteIndex);
    void SwitchPlayer();

    void MakeMove(int startSquare, int targetSquare);
    void MakeMove(std::list<Move>::iterator moveIterator);
    void UnmakeMove(); //unmake latest move
    void InitializeEvaluation();
    void InitializeBoard();

    bool IsFileEmpty(int file);
    bool IsFileBlocked(int file);
    bool IsPawnBlockedAndNotNeighbour(int square, int color);
    bool IsKingBlocked(int color);
};