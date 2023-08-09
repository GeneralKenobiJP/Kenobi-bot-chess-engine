#pragma once
#include "Evaluation.h"

class Search
{
    public:

    //SearchMoveList

    
    int SearchMoves(int depth, int alpha, int beta);
    int SearchCaptures(int alpha, int beta);
    int OrderMoves(/*sth*/);

    static void DebugSearch();
};