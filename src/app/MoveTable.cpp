#include "MoveTable.h"
#include <algorithm>
#include "board.h"
#include "piece.h"
#include <iostream>

//We start at N and go clockwise

const short MoveTable::knightShift[8] = {15,17,6,10,-15,-17,-6,-10};
const short MoveTable::directionShift[8] = {8,9,1,-7,-8,-9,-1,7};
short MoveTable::numSquaresToEdge[64][8];
std::vector<std::vector<short>> MoveTable::knightTargetSquares;
std::vector<std::vector<short>> MoveTable::kingTargetSquares;
std::list<Move> MoveTable::CurrentMoveList;
std::list<int> MoveTable::AttackList;
std::list<int> MoveTable::PinList;
short MoveTable::enPassantSquare = -1;
bool MoveTable::W_CanCastleKingside;
bool MoveTable::B_CanCastleKingside;
bool MoveTable::W_CanCastleQueenside;
bool MoveTable::B_CanCastleQueenside;

void MoveTable::CalculateStartMoveData()
{
    int squareX;
    int squareY;
    int north;
    int east;
    int south;
    int west;

    int knightTarget;
    int kingTarget;

    knightTargetSquares.resize(64);
    kingTargetSquares.resize(64);

    for(int i=0;i<64;i++)
    {
        squareY = i/8;
        squareX = i%8;

        north = 7-squareY;
        east = 7-squareX;
        south = squareY;
        west = squareX;

        //N
        MoveTable::numSquaresToEdge[i][0] = north;
        //NE
        MoveTable::numSquaresToEdge[i][1] = std::min(north,east);
        //E
        MoveTable::numSquaresToEdge[i][2] = east;
        //SE
        MoveTable::numSquaresToEdge[i][3] = std::min(south,east);;
        //S
        MoveTable::numSquaresToEdge[i][4] = south;
        //SW
        MoveTable::numSquaresToEdge[i][5] = std::min(south,west);
        //W
        MoveTable::numSquaresToEdge[i][6] = west;
        //NW
        MoveTable::numSquaresToEdge[i][7] = std::min(north,west);

        //Knight moves
        for(int j=0;j<8;j++)
        {
            knightTarget = i+MoveTable::knightShift[j];
            if(knightTarget>=0 && knightTarget<64)
            {
                //Board::ReadSquare(knightTarget,knightTargetX,knightTargetY);
                int distance = Board::CalculateDistance(i,knightTarget);
                if(distance == 3) //preventing jumping across the board boundary to an opposite end
                {
                    MoveTable::knightTargetSquares[i].push_back(knightTarget);
                }
            }
        }

        //King moves
        for(int j=0;j<8;j++)
        {
            kingTarget = i+MoveTable::directionShift[j];
            if(kingTarget>=0 && kingTarget<64)
            {
                int distance = Board::CalculateDistance(i,kingTarget);
                if(distance <= 2) //preventing jumping across the board boundary to an opposite end
                {
                    MoveTable::kingTargetSquares[i].push_back(kingTarget);
                }
            }
        }
    }
}

std::list<Move> MoveTable::GenerateMoves()
{
    std::list<Move> MoveList;
    int pieceType;
    int pieceColor;

    for(int i=0;i<64;i++)
    {   
        Piece::ReadPiece(Board::squareState[i],pieceType,pieceColor);

        if(pieceColor/8 != Board::activePlayer)
            continue;

        if(Piece::IsLongRange(pieceType))
        {
            MoveTable::GenerateLongRangeMoves(i,pieceType,MoveList);
        }
        else if(pieceType == Piece::knight)
        {
            MoveTable::GenerateKnightMoves(i,MoveList);
        }
        else if(pieceType == Piece::pawn)
        {
            MoveTable::GeneratePawnMoves(i,MoveList);
        }
        else //king
        {
            MoveTable::GenerateKingMoves(i,MoveList);
        }

        //if(pieceColor != 0)
            //break;
        
    }

    return MoveList;

}

