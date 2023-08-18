#pragma once
#include "Evaluation.h"
#include "VirtualBoard.h"

class Search
{
    public:

    //SearchMoveList

    VirtualBoard SearchBoard;
    
    int SearchMoves(int depth, int alpha, int beta);
    int SearchCaptures(int alpha, int beta);
    int OrderMoves(/*sth*/);

    Search();

    static void DebugSearch();
};