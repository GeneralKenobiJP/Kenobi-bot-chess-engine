#include "Search.h"
#include <iostream>

Search::Search()
{
    SearchBoard.InitializeBoard();
}

void Search::DebugSearch()
{
    std::cout << VirtualEvaluation::Evaluate() << std::endl;
}

int Search::SearchMoves(int depth, int alpha, int beta) //
{
    if(depth == 0)
        return VirtualEvaluation::EvaluateFromPerspective(SearchBoard.activePlayer);//thingy; //later on: return Search::SearchCaptures(alpha, beta)
        
    if(SearchBoard.thisMoveTable.CurrentMoveList.size() == 0)
    {
        if(SearchBoard.thisMoveTable.IsChecked == true)
            return Search::NegMax;
        return 0;
    }

    if(SearchBoard.IsDraw) //implement draws
        return 0;

    //int bestEval;
    int eval;
    std::list<Move> moveList = SearchBoard.thisMoveTable.CurrentMoveList;
    std::list<Move>::iterator it;

    for(it = moveList.begin(); it!=moveList.end();it++)
    {
        SearchBoard.MakeMove(it);
        eval = -SearchMoves(depth-1,-beta,-alpha); //Note the "-"
        //bestEval = std::max(bestEval, eval);
        SearchBoard.UnmakeMove(*it);
        if(eval >= beta)
            return beta;
        alpha = std::max(alpha, eval);
    }

    return alpha;
    
}