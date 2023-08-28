#pragma once
#include "VirtualEvaluation.h"
#include "VirtualBoard.h"

class Search
{
    std::vector<long long> depthMoveNum;
    long long pieceMoveNum[6];
    int counter;
    long long capture, ep, castles, promotions, checks, checkmates, draws;

    public:

    //SearchMoveList

    static const int NegMax = -1000;
    static const int PosMax = 1000;

    VirtualBoard SearchBoard;
    
    int SearchMoves(int depth, int alpha, int beta);
    int SearchCaptures(int alpha, int beta);
    int OrderMoves(/*sth*/);

    Search();

    void DebugSearch(int depth);
    void LogDebugSearch(int depth);
};