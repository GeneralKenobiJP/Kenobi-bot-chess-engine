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

void VirtualBoard::Promote(int square, int pieceID)
{
    this->PutOnSquare(square, pieceID);
}

void VirtualBoard::HandlePromotion(int promotionSpriteIndex) //obsolete
{
    int type = promotionSpriteIndex % 4 + 1;
    int color = (promotionSpriteIndex > 4) ? Piece::black : Piece::white;

    switch (type)
    {
    case 1:
        type = Piece::queen;
        break;
    case 2:
        type = Piece::knight;
        break;
    case 3:
        type = Piece::rook;
        break;
    case 4:
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
    //std::cout << PositionIndexHistory.size() << std::endl;
    bool hasCaptured = 0;
    bool isEnPassant = 0;
    int pieceType;
    int pieceColor;
    //FEN currentFEN;
    Piece::ReadPiece(squareState[startSquare], pieceType, pieceColor);
    //\std::cout << "The piece was hereby read as: " << squareState[startSquare] <<", " << pieceType << ", " << pieceColor << std::endl;
    //\std::cout << "Move" << startSquare << "->" << targetSquare << "(" << promotionNum << ")" << std::endl;

    int capturedPiece;

    if (pieceType == Piece::pawn)
    {
        isEnPassant = thisMoveTable.IsEnPassant(targetSquare);
        if (isEnPassant)
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
        if (promotionNum != 0) // PROMOTION
        {
            //\std::cout << "We are approaching the promotion, sir" << std::endl;
            if (squareState[targetSquare] != 0) // enemy piece
            {
                hasCaptured = true;
                capturedPiece = squareState[targetSquare];
                this->RemoveFromSquare(targetSquare);
                this->Promote(targetSquare, promotionNum);
                // Board::SwitchPlayer();
                // break;
            }
            else
            {
                this->Promote(targetSquare, promotionNum);
                // Board::SwitchPlayer();
                // break;
            }
            //\std::cout << "We've successfully completed the promotion" << std::endl;
        }
    }
    else
    {
        thisMoveTable.enPassantSquare = -1;
        EnPassantHistory.push_back(false);
    }

    //std::cout << "We got through the pawn stage" << std::endl;

    //currentFEN.GetPosition(squareState, activePlayer, thisMoveTable.W_CanCastleKingside, thisMoveTable.W_CanCastleQueenside, thisMoveTable.B_CanCastleKingside, thisMoveTable.B_CanCastleQueenside, thisMoveTable.enPassantSquare, thisMoveTable.consecutiveMoves);
    //std::cout << currentFEN.FENtext << std::endl;

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

        if (thisMoveTable.IsCastling(startSquare, targetSquare, pieceColor , IsKingside))
        {
            //std::cout << "CASTLING ATTEMPT DETECTED." << std::endl;
            if (IsKingside)
            {
                //std::cout << "kingside" << std::endl;
                this->RemoveFromSquare(targetSquare + 1);
                this->PutOnSquare(targetSquare - 1,Piece::rook, pieceColor);
            }
            else
            {
                this->RemoveFromSquare(targetSquare - 2);
                this->PutOnSquare(targetSquare + 1,Piece::rook, pieceColor);
            }
        }
    }

    //currentFEN.GetPosition(squareState, activePlayer, thisMoveTable.W_CanCastleKingside, thisMoveTable.W_CanCastleQueenside, thisMoveTable.B_CanCastleKingside, thisMoveTable.B_CanCastleQueenside, thisMoveTable.enPassantSquare, thisMoveTable.consecutiveMoves);
    //std::cout << currentFEN.FENtext << std::endl;
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

    //currentFEN.GetPosition(squareState, activePlayer, thisMoveTable.W_CanCastleKingside, thisMoveTable.W_CanCastleQueenside, thisMoveTable.B_CanCastleKingside, thisMoveTable.B_CanCastleQueenside, thisMoveTable.enPassantSquare, thisMoveTable.consecutiveMoves);
    //std::cout << currentFEN.FENtext << std::endl;
    //std::cout << "We got through the rook stage" << std::endl;

    if (squareState[targetSquare] != 0 && promotionNum == 0)
    {
        // Piece::RemovePieceSprite(i);
        capturedPiece = squareState[targetSquare];
        this->RemoveFromSquare(targetSquare);
        hasCaptured = true;
    }
    //\std::cout << "We got through the capture stage" << std::endl;

    if(hasCaptured && !isEnPassant)
        CaptureHistory.push_back(capturedPiece);
    else
        CaptureHistory.push_back(0); //Piece::none

    //\std::cout << "We got through the capture history stage" << std::endl;

    if (pieceType != Piece::pawn && !hasCaptured)
        thisMoveTable.consecutiveMoves++;
    else
        thisMoveTable.consecutiveMoves = 0;

    if(promotionNum==0)
        this->PutOnSquare(targetSquare, squareState[startSquare]);
    this->RemoveFromSquare(startSquare);
    //std::cout << "We got through the Put piece stage" << std::endl;
    this->SwitchPlayer();
    //std::cout << "We got through the switch player stage" << std::endl;
    // break;

    //std::cout << "We got through the consecutive moves stage" << std::endl;
    //std::cout << "MADE MOVE..." << std::flush;
    //Move move(startSquare,targetSquare, promotionNum);
    //move.LogMove();
    //currentFEN.GetPosition(squareState, activePlayer, thisMoveTable.W_CanCastleKingside, thisMoveTable.W_CanCastleQueenside, thisMoveTable.B_CanCastleKingside, thisMoveTable.B_CanCastleQueenside, thisMoveTable.enPassantSquare, thisMoveTable.consecutiveMoves);
    //std::cout << currentFEN.FENtext << std::endl;

    thisMoveTable.GenerateMoves(thisMoveTable.CurrentMoveList);

    //std::cout << "We got through the Generate Moves stage" << std::endl;
    //thisMoveTable.CheckState();
}

