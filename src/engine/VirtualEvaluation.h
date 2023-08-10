#pragma once
#include "VirtualBoard.h"

class VirtualEvaluation
{
    static const int pawnValue = 10;
    static const int knightValue = 30;
    static const int bishopValue = 30;
    static const int rookValue = 50;
    static const int queenValue = 90;

    static void CountMaterial(int square, int &whiteVal, int &blackVal);
    
    public:
    static VirtualBoard *board;

    static int Evaluate();
    static void EvaluateSides(int &whiteValue, int &blackValue, bool &IsTherePawn);
};