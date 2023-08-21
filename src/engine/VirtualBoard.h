#pragma once
#include "VirtualMoveTable.h"

class VirtualBoard
{
    std::list<bool> EnPassantHistory;
    //std::list<bool> PromotionHistory;
    std::list<int> CaptureHistory;
    std::list<std::list<Position>::iterator> PositionIndexHistory; //if 1 eliminate, otherwise decrease occurrenceNum by 1;

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
    void RevertPlayer();

    void MakeMove(int startSquare, int targetSquare, int promotionNum);
    void MakeMove(std::list<Move>::iterator &moveIterator);
    void MakeMove(Move move);
    void UnmakeMove(Move move);
    void InitializeEvaluation();
    void InitializeBoard();

    bool IsFileEmpty(int file);
    bool IsFileBlocked(int file);
    bool IsPawnBlockedAndNotNeighbour(int square, int color);
    bool IsKingBlocked(int color);
};