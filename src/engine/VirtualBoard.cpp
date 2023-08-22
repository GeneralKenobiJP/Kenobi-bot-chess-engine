#include "VirtualBoard.h"
#include "piece.h"
#include <iostream>
#include "VirtualEvaluation.h"
// #include "MoveTable.h"
// #include "VirtualMoveTable.h"

////// UPDATE MOVE TABLE FUNCTIONS

void VirtualBoard::PutOnSquare(int num, int piece, int color)
{
    squareState[num] = (piece | color);
    // Piece::pieceList.
}
void VirtualBoard::PutOnSquare(int num, int pieceID)
{
    squareState[num] = pieceID;
    // Piece::pieceList.
}

void VirtualBoard::RemoveFromSquare(int num)
{
    squareState[num] = Piece::none;
}

void VirtualBoard::ReadSquare(int squareIndex, int &file, int &rank)
{
    file = squareIndex % 8;
    rank = squareIndex / 8;
}

int VirtualBoard::CalculateDistance(int squareA, int squareB)
{
    int fileA;
    int fileB;
    int rankA;
    int rankB;

    this->ReadSquare(squareA, fileA, rankA);
    this->ReadSquare(squareB, fileB, rankB);

    int dist = std::abs(fileA - fileB) + std::abs(rankA - rankB);

    return dist;
}

void VirtualBoard::Promote(int square, int pieceID) // to develop
{
    PutOnSquare(square, pieceID);
}

void VirtualBoard::HandlePromotion(int promotionSpriteIndex)
{
    int type = promotionSpriteIndex % 4;
    int color = (promotionSpriteIndex > 3) ? Piece::black : Piece::white;

    switch (type)
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
    // SpriteHandler::HidePromotionMenu(type, color);
    promotionSquare = -1;
    // Board::selectedSquare = -1;
    // Board::SwitchPlayer();
    // MoveTable::GenerateMoves(MoveTable::CurrentMoveList); //DEVELOP
    // MoveTable::CheckState(); //DEVELOP
}

