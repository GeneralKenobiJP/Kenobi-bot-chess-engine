#include "VirtualMoveTable.h"
#include <algorithm>
#include "board.h"
#include "piece.h"
#include <iostream>
#include "VirtualEvaluation.h"

/////FIX DRAWS

//We start at N and go clockwise
const short VirtualMoveTable::knightShift[8] = {15,17,6,10,-15,-17,-6,-10};
const short VirtualMoveTable::directionShift[8] = {8,9,1,-7,-8,-9,-1,7};
short VirtualMoveTable::numSquaresToEdge[64][8];
std::vector<std::vector<short>> VirtualMoveTable::knightTargetSquares;
std::vector<std::vector<short>> VirtualMoveTable::kingTargetSquares;
std::list<int> VirtualMoveTable::pawnAttackList;
std::list<int> VirtualMoveTable::kingVirtualAttackList;
/*std::list<Move> MoveTable::CurrentMoveList;
std::list<int> MoveTable::AttackList;
std::list<int> MoveTable::PinList;
short MoveTable::enPassantSquare = -1;
bool MoveTable::W_CanCastleKingside;
bool MoveTable::B_CanCastleKingside;
bool MoveTable::W_CanCastleQueenside;
bool MoveTable::B_CanCastleQueenside;
std::list<int> MoveTable::pawnAttackList;
std::list<int> MoveTable::PinDirectionList;
std::list<int> MoveTable::VirtualAttackList;
bool MoveTable::IsChecked = false;
short MoveTable::KnightCheckNum = 0;
std::vector<std::list<int>> MoveTable::CheckSquares;
int MoveTable::CheckingKnightSquare = -1;
std::list<int> MoveTable::DefenseList;
std::list<int> MoveTable::kingVirtualAttackList;
int MoveTable::consecutiveMoves=0;
std::list<Position> MoveTable::occurredPositions;
bool MoveTable::IsThreefoldRepetition = false;
bool MoveTable::IsFiftymove = false;*/

void VirtualMoveTable::CalculateStartMoveData()
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
        VirtualMoveTable::numSquaresToEdge[i][0] = north;
        //NE
        VirtualMoveTable::numSquaresToEdge[i][1] = std::min(north,east);
        //E
        VirtualMoveTable::numSquaresToEdge[i][2] = east;
        //SE
        VirtualMoveTable::numSquaresToEdge[i][3] = std::min(south,east);;
        //S
        VirtualMoveTable::numSquaresToEdge[i][4] = south;
        //SW
        VirtualMoveTable::numSquaresToEdge[i][5] = std::min(south,west);
        //W
        VirtualMoveTable::numSquaresToEdge[i][6] = west;
        //NW
        VirtualMoveTable::numSquaresToEdge[i][7] = std::min(north,west);

        //Knight moves
        for(int j=0;j<8;j++)
        {
            knightTarget = i+VirtualMoveTable::knightShift[j];
            if(knightTarget>=0 && knightTarget<64)
            {
                //Board::ReadSquare(knightTarget,knightTargetX,knightTargetY);
                int distance = Board::CalculateDistance(i,knightTarget);
                if(distance == 3) //preventing jumping across the board boundary to an opposite end
                {
                    VirtualMoveTable::knightTargetSquares[i].push_back(knightTarget);
                }
            }
        }

        //King moves
        for(int j=0;j<8;j++)
        {
            kingTarget = i+VirtualMoveTable::directionShift[j];
            if(kingTarget>=0 && kingTarget<64)
            {
                int distance = Board::CalculateDistance(i,kingTarget);
                if(distance <= 2) //preventing jumping across the board boundary to an opposite end
                {
                    VirtualMoveTable::kingTargetSquares[i].push_back(kingTarget);
                }
            }
        }
    }
}