void MoveTable::GenerateMoves(std::list<Move> &moveList)
{
    moveList = MoveTable::GenerateMoves();
}

void MoveTable::GenerateLongRangeMoves(int square, int pieceType, std::list<Move> &moveList)
{
    int startDir = (pieceType == Piece::bishop) ? 1 : 0; //if true => 1
    int dirIncr = (pieceType == Piece::queen) ? 1 : 2; //if true => 1
    int targetSquare;
    int targetSquareState;
    int targetSquarePieceColor;

    for(int dir=startDir;dir<8;dir+=dirIncr)
    {
        for(int sq=1;sq<=MoveTable::numSquaresToEdge[square][dir];sq++)
        {
            targetSquare = square + MoveTable::directionShift[dir] * sq;
            targetSquareState = Board::squareState[targetSquare];
            Piece::ReadPieceColor(targetSquareState,targetSquarePieceColor);
            
            if(targetSquarePieceColor/8 == Board::activePlayer) //our piece is blocking the way
            {
                break;
            }

            moveList.push_front(Move(square,targetSquare));
            std::cout << square << ' ' << targetSquare << std::endl;

            if(targetSquarePieceColor != 0) //opponent's piece is blocking the way
            {

                /*for(int pinSq = sq; pinSq <= MoveTable::numSquaresToEdge[square][dir];pinSq++) //we're checking if the blocking piece is pinned
                {
                    targetSquare = square + MoveTable::directionShift[dir] * pinSq;
                    targetSquareState = Board::squareState[targetSquare];
                    
                    if(targetSquareState != 0)
                    {
                        if()
                    }
                }*/

                break;
            }

        }
    }
}

void MoveTable::GenerateKnightMoves(int square, std::list<Move> &moveList)
{
    int targetSquareColor;

    for(int i=0;i<MoveTable::knightTargetSquares[square].size();i++)
    {
        Piece::ReadPieceColor(Board::squareState[MoveTable::knightTargetSquares[square][i]],targetSquareColor);

        if(targetSquareColor/8 != Board::activePlayer)
        {
            moveList.push_front(Move(square,knightTargetSquares[square][i]));
        }
    }
}

void MoveTable::GeneratePawnMoves(int square, std::list<Move> &moveList)
{
    int pawnMoveShift = (Board::activePlayer == 1) ? 8 : -8;
    int pawnAttackShift[2];
    int targetSquare;
    int targetSquareColor;
    bool initialPos = 0;

    if(Board::activePlayer == 1)
    {
        pawnAttackShift[0] = 7;
        pawnAttackShift[1] = 9;
    }
    else
    {
        pawnAttackShift[0] = -7;
        pawnAttackShift[1] = -9;
    }

    targetSquare = square + pawnMoveShift;
    if(Board::squareState[targetSquare]==0 && targetSquare<64 && targetSquare >= 0)
    {
        moveList.push_front(Move(square,targetSquare));
    }
    
    for(int i=0;i<2;i++)
    {
        targetSquare = square + pawnAttackShift[i];
        if(targetSquare<0 || targetSquare>63)
            break;
        Piece::ReadPieceColor(Board::squareState[targetSquare],targetSquareColor);
        //EN PASSANT
        if(enPassantSquare == targetSquare)
        {
            moveList.push_front(Move(square,targetSquare));
            continue;
            //REMOVE EN PASSANT-ED PAWN
        }
        if(targetSquareColor != 0)
        {
            if(targetSquareColor/8 != Board::activePlayer)
                moveList.push_front(Move(square,targetSquare));
        }
    }

    // TWO-SQUARE ADVANCE

    if(Board::activePlayer == 1 && square/8 == 1)
    {
        initialPos = 1;
    }
    if(Board::activePlayer == 2 && square/8 == 6)
    {
        initialPos = 1;
    }

    if(initialPos)
    {
        targetSquare = square + 2*pawnMoveShift;
        if(Board::squareState[targetSquare]==0)
            moveList.push_front(Move(square,targetSquare));
    }

}

