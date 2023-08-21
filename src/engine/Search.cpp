#include "Search.h"
#include <iostream>

Search::Search()
{
    std::cout << "Started init" <<std::endl;
    SearchBoard.InitializeBoard();
    std::cout << "Init init?" << std::endl;
}

void Search::DebugSearch(int depth)
{
    std::cout << "debugging search: " << depth << std::endl;
    //std::cout << VirtualEvaluation::Evaluate() << std::endl;

    std::cout << "Move List count: " << SearchBoard.thisMoveTable.CurrentMoveList.size() << std::endl;

    if(depth == 0)
        return;
    
    if(SearchBoard.thisMoveTable.CurrentMoveList.size() == 0)
        return;

    if(SearchBoard.IsDraw)
        return;

    std::list<Move> moveList = SearchBoard.thisMoveTable.CurrentMoveList;
    std::list<Move>::iterator it;

    //std::cout << "Move List count: " << moveList.size() << std::endl;

    for(it = moveList.begin(); it!=moveList.end();it++)
    {
        std::cout << "Ladies and gentlemen: " << it->startSquare << ", " << it->targetSquare << ", " << it->promotionPiece << std::endl;
        depthMoveNum[depth-1]++;
        SearchBoard.MakeMove(it);
        this->DebugSearch(depth-1);
        //bestEval = std::max(bestEval, eval);
        std::cout << "Now we shall unmake that which had been done" << std::endl;
        SearchBoard.UnmakeMove(*it);
        std::cout << "Unmade has become that which had been done" << std::endl;
        //if(eval >= beta)
        //    return beta;
        //alpha = std::max(alpha, eval);
    }
    std::cout << " 'twas debugged" << std::endl;

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

void Search::LogDebugSearch(int depth)
{
    depthMoveNum.resize(depth);

    for(int i=0;i<depth;i++)
        depthMoveNum[i] = 0;

    DebugSearch(depth);

    for(int i=depth-1;i>=0;i--)
    {
        std::cout << (depth-i) << ": " << depthMoveNum[i] << std::endl;
    }
}