std::list<Move> VirtualMoveTable::GenerateMoves() ///DRAWS DEBUGGING
{
    std::list<Move> MoveList;
    int pieceType;
    int pieceColor;

    VirtualMoveTable::pawnAttackList.clear();
    VirtualMoveTable::kingVirtualAttackList.clear();

    if(consecutiveMoves >= 150) //leaving space for nuances here
        Board::DeclareDraw();
    else if(consecutiveMoves >= 100)
        IsFiftymove = true;

    if(!this->IsSufficientMaterial())
        Board::DeclareDraw();

    if(IsFiftymove || IsThreefoldRepetition)
    {
        Board::CanDeclareDraw = true;
    }

    for(int i=0;i<64;i++)
    {
        Piece::ReadPiece(Board::squareState[i],pieceType,pieceColor);

        if(pieceColor/8 != Board::activePlayer)
            continue;

        if(Piece::IsLongRange(pieceType))
        {
            this->GenerateLongRangeMoves(i,pieceType,MoveList);
        }
        else if(pieceType == Piece::knight)
        {
            this->GenerateKnightMoves(i,MoveList);
        }
        else if(pieceType == Piece::pawn)
        {
            this->GeneratePawnMoves(i,MoveList);
        }
        else //king
        {
            this->GenerateKingMoves(i,MoveList);
        }

        //if(pieceColor != 0)
            //break;
        
    }

    return MoveList;

}

void VirtualMoveTable::GenerateMoves(std::list<Move> &moveList)
{
    moveList = this->GenerateMoves();
}

void VirtualMoveTable::GenerateLongRangeMoves(int square, int pieceType, std::list<Move> &moveList)
{
    int startDir = (pieceType == Piece::bishop) ? 1 : 0; //if true => 1
    int dirIncr = (pieceType == Piece::queen) ? 1 : 2; //if true => 1
    int targetSquare;
    int targetSquareState;
    int targetSquarePieceColor;
    bool IsPinned;
    int pinDir;

    IsPinned = this->IsPinned(square, pinDir);

    for(int dir=startDir;dir<8;dir+=dirIncr)
    {
        if(IsPinned)
                if(std::abs(pinDir) != std::abs(VirtualMoveTable::directionShift[dir]))
                    continue;
        
        for(int sq=1;sq<=MoveTable::numSquaresToEdge[square][dir];sq++)
        {
            targetSquare = square + VirtualMoveTable::directionShift[dir] * sq;

            targetSquareState = Board::squareState[targetSquare];
            Piece::ReadPieceColor(targetSquareState,targetSquarePieceColor);
            
            if(targetSquarePieceColor/8 == Board::activePlayer) //our piece is blocking the way
            {
                DefenseList.push_back(targetSquare);
                break;
            }

            if(!IsChecked || this->IsCoveringCheck(targetSquare))
            {
                moveList.push_front(Move(square,targetSquare));
            }

            std::cout << square << ' ' << targetSquare << std::endl;

            if(targetSquarePieceColor != 0) //opponent's piece is blocking the way
            {
                break;
            }

        }
    }
}

void VirtualMoveTable::GenerateKnightMoves(int square, std::list<Move> &moveList)
{
    int targetSquareColor;

    if(this->IsPinned(square))
        return;

    for(int i=0;i<VirtualMoveTable::knightTargetSquares[square].size();i++)
    {
        if(IsChecked && !this->IsCoveringCheck(VirtualMoveTable::knightTargetSquares[square][i]))
            continue;

        Piece::ReadPieceColor(Board::squareState[VirtualMoveTable::knightTargetSquares[square][i]],targetSquareColor);

        if(targetSquareColor/8 != Board::activePlayer)
        {
            moveList.push_front(Move(square,knightTargetSquares[square][i]));
        }
        else
            DefenseList.push_back(knightTargetSquares[square][i]);
    }
}

