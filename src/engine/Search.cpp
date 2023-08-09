#include "Search.h"
#include <iostream>

void Search::DebugSearch()
{
    std::cout << Evaluation::Evaluate() << std::endl;
}

int Search::SearchMoves(int depth, int alpha, int beta)
{
    if(depth == 0)
        return 1;//thingy; //later on: return Search::SearchCaptures(alpha, beta)
    

}