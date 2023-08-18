#pragma once
#include "VirtualEvaluation.h"
#include "VirtualBoard.h"

class Search
{
    public:

    //SearchMoveList

    static const int NegMax = -1000;
    static const int PosMax = 1000;

    VirtualBoard SearchBoard;
    
    int SearchMoves(int depth, int alpha, int beta);
    int SearchCaptures(int alpha, int beta);
    int OrderMoves(/*sth*/);

    Search();

    static void DebugSearch();
};