void VirtualMoveTable::GeneratePawnMoves(int square, std::list<Move> &moveList)
{
    int pawnMoveShift = (Board::activePlayer == 1) ? 8 : -8;
    int pawnAttackShift[2];
    int targetSquare;
    int targetSquareColor;
    bool initialPos = 0;
    bool IsPinned;
    int pinDir;

    IsPinned = this->IsPinned(square, pinDir);

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

    if((IsPinned && std::abs(pinDir) == std::abs(pawnMoveShift)) || !IsPinned)
    {
        targetSquare = square + pawnMoveShift;
        if(!IsChecked || this->IsCoveringCheck(targetSquare))
        {
            if(Board::squareState[targetSquare]==0 && targetSquare<64 && targetSquare >= 0)
            {
                moveList.push_front(Move(square,targetSquare));
            }
        }
    }
    
    for(int i=0;i<2;i++)
    {
        if(IsPinned && std::abs(pinDir)!= std::abs(pawnAttackShift[i]))
            continue;

        targetSquare = square + pawnAttackShift[i];

        if(IsChecked && !this->IsCoveringCheck(targetSquare))
            continue;

        if(targetSquare<0 || targetSquare>63)
            break;
        if((targetSquare % 8 != square % 8 - 1) && (targetSquare % 8 != square % 8 + 1)) //preventing from going around the board
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
            else
                DefenseList.push_back(targetSquare);
            
            continue;
        }

        pawnAttackList.push_front(targetSquare);
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
        if(!IsPinned || (IsPinned && std::abs(pinDir) == std::abs(pawnMoveShift)))
        {
            targetSquare = square + 2*pawnMoveShift;
            if(IsChecked && !this->IsCoveringCheck(targetSquare))
                return;
            int passingSquare = square + pawnMoveShift;
            if(Board::squareState[targetSquare]==0 && Board::squareState[passingSquare] == 0)
                moveList.push_front(Move(square,targetSquare));
        }
    }

}

void VirtualMoveTable::GenerateKingMoves(int square, std::list<Move> &moveList)
{
    int targetSquareColor;

    for(int i=0;i<VirtualMoveTable::kingTargetSquares[square].size();i++)
    {
        if(this->IsAttacked(VirtualMoveTable::kingTargetSquares[square][i]))
        {
            kingVirtualAttackList.push_back(kingTargetSquares[square][i]);
            continue;
        }

        if(this->IsVirtuallyAttacked(VirtualMoveTable::kingTargetSquares[square][i]))
        {
            kingVirtualAttackList.push_back(kingTargetSquares[square][i]);
            continue;
        }

        if(this->IsDefended(VirtualMoveTable::kingTargetSquares[square][i]))
            continue;

        Piece::ReadPieceColor(Board::squareState[VirtualMoveTable::kingTargetSquares[square][i]],targetSquareColor);

        if(targetSquareColor/8 != Board::activePlayer)
        {
            moveList.push_front(Move(square,kingTargetSquares[square][i]));
        }
        else if(targetSquareColor != 0)
            DefenseList.push_back(kingTargetSquares[square][i]);
    }

    if(IsChecked)
        return;

    if(Board::activePlayer == 1) //white
    {
        if(W_CanCastleKingside)
            if(Board::squareState[7]==13) //white rook
                if(Board::squareState[5]==0 && Board::squareState[6] == 0)
                    if(!this->IsAttacked(5) && !this->IsAttacked(6))
                        moveList.push_front(Move(square,6));

        if(W_CanCastleQueenside)
            if(Board::squareState[0]==13) //white rook
                if(Board::squareState[2]==0 && Board::squareState[3] == 0)
                    if(!this->IsAttacked(2) && !this->IsAttacked(3))
                        moveList.push_front(Move(square,2));
    }
    if(Board::activePlayer == 2) //black
    {
        if(B_CanCastleKingside)
            if(Board::squareState[63]==21) //black rook
                if(Board::squareState[61]==0 && Board::squareState[62] == 0)
                    if(!this->IsAttacked(61) && !this->IsAttacked(62))
                        moveList.push_front(Move(square,62));

        if(B_CanCastleQueenside)
            if(Board::squareState[56]==21) //black rook
                if(Board::squareState[58]==0 && Board::squareState[59] == 0)
                    if(!this->IsAttacked(58) && !this->IsAttacked(59))
                        moveList.push_front(Move(square,58));
    }
}

bool VirtualMoveTable::IsLegal(int startSquare, int targetSquare)
{
    std::list<Move>::iterator it;

    for(it=CurrentMoveList.begin();it!=CurrentMoveList.end();it++)
    {
        if(it->startSquare == startSquare)
            if(it->targetSquare == targetSquare)
                return true;
    }

    return false;
}

bool VirtualMoveTable::IsEnPassant(int targetSquare) //we're assuming we've already tested for being a pawn
{
    if(targetSquare == enPassantSquare)
        return true;
    return false;
}
bool VirtualMoveTable::IsTwoSquareAdvance(int startSquare, int targetSquare) //we're assuming we've already tested for being a pawn
{
    int diff = targetSquare-startSquare;
    if(diff == 16 || diff == -16)
        return true;
    return false;
}