void MoveTable::GenerateKingMoves(int square, std::list<Move> &moveList)
{
    int targetSquareColor;

    for(int i=0;i<MoveTable::kingTargetSquares[square].size();i++)
    {
        Piece::ReadPieceColor(Board::squareState[MoveTable::kingTargetSquares[square][i]],targetSquareColor);

        if(targetSquareColor/8 != Board::activePlayer)
        {
            moveList.push_front(Move(square,kingTargetSquares[square][i]));
        }
    }
}

bool MoveTable::IsLegal(int startSquare, int targetSquare)
{
    std::list<Move>::iterator it;

    for(it=MoveTable::CurrentMoveList.begin();it!=MoveTable::CurrentMoveList.end();it++)
    {
        if(it->startSquare == startSquare)
            if(it->targetSquare == targetSquare)
                return true;
    }

    return false;
}

bool MoveTable::IsEnPassant(int targetSquare) //we're assuming we've already tested for being a pawn
{
    if(targetSquare == MoveTable::enPassantSquare)
        return true;
    return false;
}
bool MoveTable::IsTwoSquareAdvance(int startSquare, int targetSquare) //we're assuming we've already tested for being a pawn
{
    int diff = targetSquare-startSquare;
    if(diff == 16 || diff == -16)
        return true;
    return false;
}

void MoveTable::GenerateAttacks()
{
    MoveTable::AttackList.clear();
    MoveTable::PinList.clear();

    std::list<Move>::iterator it;
    int startSquarePiece;
    int startSquareType;

    for(it=MoveTable::CurrentMoveList.begin();it!=MoveTable::CurrentMoveList.end();it++)
    {
        MoveTable::AttackList.push_front(it->targetSquare);

        startSquarePiece = Board::squareState[it->startSquare];

        Piece::ReadPieceType(startSquarePiece,startSquareType);

        if(Piece::IsLongRange(startSquareType))
            CheckForPins(it->startSquare,it->targetSquare);
    }
}

void MoveTable::CheckForPins(int startSquare, int targetSquare)
{
    int diff = (targetSquare - startSquare);

    int startFile, startRank;
    int targetFile, targetRank;

    int dir;

    int pinTargetSquare;

    Board::ReadSquare(startSquare, startFile, startRank);
    Board::ReadSquare(targetSquare, targetFile, targetRank);

    if(startFile == targetFile)
    {
        if(diff>0)
            dir = 8;
        else
            dir = -8;
    }
    else if(startRank == targetRank)
    {
        if(diff>0)
            dir = 1;
        else
            dir = -1;
    }
    else
    {
        if(diff%9==0) //if it's 63=7*9, it must be 7 times +9 moves, because the maximum move is 8 squares
        {
            if(diff>0)
                dir = 9;
            else
                dir = -9;
        }
        else
        {
            if(diff>0)
                dir = 7;
            else
                dir = -7;
        }

    }
    //THE FUNCTION IS CALLED JUST BEFORE SWITCHING PLAYERS
    for(int i=1;i<=MoveTable::numSquaresToEdge[targetSquare][dir];i++)
    {
        pinTargetSquare = targetSquare + dir*i;
        
        if(Board::squareState[pinTargetSquare] != 0)
        {
            if(Piece::IsEnemyKing(Board::squareState[pinTargetSquare]))
            {
                PinList.push_front(targetSquare);
                break;
            }
            else
            {
                break;
            }
        }

    }
}

bool MoveTable::IsPinned(int square)
{
    std::list<int>::iterator it;

    for(it=MoveTable::PinList.begin();it!=MoveTable::PinList.end();it++)
        if(*it == square)
            return true;
    
    return false;
}

bool MoveTable::IsAttacked(int square)
{
    std::list<int>::iterator it;

    for(it=MoveTable::AttackList.begin();it!=MoveTable::AttackList.end();it++)
        if(*it == square)
            return true;
    
    return false;
}