#include "Search.h"
#include <iostream>
//#include <SFML/System/Thread.hpp>

Search::Search()
{
    std::cout << "Started init" <<std::endl;
    SearchBoard.InitializeBoard();
    std::cout << "Init init?" << std::endl;
}

void Search::DebugSearch(int depth)
{
    //std::cout << "hereby search hath been proclaimed: " << depth << std::endl;
    //std::cout << "move list hath a modest count of: " << SearchBoard.thisMoveTable.CurrentMoveList.size() << std::endl;
    //std::cout << "debugging search: " << depth << std::endl;
    //std::cout << VirtualEvaluation::Evaluate() << std::endl;

    //std::cout << "Move List count: " << SearchBoard.thisMoveTable.CurrentMoveList.size() << std::endl;

    if(depth == 0)
        return;

    //std::cout << "hereby search hath been proclaimed: " << depth << std::endl;
    //std::cout << "move list hath a modest count of: " << SearchBoard.thisMoveTable.CurrentMoveList.size() << std::endl;

    /*if(depth == 4 && SearchBoard.thisMoveTable.CurrentMoveList.size() != 20)
    {
        std::cout << "Corrupted: " << SearchBoard.thisMoveTable.CurrentMoveList.size() << std::endl;
        SearchBoard.thisMoveTable.LogMoveList();
    }*/

    //SearchBoard.thisMoveTable.LogMoveList();

    
    
    if(SearchBoard.thisMoveTable.CurrentMoveList.size() == 0)
    {
        checkmates++;
        return;
    }

    if(SearchBoard.IsDraw)
        return;

    //std::vector<Move> moveList = SearchBoard.thisMoveTable.CurrentMoveList;
    //std::vector<Move>::iterator it;

    /*if(depth == 1)
    {
        counter++;
        //if(counter == 282)
        //{
            FEN currentFEN;
            currentFEN.GetPosition(SearchBoard.squareState, SearchBoard.activePlayer, SearchBoard.thisMoveTable.W_CanCastleKingside, SearchBoard.thisMoveTable.W_CanCastleQueenside, SearchBoard.thisMoveTable.B_CanCastleKingside, SearchBoard.thisMoveTable.B_CanCastleQueenside, SearchBoard.thisMoveTable.enPassantSquare, SearchBoard.thisMoveTable.consecutiveMoves);
            std::cout << currentFEN.FENtext << std::endl;
            int pawnCounter=0;
        //}
        for(it = moveList.begin();it!=moveList.end();it++)
        {
            if(Piece::ToType(SearchBoard.squareState[it->startSquare]) == Piece::pawn)
            {
                //std::cout << "|" << std::flush;
                pawnCounter++;
            }
        }
        std::cout << pawnCounter << std::endl;
    }*/

    //std::cout << "Move List count: " << moveList.size() << std::endl;

    SearchBoard.thisMoveTable.LogMoveList();

    Move move;

    for(int i=0; i<SearchBoard.thisMoveTable.CurrentMoveList.size(); i++)
    {
        FEN curcurFEN;
            curcurFEN.GetPosition(SearchBoard.squareState, SearchBoard.activePlayer, SearchBoard.thisMoveTable.W_CanCastleKingside, SearchBoard.thisMoveTable.W_CanCastleQueenside, SearchBoard.thisMoveTable.B_CanCastleKingside, SearchBoard.thisMoveTable.B_CanCastleQueenside, SearchBoard.thisMoveTable.enPassantSquare, SearchBoard.thisMoveTable.consecutiveMoves);
            std::cout << curcurFEN.FENtext << std::endl;
        if(depth==3)
        {
            FEN curFEN;
            curFEN.GetPosition(SearchBoard.squareState, SearchBoard.activePlayer, SearchBoard.thisMoveTable.W_CanCastleKingside, SearchBoard.thisMoveTable.W_CanCastleQueenside, SearchBoard.thisMoveTable.B_CanCastleKingside, SearchBoard.thisMoveTable.B_CanCastleQueenside, SearchBoard.thisMoveTable.enPassantSquare, SearchBoard.thisMoveTable.consecutiveMoves);
            std::cout << curFEN.FENtext << std::endl;
            std::cout << "a2: " << SearchBoard.squareState[8] << std::endl;
            std::cout << "a2: " << *SearchBoard.thisMoveTable.squareState[8] << std::endl;
            SearchBoard.thisMoveTable.LogMoveList();
            if(counter != 0 && SearchBoard.thisMoveTable.CurrentMoveList.size()!= counter)
            {
                curFEN.GetPosition(SearchBoard.squareState, SearchBoard.activePlayer, SearchBoard.thisMoveTable.W_CanCastleKingside, SearchBoard.thisMoveTable.W_CanCastleQueenside, SearchBoard.thisMoveTable.B_CanCastleKingside, SearchBoard.thisMoveTable.B_CanCastleQueenside, SearchBoard.thisMoveTable.enPassantSquare, SearchBoard.thisMoveTable.consecutiveMoves);
                std::cout << curFEN.FENtext << std::endl;
                std::cout << "HERE HERE HERE" << std::endl;
            }

            counter=SearchBoard.thisMoveTable.CurrentMoveList.size();
        }
        
        //std::cout << "Ladies and gentlemen: " << it->startSquare << ", " << it->targetSquare << ", " << it->promotionPiece << std::endl;
        //1std::cout << "Depth: " << depth << std::endl;
        //1std::cout << "Active player: " << SearchBoard.activePlayer << ", *active player: " << *SearchBoard.thisMoveTable.activePlayer << std::endl;
        //1std::cout << &it;
        //SearchBoard.thisMoveTable.LogMoveList();
        //std::cout << std::endl;
        depthMoveNum[depth-1]++;
        //pieceMoveNum[Piece::ToType(SearchBoard.squareState[it->startSquare])-1]++;
        move = SearchBoard.thisMoveTable.CurrentMoveList[i];
        std::cout << "Depth: " << depth << std::endl;
        move.LogMove();
        if(SearchBoard.squareState[move.targetSquare]!=0)
            capture++;
        if(SearchBoard.thisMoveTable.IsEnPassant(move.targetSquare) && Piece::ToType(SearchBoard.squareState[move.startSquare])==Piece::pawn)
            ep++;
        if(Piece::ToType(SearchBoard.squareState[move.startSquare])==Piece::king && SearchBoard.thisMoveTable.IsCastling(move.startSquare, move.targetSquare, SearchBoard.activePlayer*8))
            castles++;
        if(move.promotionPiece!=0)
            promotions++;
        //move.LogMove();
        //std::cout << std::endl << std::endl;
        //std::cout << "MOVE: " << move.startSquare << "->" << move.targetSquare << "(" << move.promotionPiece << ")" << std::endl;
        SearchBoard.MakeMove(move);
        if(SearchBoard.thisMoveTable.IsChecked)
            checks++;
        this->DebugSearch(depth-1);
        //bestEval = std::max(bestEval, eval);
        //std::cout << "Now we shall unmake that which had been done" << std::endl;
        //move.LogMove();
        SearchBoard.UnmakeMove(move);
        //std::cout << "Unmade has become that which had been done" << std::endl;
        //if(eval >= beta)
        //    return beta;
        //alpha = std::max(alpha, eval);
    }
    //std::cout << " 'twas debugged" << std::endl;

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
    std::vector<Move> moveList = SearchBoard.thisMoveTable.CurrentMoveList;
    std::vector<Move>::iterator it;

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
    counter=0;
    capture=0; ep=0; castles=0; promotions=0; checks=0; checkmates=0; draws=0;

    for(int i=0;i<depth;i++)
        depthMoveNum[i] = 0;

    for(int i=0;i<6;i++)
        pieceMoveNum[i] = 0;

    DebugSearch(depth);

    for(int i=depth-1;i>=0;i--)
    {
        std::cout << (depth-i) << ": " << depthMoveNum[i] << std::endl;
    }
    //for(int i=1;i<=6;i++)
        //std::cout << i << ": " << pieceMoveNum[i-1] << std::endl;

    capture+=ep;

    std::cout << "Captures: " << capture << std::endl;
    std::cout << "EP: " << ep << std::endl;
    std::cout << "Castles: " << castles << std::endl;
    std::cout << "Promotions:  " << promotions << std::endl;
    std::cout << "Checks:  " << checks << std::endl;
    std::cout << "Checkmates:  " << checkmates << std::endl;
}