void VirtualMoveTable::GenerateAttacks()
{

    DefenseList.clear();

    this->GenerateMoves(CurrentMoveList);

    IsChecked = false;
    KnightCheckNum = 0;
    CheckingKnightSquare = -1;

    AttackList.clear();
    PinList.clear();
    PinDirectionList.clear();
    CheckSquares.clear();
    VirtualAttackList.clear();

    std::list<Move>::iterator it;
    int startSquarePiece;
    int startSquareType;


    for(it=CurrentMoveList.begin();it!=CurrentMoveList.end();it++)
    {
        //std::cout << "Here, sir" << std::endl;

        if(Piece::ToType(Board::squareState[it->startSquare]) == Piece::pawn)
            if(it->startSquare % 8 == it->targetSquare % 8) //this is not an attack for pawn (detection of the push foward move)
                continue;
        
        MoveTable::AttackList.push_front(it->targetSquare);

        //std::cout << "Pushed for attack. From: " << it->startSquare << " to: " << it->targetSquare << std::endl;

        startSquarePiece = Board::squareState[it->startSquare];

        Piece::ReadPieceType(startSquarePiece,startSquareType);

        if(Piece::IsLongRange(startSquareType))
            if(Board::squareState[it->targetSquare] != 0)
                this->CheckForPins(it->startSquare,it->targetSquare);

        if(startSquareType == Piece::knight)
            this->CheckForKnightChecks(it->targetSquare, it->startSquare);

        if(startSquareType == Piece::pawn)
            this->CheckForPawnChecks(it->targetSquare);
    }

    //std::cout << "And for pawns" << std::endl;

    std::list<int>::iterator iter;

    for(iter = pawnAttackList.begin(); iter!= pawnAttackList.end();iter++)
    {
        AttackList.push_front(*iter);
        //std::cout << "Pushed pawn for attack:  "<< *iter << std::endl;
    }
    for(iter = kingVirtualAttackList.begin(); iter!= kingVirtualAttackList.end();iter++)
    {
        AttackList.push_front(*iter);
        //std::cout << "Pushed king for attack:  "<< *iter << std::endl;
    }

    AttackList.unique();
    PinList.unique();
    DefenseList.unique();

    //std::cout << "Well: " << MoveTable::AttackList.size() << std::endl;
    //std::cout << "Hath our monarch been checked? " << MoveTable::IsChecked << std::endl;
    //std::cout << "The knight pale hath attacked in the count of " << MoveTable::KnightCheckNum << std::endl;
}

void VirtualMoveTable::CheckForPins(int startSquare, int targetSquare)
{
    int diff = (targetSquare - startSquare);

    int startFile, startRank;
    int targetFile, targetRank;

    int dir, dirIndex;

    int pinTargetSquare;

    //bool IsVirtual = false;

    Board::ReadSquare(startSquare, startFile, startRank);
    Board::ReadSquare(targetSquare, targetFile, targetRank);

    if(startFile == targetFile) //vertical move
    {
        if(diff>0) //up
        {
            dir = 8;
            dirIndex = 0;
        }
        else //down
        {
            dir = -8;
            dirIndex = 4;
        }
    }
    else if(startRank == targetRank) //horizontal move
    {
        if(diff>0) //right
        {
            dir = 1;
            dirIndex = 2;
        }
        else //left
        {
            dir = -1;
            dirIndex = 6;
        }
    }
    else //diagonal move
    {
        if(diff%9==0) //if it's 63=7*9, it must be 7 times +9 moves, because the maximum move is 8 squares
        {
            if(diff>0) //NE
            {
                dir = 9;
                dirIndex = 1;
            }
            else //SW
            {
                dir = -9;
                dirIndex = 5;
            }
        }
        else
        {
            if(diff>0) //NW
            {
                dir = 7;
                dirIndex = 7;
            }
            else //SE
            {
                dir = -7;
                dirIndex = 3;
            }
        }

    }

    //THE FUNCTION IS CALLED JUST BEFORE SWITCHING PLAYERS
    for(int i=1;i<=VirtualMoveTable::numSquaresToEdge[targetSquare][dirIndex];i++)
    {
        pinTargetSquare = targetSquare + dir*i;
        
        if(Board::squareState[pinTargetSquare] != 0)
        {
            if(Piece::IsEnemyKing(Board::squareState[pinTargetSquare]))
            {
                /*std::cout << "Hereby, there stands a king: " << pinTargetSquare << std::endl;
                std::cout << "And the direction of the pin is: " << dir << std::endl;
                std::cout << pinTargetSquare << " = " << targetSquare << " + " << dir << " * " << i << std::endl;
                std::cout << "numSquaresToEdge: " << MoveTable::numSquaresToEdge[targetSquare][dir] << std::endl;
                std::cout << "targetSquare: " << targetSquare << std::endl;*/
                PinList.push_front(targetSquare);
                PinDirectionList.push_front(dir);
                //std::cout << "Pinned dir: " << dir << std::endl;
                break;
            }
            else
            {
                break;
            }
        }

    }

    ///CHECKING FOR CHECKS AND VIRTUAL ATTACKS
    if(Piece::IsEnemyKing(Board::squareState[targetSquare]))
    {
        IsChecked = true;

        int thisSquare;
        std::list<int> thisList;
        for(int i=targetSquare-dir;i!=startSquare;i-=dir)
        {
            thisList.push_back(i);
        }
        for(int i=1;i <= VirtualMoveTable::numSquaresToEdge[targetSquare][dirIndex];i++)
        {
            thisSquare = targetSquare + i*dir;
            if(Board::squareState[thisSquare] != 0)
                break;
            //std::cout << "Virtual square: " << thisSquare << std::endl;
            VirtualAttackList.push_back(thisSquare);
        }
        thisList.push_back(startSquare);
        CheckSquares.push_back(thisList);
    }

}

