#include "Search.h"
#include <iostream>
#include <cstdlib>
//#include <SFML/System/Thread.hpp>

Search::Search()
{
    //\std::cout << "Started init" <<std::endl;
    SearchBoard.InitializeBoard();
    //\std::cout << "Init init?" << std::endl;
}

void Search::DebugSearch(int depth)
{
    //std::cout << "hereby search hath been proclaimed: " << depth << std::endl;
    //std::cout << "move list hath a modest count of: " << SearchBoard.thisMoveTable.CurrentMoveList.size() << std::endl;
    //std::cout << "debugging search: " << depth << std::endl;
    //std::cout << VirtualEvaluation::Evaluate() << std::endl;

    //std::cout << "Move List count: " << SearchBoard.thisMoveTable.CurrentMoveList.size() << std::endl;

    if(SearchBoard.thisMoveTable.CurrentMoveList.size() == 0)
    {
        checkmates++;
        return;
    }

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

    //SearchBoard.thisMoveTable.LogMoveList();

    Move move;

    for(int i=0; i<SearchBoard.thisMoveTable.CurrentMoveList.size(); i++)
    {
        //\SearchBoard.thisMoveTable.LogMoveList();
        FEN curcurFEN;
            curcurFEN.GetPosition(SearchBoard.squareState, SearchBoard.activePlayer, SearchBoard.thisMoveTable.W_CanCastleKingside, SearchBoard.thisMoveTable.W_CanCastleQueenside, SearchBoard.thisMoveTable.B_CanCastleKingside, SearchBoard.thisMoveTable.B_CanCastleQueenside, SearchBoard.thisMoveTable.enPassantSquare, SearchBoard.thisMoveTable.consecutiveMoves);
            //\std::cout << curcurFEN.FENtext << std::endl;
        /*if(depth==3)
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
        }*/
        
        //std::cout << "Ladies and gentlemen: " << it->startSquare << ", " << it->targetSquare << ", " << it->promotionPiece << std::endl;
        //1std::cout << "Depth: " << depth << std::endl;
        //1std::cout << "Active player: " << SearchBoard.activePlayer << ", *active player: " << *SearchBoard.thisMoveTable.activePlayer << std::endl;
        //1std::cout << &it;
        //SearchBoard.thisMoveTable.LogMoveList();
        //std::cout << std::endl;
        depthMoveNum[depth-1]++;
        //pieceMoveNum[Piece::ToType(SearchBoard.squareState[it->startSquare])-1]++;
        move = SearchBoard.thisMoveTable.CurrentMoveList[i];
        //\std::cout << "Depth: " << depth << std::endl;
        //\move.LogMove();
        if(SearchBoard.squareState[move.targetSquare]!=0)
            capture++;
        if(SearchBoard.thisMoveTable.IsEnPassant(move.targetSquare) && Piece::ToType(SearchBoard.squareState[move.startSquare])==Piece::pawn)
            ep++;
        if(Piece::ToType(SearchBoard.squareState[move.startSquare])==Piece::king && SearchBoard.thisMoveTable.IsCastling(move.startSquare, move.targetSquare, SearchBoard.activePlayer*8))
        {
            castles++;
            //\std::cout << "BEHOLD, FOR THIS IS A CASTLING MOVE DOWN BELOW" << std::endl;
            curcurFEN.GetPosition(SearchBoard.squareState, SearchBoard.activePlayer, SearchBoard.thisMoveTable.W_CanCastleKingside, SearchBoard.thisMoveTable.W_CanCastleQueenside, SearchBoard.thisMoveTable.B_CanCastleKingside, SearchBoard.thisMoveTable.B_CanCastleQueenside, SearchBoard.thisMoveTable.enPassantSquare, SearchBoard.thisMoveTable.consecutiveMoves);
            //\std::cout << curcurFEN.FENtext << std::endl;
            //\move.LogMove();
        }
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

        //\if(!change && numOfMoves[3-depth]!= SearchBoard.thisMoveTable.CurrentMoveList.size())
            //\std::cout << "LE SHOUT: le old: " << numOfMoves[3-depth] << " le new: " << SearchBoard.thisMoveTable.CurrentMoveList.size() << std::endl;
        change = false;
        numOfMoves[3-depth] = SearchBoard.thisMoveTable.CurrentMoveList.size();
        //std::cout << "Unmade has become that which had been done" << std::endl;
        //if(eval >= beta)
        //    return beta;
        //alpha = std::max(alpha, eval);
    }
    change = true;
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

    this->OrderMoves();

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

    ///DELETE THE IF
    if(depth == 6)
    {
        std::cout << "Final result:" << alpha << std::endl;
    }

    return alpha;
    
}

