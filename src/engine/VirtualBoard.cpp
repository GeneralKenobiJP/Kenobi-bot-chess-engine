#include "VirtualBoard.h"
#include "piece.h"
#include <iostream>
//#include "MoveTable.h"
//#include "VirtualMoveTable.h"

////// UPDATE MOVE TABLE FUNCTIONS

void VirtualBoard::PutOnSquare(int num, int piece, int color)
{
    squareState[num] = (piece | color);
    //Piece::pieceList.
}
void VirtualBoard::PutOnSquare(int num, int pieceID)
{
    squareState[num] = pieceID;
    //Piece::pieceList.
}

void VirtualBoard::RemoveFromSquare(int num)
{
    squareState[num] = Piece::none;
}

void VirtualBoard::ReadSquare(int squareIndex, int &file, int &rank)
{
    file = squareIndex%8;
    rank = squareIndex/8;
}

int VirtualBoard::CalculateDistance(int squareA, int squareB)
{
    int fileA;
    int fileB;
    int rankA;
    int rankB;

    this->ReadSquare(squareA,fileA,rankA);
    this->ReadSquare(squareB,fileB,rankB);

    int dist = std::abs(fileA-fileB) + std::abs(rankA-rankB);

    return dist;
}

void VirtualBoard::Promote(int square, int color) //to develop
{
    //SpriteHandler::IsPromotion = true;
    //Board::promotionSquare = square;
    //SpriteHandler::ShowPromotionMenu(square);
}

void VirtualBoard::HandlePromotion(int promotionSpriteIndex)
{
    int type = promotionSpriteIndex%4;
    int color = (promotionSpriteIndex>3) ? Piece::black : Piece::white;
    
    switch(type)
    {
        case 0:
            type = Piece::queen;
            break;
        case 1:
            type = Piece::knight;
            break;
        case 2:
            type = Piece::rook;
            break;
        case 3:
            type = Piece::bishop;
            break;
    }

    this->PutOnSquare(promotionSquare, type, color);
    //SpriteHandler::HidePromotionMenu(type, color);
    promotionSquare = -1;
    //Board::selectedSquare = -1;
    //Board::SwitchPlayer();
    //MoveTable::GenerateMoves(MoveTable::CurrentMoveList); //DEVELOP
    //MoveTable::CheckState(); //DEVELOP
}

bool VirtualBoard::IsFileEmpty(int file)
{
    int square;
    for(square = file; square<64; square+=8)
    {
        if(Piece::ToType(squareState[square]) > Piece::king)
            return false;
    }
    return true;
}

bool VirtualBoard::IsFileBlocked(int file)
{
    int square;
    bool IsBlack = false;
    bool WasBlack = false;
    int type, color;

    if(this->IsFileEmpty(file))
        return true;
    
    /*if(file>0)
        if(!Board::IsFileEmpty(file-1))
            return false;

    if(file<7)
        if(!Board::IsFileEmpty(file+1))
            return false;*/
        
    for(square = file; square<64; square+=8)
    {
        std::cout << square << std::endl;
        Piece::ReadPiece(squareState[square],type,color);
        if(type>Piece::pawn)
            return false;
        if(type<Piece::pawn)
            continue;
        
        if(IsBlack == false && color == Piece::black)
            return false;
        if(IsBlack == true && color == Piece::white)
            return false;

        if(color == Piece::white)
        {
            if(!WasBlack)
            {
                IsBlack = true;
                if(!this->IsPawnBlockedAndNotNeighbour(square, Piece::white))
                    return false;
            }
            else
                return false;
        }
        if(IsBlack == true && color == Piece::black)
        {
            WasBlack = true;
            IsBlack = false;
            if(!this->IsPawnBlockedAndNotNeighbour(square, Piece::black))
                return false;
        }
    }

    if(IsBlack == true)
        return false;

    return true;
}

bool VirtualBoard::IsPawnBlockedAndNotNeighbour(int square, int color)
{
    if(this->IsFileEmpty(square%8))
        return true;
    if(Piece::ToColor(squareState[square])==color)
        return false;

    for(square = square%8; square<64; square+=8)
    {
        if(Piece::ToColor(squareState[square])==Piece::white)
            if(Piece::ToColor(squareState[square]!=Piece::black))
                return false;
    }
    return true;
}

bool VirtualBoard::IsKingBlocked(int color)
{
    int file,rank;
    int startRank = (color == 8) ? 0 : 7;
    int endRank = (color == 8) ? 8 : -1;
    int incr = (color == 8) ? 1 : -1;
    int square;
    bool IsRightConnected = false;
    bool IsLeftConnected = false;

    for(file=0;file<=7;file++)
    {
        if(rank==endRank)
            return false;
        for(rank=startRank;rank!=endRank;rank+=incr)
        {
            square = 8*rank + file;
            if(squareState[square] != 0 || thisMoveTable.IsAttacked(square))
            {
                if(file>0)
                {
                    if(squareState[square+7] != 0 || thisMoveTable.IsAttacked(square+7))
                        IsLeftConnected = true;
                    else if(squareState[square-1] != 0 || thisMoveTable.IsAttacked(square-1))
                        IsLeftConnected = true;
                    else if(squareState[square-9] != 0 || thisMoveTable.IsAttacked(square-9))
                        IsLeftConnected = true;
                    else
                        continue;
                }
                else
                    IsLeftConnected = true;

                if(file<7)
                {
                    if(squareState[square+9] != 0 || thisMoveTable.IsAttacked(square+9))
                        IsRightConnected = true;
                    else if(squareState[square+1] != 0 || thisMoveTable.IsAttacked(square+1))
                        IsRightConnected = true;
                    else if(squareState[square-7] != 0 || thisMoveTable.IsAttacked(square-7))
                        IsRightConnected = true;
                    else
                        continue;
                    
                }
                else
                    IsRightConnected = true;

                if(IsRightConnected && IsLeftConnected)
                    break;
            }
        }
    }

    return true;
}

void VirtualBoard::MakeMove(int startSquare, int targetSquare)
{
    
}