bool VirtualMoveTable::IsPinned(int square)
{
    std::list<int>::iterator it;

    for(it=PinList.begin();it!=PinList.end();it++)
        if(*it == square)
            return true;
    
    return false;
}

bool VirtualMoveTable::IsPinned(int square, int &pinDir)
{
    std::list<int>::iterator it;
    std::list<int>::iterator pinDirIt = PinDirectionList.begin();

    for(it=PinList.begin();it!=PinList.end();it++)
    {
        if(*it == square)
        {
            pinDir = *pinDirIt;
            return true;
        }
        pinDirIt++;
    }
    
    return false;
}

bool VirtualMoveTable::IsAttacked(int square)
{
    std::list<int>::iterator it;

    for(it=AttackList.begin();it!=AttackList.end();it++)
        if(*it == square)
            return true;
    
    return false;
}

bool VirtualMoveTable::IsVirtuallyAttacked(int square)
{
    std::list<int>::iterator it;

    for(it=VirtualAttackList.begin();it!=VirtualAttackList.end();it++)
        if(*it == square)
            return true;
    
    return false;
}

bool VirtualMoveTable::IsCoveringCheck(int square)
{
    if(!IsChecked)
        return true; //whatever
    if(KnightCheckNum == 1)
    {
        if(square != CheckingKnightSquare)
            return false;
    }
    else if(KnightCheckNum > 1)
        return false;

    std::list<int>::iterator it;
    bool IsThisPathCovered = false;

    for(int i=0;i<CheckSquares.size();i++)
    {
        for(it = CheckSquares[i].begin();it!=CheckSquares[i].end();it++)
        {
            if(*it == square)
            {
                IsThisPathCovered = true;
                break;
            }
        }
        if(!IsThisPathCovered)
            return false;

        IsThisPathCovered = false;
    }

    return true;
}

void VirtualMoveTable::CheckForKnightChecks(int targetSquare, int knightSquare)
{
    if(Piece::IsEnemyKing(Board::squareState[targetSquare]))
    {
        //std::cout << "We have verified this potential knight adversary to be the king" << std::endl;
        IsChecked = true;
        KnightCheckNum++;
        CheckingKnightSquare = knightSquare;
    }
}
void VirtualMoveTable::CheckForPawnChecks(int targetSquare)
{
    if(Piece::IsEnemyKing(Board::squareState[targetSquare]))
    {
        IsChecked = true;
    }
}
bool VirtualMoveTable::IsDefended(int targetSquare)
{
    std::list<int>::iterator it;

    for(it=DefenseList.begin();it!=DefenseList.end();it++)
        if(*it == targetSquare)
            return true;
    
    return false;
}
bool VirtualMoveTable::IsCastling(int startSquare, int targetSquare, bool &IsKingside)
{
    //we are assuming we've checked for a king on the startSquare and checked for castling booleans in the move generation
    if(Board::activePlayer == 1)
    {
        if(startSquare != 4)
            return false;

        if(targetSquare == 6)
        {
            IsKingside = 1;
            return true;
        }
        else if(targetSquare == 2)
        {
            IsKingside = 0;
            return true;
        }
        return false;
    }
    else if(Board::activePlayer == 2)
    {
        if(startSquare != 60)
            return false;

        if(targetSquare == 62)
        {
            IsKingside = 1;
            return true;
        }
        else if(targetSquare == 58)
        {
            IsKingside = 0;
            return true;
        }
        return false;
    }
    return false;
}