void Search::LogDebugSearch(int depth)
{
    depthMoveNum.resize(depth);
    counter=0;
    capture=0; ep=0; castles=0; promotions=0; checks=0; checkmates=0; draws=0;
    numOfMoves[0]=0;
    numOfMoves[1]=0;
    numOfMoves[2]=0;
    change = true;

    for(int i=0;i<depth;i++)
        depthMoveNum[i] = 0;

    for(int i=0;i<6;i++)
        pieceMoveNum[i] = 0;

    DebugSearch(depth);

    //\for(int i=depth-1;i>=0;i--)
    //\{
        //\std::cout << (depth-i) << ": " << depthMoveNum[i] << std::endl;
    //\}
    //for(int i=1;i<=6;i++)
        //std::cout << i << ": " << pieceMoveNum[i-1] << std::endl;

    capture+=ep;

    //\std::cout << "Captures: " << capture << std::endl;
    //\std::cout << "EP: " << ep << std::endl;
    //\std::cout << "Castles: " << castles << std::endl;
    //\std::cout << "Promotions:  " << promotions << std::endl;
    //\std::cout << "Checks:  " << checks << std::endl;
    //\std::cout << "Checkmates:  " << checkmates << std::endl;
}

void Search::OrderMoves()
{
    //std::vector<int> orderEvaluation;
    //orderEvaluation.resize(SearchBoard.thisMoveTable.CurrentMoveList.size());

    std::vector<std::pair<Move, int>> orderVector;

    int targetSquareState;
    int startSquarePieceType;
    int targetSquarePieceType;
    int moveScore;

    for(auto it = SearchBoard.thisMoveTable.CurrentMoveList.begin(); it != SearchBoard.thisMoveTable.CurrentMoveList.end(); it++)
    {
        moveScore=0;
        targetSquareState = SearchBoard.squareState[it->targetSquare];
        startSquarePieceType = Piece::ToType(SearchBoard.squareState[it->startSquare]);
        if(targetSquareState != 0)
        {
            targetSquarePieceType = Piece::ToType(targetSquareState);
            moveScore += 10 * targetSquarePieceType - startSquarePieceType;
        }

        if(it->promotionPiece != 0)
        {
            int promotionPieceType = Piece::ToType(it->promotionPiece);
            if(promotionPieceType == 6)
                moveScore += 50;
            else if(promotionPieceType == 3)
                moveScore += 25;

            for(auto iter = SearchBoard.thisMoveTable.AttackList.begin(); iter != SearchBoard.thisMoveTable.AttackList.end(); iter++)
            {
                if(*iter == it->targetSquare)
                {
                    moveScore -= 3*startSquarePieceType;
                    break;
                }
            }
            for(auto iter = SearchBoard.thisMoveTable.DefenseList.begin(); iter != SearchBoard.thisMoveTable.DefenseList.end(); iter++)
            {
                if(*iter == it->targetSquare)
                {
                    moveScore -= startSquarePieceType;
                    break;
                }
            }
        }

        orderVector.push_back(std::make_pair(*it,moveScore));
    }

    //\for(auto it = orderVector.begin(); it != orderVector.end(); it++)
    //\{
        //\std::cout << "'Tis an orderVector: (" << std::flush;
        //\it->first.LogMove();
        //\std::cout << ", " << it->second << ")" << std::endl;
    //\}

    //\std::cout << "We got before the qsort" << std::endl;

    /*std::qsort(&orderVector, orderVector.size(), sizeof(std::pair<Move,int>), [](const void *a, const void *b)
    {
        //int evalA = std::get<1>(*(std::pair<Move, int>*) a);
        //int evalB = std::get<1>(*(std::pair<Move, int>*) b);
        //const std::pair<Move,int> pairA = *(std::pair<Move,int>*) a;
        const std::pair<Move,int>* pairA = (const std::pair<Move, int>*) a;
        const std::pair<Move,int>* pairB = (const std::pair<Move, int>*) b;
        //const std::pair<Move,int> pairB = *(std::pair<Move,int>*) b;
        int evalA = pairA->second;
        int evalB = pairB->second;
        std::cout << "evalA: " << evalA << std::endl;
        std::cout << "evalB: " << evalB << std::endl;
        if(evalA > evalB)
            return 1;
        if(evalA == evalB)
            return 0;
        return -1;
    }
    );*/

    std::sort(orderVector.begin(),orderVector.end(), [](std::pair<Move,int> a, std::pair<Move,int> b)
    {
        int evalA = a.second;
        int evalB = b.second;

        //\std::cout << "evalA: " << evalA << std::endl;
        //\std::cout << "evalB: " << evalB << std::endl;

        if(evalA > evalB)
            return true;
        
        return false;
    }
    );

    std::vector<std::pair<Move,int>>::iterator debugIt = orderVector.begin();
    //\std::cout << (debugIt == orderVector.end()) << std::endl;
    debugIt++;

    //\std::cout << "We got after the qsort" << std::endl;

    //\for(auto it = orderVector.begin(); it != orderVector.end(); it++)
    //\{
        //\std::cout << "'Tis an orderVector: (" << std::flush;
        //\it->first.LogMove();
        //\std::cout << ", " << it->second << ")" << std::endl;
    //\}
    
    /*SearchBoard.thisMoveTable.CurrentMoveList.clear();

    for(auto it = orderVector.begin(); it != orderVector.end(); it++)
    {
        SearchBoard.thisMoveTable.CurrentMoveList.push_back(it->first);
    }*/

}