void VirtualBoard::MakeMove(std::vector<Move>::iterator &moveIterator)
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
    int pieceType, pieceColor;
    bool hasCaptured=false;
    bool isKingside;
    //1std::cout << "Unmaking move... :" << move.startSquare << "->" << move.targetSquare << "(" << move.promotionPiece << ")" << std::endl;
    if(move.promotionPiece == 0)
    {
        //\std::cout << "heyyeahyeah" << std::endl;
        //std::cout << "squareState: " << squareState[move.targetSquare] << std::endl;
        this->PutOnSquare(move.startSquare, squareState[move.targetSquare]);
    }
    else
    {
        this->PutOnSquare(move.startSquare, Piece::pawn, ((activePlayer%2)+1)*8);
        //\std::cout << "move.startSquare = " << move.startSquare << std::endl;
        //\std::cout << "squareState = " << squareState[move.startSquare] << std::endl;
    }

    //std::cout << "Thy piece has been cast upon the mortal soil of your choice" << std::endl;

    //std::cout << EnPassantHistory.back() << std::endl;
    if(!EnPassantHistory.empty())
    {
        if(EnPassantHistory.back() == true)
        {
            //std::cout << "ALMIGHTY EN PASSANT" << std::endl;
            //move.LogMove();
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
            hasCaptured = true;
        }
        CaptureHistory.pop_back();
    }

    //std::cout << "Before 'istory" << std::endl;
    //std::cout << PositionIndexHistory.back()->fen.FENtext << std::endl;
    //std::cout << PositionIndexHistory.size() << std::endl;

    if(!PositionIndexHistory.empty())
    {
        //std::cout << "We've been enlooped" << std::endl;
        thisMoveTable.RemovePosition(PositionIndexHistory.back());
        //std::cout << "El removal" << std::endl;
        PositionIndexHistory.pop_back();
    }

    //std::cout << "Francis Fukuyama" << std::endl;

    Piece::ReadPiece(squareState[move.startSquare],pieceType,pieceColor);

    //std::cout << "Piece Type: " << pieceType << std::endl;
    //std::cout << thisMoveTable.IsCastling(move.startSquare, move.targetSquare,pieceColor, isKingside) << std::endl;

    //FEN currentFEN;
    //currentFEN.GetPosition(squareState, activePlayer, thisMoveTable.W_CanCastleKingside, thisMoveTable.W_CanCastleQueenside, thisMoveTable.B_CanCastleKingside, thisMoveTable.B_CanCastleQueenside, thisMoveTable.enPassantSquare, thisMoveTable.consecutiveMoves);
    //std::cout << currentFEN.FENtext << std::endl;

    if(pieceType==Piece::king && thisMoveTable.IsCastling(move.startSquare, move.targetSquare, pieceColor , isKingside))
    {
        //std::cout << "CASTLING DETECTED. LETHAL PROCEDURES ENGAGED" << std::endl;
        if(isKingside)
        {
            this->RemoveFromSquare(move.startSquare+1);
            this->PutOnSquare(move.targetSquare+1, Piece::rook, pieceColor);
        }
        else
        {
            //currentFEN.GetPosition(squareState, activePlayer, thisMoveTable.W_CanCastleKingside, thisMoveTable.W_CanCastleQueenside, thisMoveTable.B_CanCastleKingside, thisMoveTable.B_CanCastleQueenside, thisMoveTable.enPassantSquare, thisMoveTable.consecutiveMoves);
    //std::cout << currentFEN.FENtext << std::endl;
            this->RemoveFromSquare(move.startSquare-1);
            this->PutOnSquare(move.targetSquare-2, Piece::rook, pieceColor);
            //currentFEN.GetPosition(squareState, activePlayer, thisMoveTable.W_CanCastleKingside, thisMoveTable.W_CanCastleQueenside, thisMoveTable.B_CanCastleKingside, thisMoveTable.B_CanCastleQueenside, thisMoveTable.enPassantSquare, thisMoveTable.consecutiveMoves);
    //std::cout << currentFEN.FENtext << std::endl;
        }
    }
    //auto x = PositionIndexHistory.begin();
    //std::cout << PositionIndexHistory.front()->fen.FENtext << std::endl;
    //std::cout << PositionIndexHistory.size() << std::endl;
    //std::cout << "Here" << std::endl;
    //std::cout << PositionIndexHistory.back()->occurrenceNum << std::endl;
    //std::cout << thisMoveTable.occurredPositions.size() << std::endl;
    PositionIndexHistory[PositionIndexHistory.size()-1]->fen.ReadContext(thisMoveTable.W_CanCastleKingside, thisMoveTable.W_CanCastleQueenside, thisMoveTable.B_CanCastleKingside, thisMoveTable.B_CanCastleQueenside, thisMoveTable.enPassantSquare, thisMoveTable.consecutiveMoves);

    this->RevertPlayer();
}