bool VirtualBoard::IsFileEmpty(int file)
{
    int square;
    for (square = file; square < 64; square += 8)
    {
        if (Piece::ToType(squareState[square]) > Piece::king)
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

    if (this->IsFileEmpty(file))
        return true;

    /*if(file>0)
        if(!Board::IsFileEmpty(file-1))
            return false;

    if(file<7)
        if(!Board::IsFileEmpty(file+1))
            return false;*/

    for (square = file; square < 64; square += 8)
    {
        //std::cout << square << std::endl;
        Piece::ReadPiece(squareState[square], type, color);
        if (type > Piece::pawn)
            return false;
        if (type < Piece::pawn)
            continue;

        if (IsBlack == false && color == Piece::black)
            return false;
        if (IsBlack == true && color == Piece::white)
            return false;

        if (color == Piece::white)
        {
            if (!WasBlack)
            {
                IsBlack = true;
                if (!this->IsPawnBlockedAndNotNeighbour(square, Piece::white))
                    return false;
            }
            else
                return false;
        }
        if (IsBlack == true && color == Piece::black)
        {
            WasBlack = true;
            IsBlack = false;
            if (!this->IsPawnBlockedAndNotNeighbour(square, Piece::black))
                return false;
        }
    }

    if (IsBlack == true)
        return false;

    return true;
}

bool VirtualBoard::IsPawnBlockedAndNotNeighbour(int square, int color)
{
    if (this->IsFileEmpty(square % 8))
        return true;
    if (Piece::ToColor(squareState[square]) == color)
        return false;

    for (square = square % 8; square < 64; square += 8)
    {
        if (Piece::ToColor(squareState[square]) == Piece::white)
            if (Piece::ToColor(squareState[square] != Piece::black))
                return false;
    }
    return true;
}

bool VirtualBoard::IsKingBlocked(int color)
{
    int file, rank;
    int startRank = (color == 8) ? 0 : 7;
    int endRank = (color == 8) ? 8 : -1;
    int incr = (color == 8) ? 1 : -1;
    int square;
    bool IsRightConnected = false;
    bool IsLeftConnected = false;

    for (file = 0; file <= 7; file++)
    {
        if (rank == endRank)
            return false;
        for (rank = startRank; rank != endRank; rank += incr)
        {
            square = 8 * rank + file;
            if (squareState[square] != 0 || thisMoveTable.IsAttacked(square))
            {
                if (file > 0)
                {
                    if (squareState[square + 7] != 0 || thisMoveTable.IsAttacked(square + 7))
                        IsLeftConnected = true;
                    else if (squareState[square - 1] != 0 || thisMoveTable.IsAttacked(square - 1))
                        IsLeftConnected = true;
                    else if (squareState[square - 9] != 0 || thisMoveTable.IsAttacked(square - 9))
                        IsLeftConnected = true;
                    else
                        continue;
                }
                else
                    IsLeftConnected = true;

                if (file < 7)
                {
                    if (squareState[square + 9] != 0 || thisMoveTable.IsAttacked(square + 9))
                        IsRightConnected = true;
                    else if (squareState[square + 1] != 0 || thisMoveTable.IsAttacked(square + 1))
                        IsRightConnected = true;
                    else if (squareState[square - 7] != 0 || thisMoveTable.IsAttacked(square - 7))
                        IsRightConnected = true;
                    else
                        continue;
                }
                else
                    IsRightConnected = true;

                if (IsRightConnected && IsLeftConnected)
                    break;
            }
        }
    }

    return true;
}

void VirtualBoard::MakeMove(int startSquare, int targetSquare, int promotionNum) // WE ARE ASSUMING IT IS A LEGAL MOVE
{
    bool hasCaptured = 0;
    int pieceType;
    int pieceColor;
    Piece::ReadPiece(squareState[startSquare], pieceType, pieceColor);
    //1std::cout << "The piece was hereby read as: " << squareState[startSquare] <<", " << pieceType << ", " << pieceColor << std::endl;
    //1std::cout << "Move" << startSquare << "->" << targetSquare << "(" << promotionNum << ")" << std::endl;

    int capturedPiece;

    if (pieceType == Piece::pawn)
    {
        if (thisMoveTable.IsEnPassant(targetSquare))
        {
            hasCaptured = true;
            if (activePlayer == 1)
            {
                this->RemoveFromSquare(targetSquare - 8);
            }
            else // black
            {
                this->RemoveFromSquare(targetSquare + 8);
            }
            EnPassantHistory.push_back(true);
            // break;
        }
        else
        {
            EnPassantHistory.push_back(false);
        }
        thisMoveTable.enPassantSquare = -1;
        if (thisMoveTable.IsTwoSquareAdvance(startSquare, targetSquare))
        {
            thisMoveTable.enPassantSquare = (targetSquare + startSquare) / 2;
        }
        if (targetSquare / 8 == 7 || targetSquare / 8 == 0) // PROMOTION
        {
            if (squareState[targetSquare] != 0) // enemy piece
            {
                hasCaptured = true;
                capturedPiece = squareState[targetSquare];
                this->RemoveFromSquare(targetSquare);
                this->Promote(targetSquare, pieceColor);
                // Board::SwitchPlayer();
                // break;
            }
            else
            {
                this->Promote(targetSquare, promotionNum);
                // Board::SwitchPlayer();
                // break;
            }
        }
    }
    else
        thisMoveTable.enPassantSquare = -1;

    //std::cout << "We got through the pawn stage" << std::endl;

    if (pieceType == Piece::king)
    {
        bool IsKingside;

        if (pieceColor == Piece::white)
        {
            thisMoveTable.W_CanCastleKingside = 0;
            thisMoveTable.W_CanCastleQueenside = 0;
        }
        else
        {
            thisMoveTable.B_CanCastleKingside = 0;
            thisMoveTable.B_CanCastleQueenside = 0;
        }

        if (thisMoveTable.IsCastling(startSquare, targetSquare, IsKingside))
        {
            if (IsKingside)
            {
                this->RemoveFromSquare(targetSquare + 1);
                this->PutOnSquare(Piece::rook, Piece::ToColor(squareState[startSquare]), targetSquare - 1);
            }
            else
            {
                this->RemoveFromSquare(targetSquare - 2);
                this->PutOnSquare(Piece::rook, Piece::ToColor(squareState[startSquare]), targetSquare + 1);
            }
        }
    }

    //std::cout << "We got through the king stage" << std::endl;

    if (pieceType == Piece::rook)
    {
        if (startSquare == 0) // a1
            thisMoveTable.W_CanCastleQueenside = 0;
        else if (startSquare == 7) // h1
            thisMoveTable.W_CanCastleKingside = 0;
        else if (startSquare == 56) // a8
            thisMoveTable.B_CanCastleQueenside = 0;
        else if (startSquare == 63) // h8
            thisMoveTable.B_CanCastleKingside = 0;
    }

    //std::cout << "We got through the rook stage" << std::endl;

    if (squareState[targetSquare] != 0)
    {
        // Piece::RemovePieceSprite(i);
        capturedPiece = squareState[targetSquare];
        this->RemoveFromSquare(targetSquare);
        hasCaptured = true;
    }
    //std::cout << "We got through the capture stage" << std::endl;

    if(hasCaptured)
        CaptureHistory.push_back(capturedPiece);
    else
        CaptureHistory.push_back(0); //Piece::none

    //std::cout << "We got through the capture history stage" << std::endl;

    this->PutOnSquare(targetSquare, squareState[startSquare]);
    this->RemoveFromSquare(startSquare);
    //std::cout << "We got through the Put piece stage" << std::endl;
    this->SwitchPlayer();
    //std::cout << "We got through the switch player stage" << std::endl;
    // break;

    if (pieceType != Piece::pawn && !hasCaptured)
        thisMoveTable.consecutiveMoves++;
    else
        thisMoveTable.consecutiveMoves = 0;

    //std::cout << "We got through the consecutive moves stage" << std::endl;

    thisMoveTable.GenerateMoves(thisMoveTable.CurrentMoveList);

    //std::cout << "We got through the Generate Moves stage" << std::endl;
    //thisMoveTable.CheckState();
}

void VirtualBoard::MakeMove(std::list<Move>::iterator &moveIterator)
{
    //std::cout << "Our iteration: " << moveIterator->startSquare << ", " << moveIterator->targetSquare << ", " << moveIterator->promotionPiece << std::endl;
    this->MakeMove(moveIterator->startSquare, moveIterator->targetSquare, moveIterator->promotionPiece);
}
void VirtualBoard::MakeMove(Move move)
{
    this->MakeMove(move.startSquare, move.targetSquare, move.promotionPiece);
}

void VirtualBoard::UnmakeMove(Move move)
{
    //1std::cout << "Unmaking move... :" << move.startSquare << "->" << move.targetSquare << "(" << move.promotionPiece << ")" << std::endl;
    if(move.promotionPiece == 0)
    {
        //std::cout << "squareState: " << squareState[move.targetSquare] << std::endl;
        this->PutOnSquare(move.startSquare, squareState[move.targetSquare]);
    }
    else
        this->PutOnSquare(move.startSquare, Piece::pawn ,Piece::ToColor(move.promotionPiece));

    //std::cout << "Thy piece has been cast upon the mortal soil of your choice" << std::endl;

    //std::cout << EnPassantHistory.back() << std::endl;
    if(!EnPassantHistory.empty())
    {
        if(EnPassantHistory.back() == true)
        {
            if(move.targetSquare/8 == 5)
                this->PutOnSquare(move.targetSquare-8, Piece::pawn, Piece::black);
            else
                this->PutOnSquare(move.targetSquare+8, Piece::pawn, Piece::white);
        }
        EnPassantHistory.pop_back();
    }

    //std::cout << "Step back, a peasant" << std::endl;

    this->RemoveFromSquare(move.targetSquare);

    if(!CaptureHistory.empty())
    {
        if(CaptureHistory.back() != 0)
        {
            this->PutOnSquare(move.targetSquare, CaptureHistory.back());
        }
        CaptureHistory.pop_back();
    }

    if(!PositionIndexHistory.empty())
    {
        thisMoveTable.RemovePosition(PositionIndexHistory.back());
        PositionIndexHistory.pop_back();
    }

    this->RevertPlayer();
}

void VirtualBoard::SwitchPlayer()
{
    thisMoveTable.GenerateAttacks();
    activePlayer = (activePlayer % 2) + 1;
    CurrentEvalution = VirtualEvaluation::Evaluate();
    thisMoveTable.AddCurrentPosition();
}
void VirtualBoard::RevertPlayer()
{
    thisMoveTable.GenerateAttacks();
    activePlayer = (activePlayer % 2) + 1;
    CurrentEvalution = VirtualEvaluation::Evaluate();
    thisMoveTable.GenerateMoves(thisMoveTable.CurrentMoveList);
    //thisMoveTable.AddCurrentPosition();
}

void VirtualBoard::InitializeEvaluation()
{
    VirtualEvaluation::board = this;
}

#include "board.h"

void VirtualBoard::InitializeBoard()
{
    this->InitializeEvaluation();
    thisMoveTable.CalculateStartMoveData();

    activePlayer = Board::activePlayer;

    thisMoveTable.occurredPositions = MoveTable::occurredPositions;
    thisMoveTable.consecutiveMoves = MoveTable::consecutiveMoves;
    thisMoveTable.IsThreefoldRepetition = MoveTable::IsThreefoldRepetition;
    thisMoveTable.IsFiftymove = MoveTable::IsFiftymove;

    thisMoveTable.CurrentMoveList = MoveTable::CurrentMoveList;
    thisMoveTable.AttackList = MoveTable::AttackList;
    thisMoveTable.PinList = MoveTable::PinList;
    thisMoveTable.PinDirectionList = MoveTable::PinDirectionList;
    thisMoveTable.VirtualAttackList = MoveTable::VirtualAttackList;
    thisMoveTable.DefenseList = MoveTable::DefenseList;
    thisMoveTable.IsChecked = MoveTable::IsChecked;
    thisMoveTable.KnightCheckNum = MoveTable::KnightCheckNum;
    thisMoveTable.CheckingKnightSquare = MoveTable::CheckingKnightSquare;
    thisMoveTable.CheckSquares = MoveTable::CheckSquares;
    thisMoveTable.enPassantSquare = MoveTable::enPassantSquare;
    thisMoveTable.W_CanCastleKingside = MoveTable::W_CanCastleKingside;
    thisMoveTable.W_CanCastleQueenside = MoveTable::W_CanCastleQueenside;
    thisMoveTable.B_CanCastleKingside = MoveTable::B_CanCastleKingside;
    thisMoveTable.B_CanCastleQueenside = MoveTable::B_CanCastleQueenside;

    thisMoveTable.activePlayer = &activePlayer;
    
    for(int i=0;i<64;i++)
    {
        squareState[i] = Board::squareState[i];
        thisMoveTable.squareState[i] = &squareState[i];
    }

    //////////////////////////
}