void VirtualMoveTable::CheckState()
{
    if(CurrentMoveList.size() == 0)
    {
        if(IsChecked)
        {
            Board::DeclareWin((Board::activePlayer % 2) + 1);
        }
        else
            Board::DeclareDraw();
    }
}

void VirtualMoveTable::AddCurrentPosition()
{
    FEN currentFEN;
    currentFEN.GetPosition();

    //std::cout << "currentFEN: " << currentFEN.FENtext << std::endl;

    std::list<Position>::iterator it;
    for(it=occurredPositions.begin();it!=occurredPositions.end();it++)
    {
        //std::cout << "here" << std::endl;
        if(it->fen == currentFEN)
        {
            it->occurrenceNum++;
            //std::cout << "num: " << it->occurrenceNum << std::endl;
            if(it->occurrenceNum==5)
                Board::DeclareDraw();
            else if(it->occurrenceNum>=3)
                IsThreefoldRepetition = true;
            return;
        }
    }

    occurredPositions.push_back(Position(currentFEN,0));
}
void VirtualMoveTable::AddCurrentPosition(std::list<std::list<Position>::iterator> &iterList)
{
    FEN currentFEN;
    currentFEN.GetPosition();

    //std::cout << "currentFEN: " << currentFEN.FENtext << std::endl;

    std::list<Position>::iterator it;
    for(it=occurredPositions.begin();it!=occurredPositions.end();it++)
    {
        //std::cout << "here" << std::endl;
        if(it->fen == currentFEN)
        {
            iterList.push_back(it);
            it->occurrenceNum++;
            //std::cout << "num: " << it->occurrenceNum << std::endl;
            if(it->occurrenceNum==5)
                Board::DeclareDraw();
            else if(it->occurrenceNum>=3)
                IsThreefoldRepetition = true;
            return;
        }
    }

    occurredPositions.push_back(Position(currentFEN,0));
    iterList.push_back(std::prev(occurredPositions.end()));
}
void VirtualMoveTable::RemovePosition(std::list<Position>::iterator it)
{
    if(it->occurrenceNum>1)
        it->occurrenceNum--;
    else
        occurredPositions.erase(it);
}

bool VirtualMoveTable::IsSufficientMaterial()
{
    int whiteVal, blackVal;
    bool IsTherePawn;

    VirtualEvaluation::EvaluateSides(whiteVal, blackVal, IsTherePawn);

    //std::cout << "IsTherePawn? " << IsTherePawn << std::endl;

    if(!IsTherePawn)
    {

        if(whiteVal == 0 && blackVal == 0)
            return false;

        if(whiteVal == 0 && blackVal == Piece::knight) //or bishop
            return false;

        if(whiteVal == Piece::knight && blackVal == 0) //or bishop
            return false;

        return true;
    }

    //there is pawn

    /*for(int file=0;file<=7;file++) FOR NOW IT DOESN'T WORK AS INTENDED, BUT FUCK THIS SHIT
    {
        std::cout << "hey" << std::endl;
        std::cout << "IsFileBlocked? " << file <<": " << Board::IsFileBlocked(file) << std::endl; 
        if(!Board::IsFileBlocked(file)) //excluding king from the calculation
            return true; //this is probably only somewhat true, as this still might be a drawing position, but I guess it would soon enough resolve to a more clear situation, anyway
    }
    if(!Board::IsKingBlocked(Piece::white))
        return true;
    if(!Board::IsKingBlocked(Piece::black))
        return true;*/
    
    return false;
}