void VirtualBoard::SwitchPlayer()
{
    thisMoveTable.GenerateAttacks();
    //\std::cout << "Generated attacks" << std::endl;
    activePlayer = (activePlayer % 2) + 1;
    CurrentEvalution = VirtualEvaluation::Evaluate();
    //FEN curFEN;
    //curFEN.GetPosition();
    //std::cout << curFEN.FENtext << std::endl;
    //std::cout << curFEN.FENtext << std::endl;
    thisMoveTable.AddCurrentPosition(PositionIndexHistory);
    //PositionIndexHistory.push_back(thisMoveTable.occurredPositions.back());
    //std::cout << "Ze size: " << thisMoveTable.occurredPositions.size() << std::endl;
    //std::cout << thisMoveTable.occurredPositions[0].fen.FENtext << std::endl;
    //std::cout << thisMoveTable.occurredPositions[0].occurrenceNum << std::endl;
}
void VirtualBoard::RevertPlayer()
{
    FEN thisFEN;
    thisFEN.GetPosition(squareState, activePlayer, thisMoveTable.W_CanCastleKingside, thisMoveTable.W_CanCastleQueenside, thisMoveTable.B_CanCastleKingside, thisMoveTable.B_CanCastleQueenside, thisMoveTable.enPassantSquare, thisMoveTable.consecutiveMoves);
    //\std::cout << "Reverend: " << thisFEN.FENtext << std::endl;
    //\std::cout << "squareState[8]: " << squareState[8] << std::endl;
    //\std::cout << "squareState[9]: " << squareState[9] << std::endl;
    //\std::cout << "squareState[10]: " << squareState[10] << std::endl;
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
    //auto it = thisMoveTable.occurredPositions.begin();
    PositionIndexHistory.push_back(thisMoveTable.occurredPositions.begin());

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
    thisMoveTable.forbiddenEnPassantStartSquares = MoveTable::forbiddenEnPassantStartSquares;
    thisMoveTable.OwnEnPassantPins = MoveTable::OwnEnPassantPins;
    thisMoveTable.IsEnPassantPinned = MoveTable::IsEnPassantPinned;

    thisMoveTable.activePlayer = &activePlayer;
    
    for(int i=0;i<64;i++)
    {
        squareState[i] = Board::squareState[i];
        thisMoveTable.squareState[i] = &squareState[i];
    }

    //////////////////////////
}

