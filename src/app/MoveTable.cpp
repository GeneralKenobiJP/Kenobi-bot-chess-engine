#include "MoveTable.h"
#include <algorithm>
#include "board.h"
#include "piece.h"
#include <iostream>
#include "Evaluation.h"

//We start at N and go clockwise

const short MoveTable::knightShift[8] = {15,17,6,10,-15,-17,-6,-10};
const short MoveTable::directionShift[8] = {8,9,1,-7,-8,-9,-1,7};
short MoveTable::numSquaresToEdge[64][8];
std::vector<std::vector<short>> MoveTable::knightTargetSquares;
std::vector<std::vector<short>> MoveTable::kingTargetSquares;
std::vector<Move> MoveTable::CurrentMoveList;
std::vector<int> MoveTable::AttackList;
std::vector<int> MoveTable::PinList;
short MoveTable::enPassantSquare = -1;
bool MoveTable::W_CanCastleKingside;
bool MoveTable::B_CanCastleKingside;
bool MoveTable::W_CanCastleQueenside;
bool MoveTable::B_CanCastleQueenside;
std::vector<int> MoveTable::pawnAttackList;
std::vector<int> MoveTable::PinDirectionList;
std::vector<int> MoveTable::VirtualAttackList;
bool MoveTable::IsChecked = false;
short MoveTable::KnightCheckNum = 0;
std::vector<std::vector<int>> MoveTable::CheckSquares;
int MoveTable::CheckingKnightSquare = -1;
std::vector<int> MoveTable::DefenseList;
std::vector<int> MoveTable::kingVirtualAttackList;
int MoveTable::consecutiveMoves=0;
std::list<Position> MoveTable::occurredPositions;
bool MoveTable::IsThreefoldRepetition = false;
bool MoveTable::IsFiftymove = false;
std::vector<int> MoveTable::forbiddenEnPassantStartSquares;
std::vector<Move> MoveTable::OwnEnPassantPins;
bool MoveTable::IsEnPassantPinned;

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
    //MoveTable::AddCurrentPosition();
}

std::vector<Move> MoveTable::GenerateMoves()
{
    std::vector<Move> MoveList;
    int pieceType;
    int pieceColor;

    MoveTable::pawnAttackList.clear();
    MoveTable::kingVirtualAttackList.clear();

    if(MoveTable::consecutiveMoves >= 150)
        Board::DeclareDraw();
    else if(MoveTable::consecutiveMoves >= 100)
        MoveTable::IsFiftymove = true;

    if(!MoveTable::IsSufficientMaterial())
        Board::DeclareDraw();

    if(MoveTable::IsFiftymove || MoveTable::IsThreefoldRepetition)
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
std::vector<Move> MoveTable::GenerateMoves(int EnPassantPawnSquare)
{
    std::vector<Move> MoveList;
    int pieceType;
    int pieceColor;

    std::cout << "We generate moves post en passant" << std::endl;

    MoveTable::pawnAttackList.clear();
    MoveTable::kingVirtualAttackList.clear();

    if(MoveTable::consecutiveMoves >= 150)
        Board::DeclareDraw();
    else if(MoveTable::consecutiveMoves >= 100)
        MoveTable::IsFiftymove = true;

    if(!MoveTable::IsSufficientMaterial())
        Board::DeclareDraw();

    if(MoveTable::IsFiftymove || MoveTable::IsThreefoldRepetition)
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
            MoveTable::GenerateLongRangeMoves(i,pieceType,MoveList, EnPassantPawnSquare);
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

void MoveTable::GenerateMoves(std::vector<Move> &moveList)
{
    moveList = MoveTable::GenerateMoves();
}
void MoveTable::GenerateMoves(std::vector<Move> &moveList, int EnPassantPawnSquare)
{
    moveList = MoveTable::GenerateMoves(EnPassantPawnSquare);
}

void MoveTable::GenerateLongRangeMoves(int square, int pieceType, std::vector<Move> &moveList)
{
    int startDir = (pieceType == Piece::bishop) ? 1 : 0; //if true => 1
    int dirIncr = (pieceType == Piece::queen) ? 1 : 2; //if true => 1
    int targetSquare;
    int targetSquareState;
    int targetSquarePieceColor;
    bool IsPinned;
    int pinDir;

    IsPinned = MoveTable::IsPinned(square, pinDir);

    for(int dir=startDir;dir<8;dir+=dirIncr)
    {
        if(IsPinned)
                if(std::abs(pinDir) != std::abs(MoveTable::directionShift[dir]))
                    continue;
        
        for(int sq=1;sq<=MoveTable::numSquaresToEdge[square][dir];sq++)
        {
            targetSquare = square + MoveTable::directionShift[dir] * sq;

            targetSquareState = Board::squareState[targetSquare];
            Piece::ReadPieceColor(targetSquareState,targetSquarePieceColor);
            
            if(targetSquarePieceColor/8 == Board::activePlayer) //our piece is blocking the way
            {
                MoveTable::DefenseList.push_back(targetSquare);
                break;
            }

            if(!MoveTable::IsChecked || MoveTable::IsCoveringCheck(targetSquare))
            {
                moveList.push_back(Move(square,targetSquare));
            }

            std::cout << square << ' ' << targetSquare << std::endl;

            if(targetSquarePieceColor != 0) //opponent's piece is blocking the way
            {
                break;
            }

        }
    }
}
void MoveTable::GenerateLongRangeMoves(int square, int pieceType, std::vector<Move> &moveList, int EnPassantPawnSquare)
{
    int startDir = (pieceType == Piece::bishop) ? 1 : 0; //if true => 1
    int dirIncr = (pieceType == Piece::queen) ? 1 : 2; //if true => 1
    int targetSquare;
    int targetSquareState;
    int targetSquarePieceColor;
    bool IsPinned;
    int pinDir;

    std::cout << "Looks like we are post-en passant" << std::endl;

    IsPinned = MoveTable::IsPinned(square, pinDir);

    for(int dir=startDir;dir<8;dir+=dirIncr)
    {
        if(IsPinned)
                if(std::abs(pinDir) != std::abs(MoveTable::directionShift[dir]))
                    continue;
        
        for(int sq=1;sq<=MoveTable::numSquaresToEdge[square][dir];sq++)
        {
            targetSquare = square + MoveTable::directionShift[dir] * sq;

            if(targetSquare == EnPassantPawnSquare)
            {
                OwnEnPassantPins.push_back(Move(square, targetSquare));
            }

            targetSquareState = Board::squareState[targetSquare];
            Piece::ReadPieceColor(targetSquareState,targetSquarePieceColor);
            
            if(targetSquarePieceColor/8 == Board::activePlayer) //our piece is blocking the way
            {
                MoveTable::DefenseList.push_back(targetSquare);
                break;
            }

            if(!MoveTable::IsChecked || MoveTable::IsCoveringCheck(targetSquare))
            {
                moveList.push_back(Move(square,targetSquare));
            }

            std::cout << square << ' ' << targetSquare << std::endl;

            if(targetSquarePieceColor != 0) //opponent's piece is blocking the way
            {
                break;
            }

        }
    }
}

void MoveTable::GenerateKnightMoves(int square, std::vector<Move> &moveList)
{
    int targetSquareColor;

    if(MoveTable::IsPinned(square))
        return;

    for(int i=0;i<MoveTable::knightTargetSquares[square].size();i++)
    {
        if(MoveTable::IsChecked && !MoveTable::IsCoveringCheck(MoveTable::knightTargetSquares[square][i]))
            continue;

        Piece::ReadPieceColor(Board::squareState[MoveTable::knightTargetSquares[square][i]],targetSquareColor);

        if(targetSquareColor/8 != Board::activePlayer)
        {
            moveList.push_back(Move(square,knightTargetSquares[square][i]));
        }
        else
            MoveTable::DefenseList.push_back(knightTargetSquares[square][i]);
    }
}

void MoveTable::GeneratePawnMoves(int square, std::vector<Move> &moveList)
{
    int pawnMoveShift = (Board::activePlayer == 1) ? 8 : -8;
    int pawnAttackShift[2];
    int targetSquare;
    int targetSquareColor;
    bool initialPos = 0;
    bool IsPinned;
    int pinDir;

    IsPinned = MoveTable::IsPinned(square, pinDir);

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
        if(!MoveTable::IsChecked || MoveTable::IsCoveringCheck(targetSquare))
        {
            if(Board::squareState[targetSquare]==0 && targetSquare<64 && targetSquare >= 0)
            {
                if(targetSquare/8 != 7 && targetSquare/8 != 0)
                    moveList.push_back(Move(square,targetSquare));
                else //promotion (without capture)
                {
                    for(int i=1; i<5; i++)
                        moveList.push_back(Move(square,targetSquare,MoveTable::ReadPromotionPieceFromIndex(i,Board::activePlayer*8)));
                }
            }
        }
    }
    
    for(int i=0;i<2;i++)
    {
        if(IsPinned && std::abs(pinDir)!= std::abs(pawnAttackShift[i]))
            continue;

        targetSquare = square + pawnAttackShift[i];

        if(MoveTable::IsChecked && !MoveTable::IsCoveringCheck(targetSquare))
            continue;

        if(targetSquare<0 || targetSquare>63)
            break;
        if((targetSquare % 8 != square % 8 - 1) && (targetSquare % 8 != square % 8 + 1)) //preventing from going around the board
            continue;

        Piece::ReadPieceColor(Board::squareState[targetSquare],targetSquareColor);
        //EN PASSANT
        if(enPassantSquare == targetSquare && MoveTable::CanDoEnPassant(square))
        {
            moveList.push_back(Move(square,targetSquare));
            continue;
            //REMOVE EN PASSANT-ED PAWN
        }
        if(targetSquareColor != 0)
        {
            if(targetSquareColor/8 != Board::activePlayer)
            {
                if(targetSquare/8 != 7 && targetSquare/8 != 0)
                {
                    moveList.push_back(Move(square,targetSquare));
                }                    
                else //PROMOTION (with capture)
                {
                    for(int num = 1; num<5;num++)
                        moveList.push_back(Move(square,targetSquare,MoveTable::ReadPromotionPieceFromIndex(num,Board::activePlayer*8)));
                }
            }
            else
                MoveTable::DefenseList.push_back(targetSquare);
            
            continue;
        }

        MoveTable::pawnAttackList.push_back(targetSquare);
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
            if(MoveTable::IsChecked && !MoveTable::IsCoveringCheck(targetSquare))
                return;
            int passingSquare = square + pawnMoveShift;
            if(Board::squareState[targetSquare]==0 && Board::squareState[passingSquare] == 0)
                moveList.push_back(Move(square,targetSquare));
        }
    }

}

void MoveTable::GenerateKingMoves(int square, std::vector<Move> &moveList)
{
    int targetSquareColor;

    for(int i=0;i<MoveTable::kingTargetSquares[square].size();i++)
    {
        if(MoveTable::IsAttacked(MoveTable::kingTargetSquares[square][i]))
        {
            MoveTable::kingVirtualAttackList.push_back(kingTargetSquares[square][i]);
            continue;
        }

        if(MoveTable::IsVirtuallyAttacked(MoveTable::kingTargetSquares[square][i]))
        {
            MoveTable::kingVirtualAttackList.push_back(kingTargetSquares[square][i]);
            continue;
        }

        if(MoveTable::IsDefended(MoveTable::kingTargetSquares[square][i]))
            continue;

        Piece::ReadPieceColor(Board::squareState[MoveTable::kingTargetSquares[square][i]],targetSquareColor);

        if(targetSquareColor/8 != Board::activePlayer)
        {
            moveList.push_back(Move(square,kingTargetSquares[square][i]));
        }
        else if(targetSquareColor != 0)
            MoveTable::DefenseList.push_back(kingTargetSquares[square][i]);
    }

    if(MoveTable::IsChecked)
        return;

    if(Board::activePlayer == 1) //white
    {
        if(MoveTable::W_CanCastleKingside)
            if(Board::squareState[7]==13) //white rook
                if(Board::squareState[5]==0 && Board::squareState[6] == 0)
                    if(!MoveTable::IsAttacked(5) && !MoveTable::IsAttacked(6))
                        moveList.push_back(Move(square,6));

        if(MoveTable::W_CanCastleQueenside)
            if(Board::squareState[0]==13) //white rook
                if(Board::squareState[2]==0 && Board::squareState[3] == 0)
                    if(!MoveTable::IsAttacked(2) && !MoveTable::IsAttacked(3))
                        moveList.push_back(Move(square,2));
    }
    if(Board::activePlayer == 2) //black
    {
        if(MoveTable::B_CanCastleKingside)
            if(Board::squareState[63]==21) //black rook
                if(Board::squareState[61]==0 && Board::squareState[62] == 0)
                    if(!MoveTable::IsAttacked(61) && !MoveTable::IsAttacked(62))
                        moveList.push_back(Move(square,62));

        if(MoveTable::B_CanCastleQueenside)
            if(Board::squareState[56]==21) //black rook
                if(Board::squareState[58]==0 && Board::squareState[59] == 0)
                    if(!MoveTable::IsAttacked(58) && !MoveTable::IsAttacked(59))
                        moveList.push_back(Move(square,58));
    }
}

bool MoveTable::IsLegal(int startSquare, int targetSquare)
{
    std::vector<Move>::iterator it;

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
    MoveTable::DefenseList.clear();
    MoveTable::OwnEnPassantPins.clear();
    MoveTable::IsEnPassantPinned = false;

    std::cout << "En Passant situation" << MoveTable::enPassantSquare << std::endl;

    if(MoveTable::enPassantSquare != -1)
    {
        int EnPassantPawnSquare = (MoveTable::enPassantSquare / 8 == 2) ? (MoveTable::enPassantSquare + 8) : (MoveTable::enPassantSquare-8);
        MoveTable::GenerateMoves(MoveTable::CurrentMoveList, EnPassantPawnSquare);
    }
    else
        MoveTable::GenerateMoves(MoveTable::CurrentMoveList);

    MoveTable::IsChecked = false;
    MoveTable::KnightCheckNum = 0;
    MoveTable::CheckingKnightSquare = -1;

    MoveTable::AttackList.clear();
    MoveTable::PinList.clear();
    MoveTable::PinDirectionList.clear();
    MoveTable::CheckSquares.clear();
    MoveTable::VirtualAttackList.clear();
    MoveTable::forbiddenEnPassantStartSquares.clear();

    std::vector<Move>::iterator it;
    int startSquarePiece;
    int startSquareType;


    for(it=MoveTable::CurrentMoveList.begin();it!=MoveTable::CurrentMoveList.end();it++)
    {
        //std::cout << "Here, sir" << std::endl;

        if(Piece::ToType(Board::squareState[it->startSquare]) == Piece::pawn)
            if(it->startSquare % 8 == it->targetSquare % 8) //this is not an attack for pawn (detection of the push foward move)
            {
                std::cout << "Mere weak pawn move forward" << std::endl;
                continue;
            }
        
        MoveTable::AttackList.push_back(it->targetSquare);

        std::cout << "Pushed for attack. From: " << it->startSquare << " to: " << it->targetSquare << std::endl;

        startSquarePiece = Board::squareState[it->startSquare];

        Piece::ReadPieceType(startSquarePiece,startSquareType);

        if(Piece::IsLongRange(startSquareType))
            if(Board::squareState[it->targetSquare] != 0)
                CheckForPins(it->startSquare,it->targetSquare);

        if(startSquareType == Piece::knight)
            CheckForKnightChecks(it->targetSquare, it->startSquare);

        if(startSquareType == Piece::pawn)
            CheckForPawnChecks(it->targetSquare, it->startSquare);
    }
    for(it = MoveTable::OwnEnPassantPins.begin(); it!=MoveTable::OwnEnPassantPins.end();it++)
    {
        CheckForEnPassantPins(it->startSquare, it->targetSquare);
    }

    std::cout << "En Passant?: " << MoveTable::IsEnPassantPinned << std::endl;

    std::cout << "And for pawns" << std::endl;

    std::vector<int>::iterator iter;

    for(iter = MoveTable::pawnAttackList.begin(); iter!= MoveTable::pawnAttackList.end();iter++)
    {
        MoveTable::AttackList.push_back(*iter);
        std::cout << "Pushed pawn for attack:  "<< *iter << std::endl;
    }
    for(iter = MoveTable::kingVirtualAttackList.begin(); iter!= MoveTable::kingVirtualAttackList.end();iter++)
    {
        MoveTable::AttackList.push_back(*iter);
        //std::cout << "Pushed king for attack:  "<< *iter << std::endl;
    }

    std::sort(AttackList.begin(), AttackList.end());
    auto erasure1 = std::unique(AttackList.begin(), AttackList.end());
    AttackList.erase(erasure1,AttackList.end());

    std::sort(PinList.begin(), PinList.end());
    auto erasure2 = std::unique(PinList.begin(), PinList.end());
    PinList.erase(erasure2,PinList.end());

    std::sort(DefenseList.begin(), DefenseList.end());
    auto erasure3 = std::unique(DefenseList.begin(), DefenseList.end());
    DefenseList.erase(erasure3,DefenseList.end());

    std::cout << "Well: " << MoveTable::AttackList.size() << std::endl;
    std::cout << "Hath our monarch been checked? " << MoveTable::IsChecked << std::endl;
    std::cout << "The knight pale hath attacked in the count of " << MoveTable::KnightCheckNum << std::endl;
}

void MoveTable::CheckForPins(int startSquare, int targetSquare)
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

    int enPassantTargetSquare = (MoveTable::enPassantSquare == -1) ? -1 : ((Board::activePlayer == 1) ? MoveTable::enPassantSquare+8 : MoveTable::enPassantSquare-8);
    bool IsEvaluatingEnPassant = false;

    //THE FUNCTION IS CALLED JUST BEFORE SWITCHING PLAYERS
    for(int i=1;i<=MoveTable::numSquaresToEdge[targetSquare][dirIndex];i++)
    {
        pinTargetSquare = targetSquare + dir*i;
        
        if(Board::squareState[pinTargetSquare] != 0)
        {
            if(Piece::IsEnemyKing(Board::squareState[pinTargetSquare]))
            {
                if(!IsEvaluatingEnPassant)
                {
                    /*std::cout << "Hereby, there stands a king: " << pinTargetSquare << std::endl;
                    std::cout << "And the direction of the pin is: " << dir << std::endl;
                    std::cout << pinTargetSquare << " = " << targetSquare << " + " << dir << " * " << i << std::endl;
                    std::cout << "numSquaresToEdge: " << MoveTable::numSquaresToEdge[targetSquare][dir] << std::endl;
                    std::cout << "targetSquare: " << targetSquare << std::endl;*/
                    PinList.push_back(targetSquare);
                    PinDirectionList.push_back(dir);
                    std::cout << "Pinned dir: " << dir << std::endl;
                    break;
                }
                else
                {
                    forbiddenEnPassantStartSquares.push_back(targetSquare);
                    IsEvaluatingEnPassant = false;
                    break;
                }
            }
            else if(!IsEvaluatingEnPassant)
            {
                if(pinTargetSquare == enPassantTargetSquare)
                    IsEvaluatingEnPassant = true;
                else
                    break;
                
                //break;
            }
            else
            {
                IsEvaluatingEnPassant = false;
                break;
            }

        }

    }

    ///CHECKING FOR CHECKS AND VIRTUAL ATTACKS
    if(Piece::IsEnemyKing(Board::squareState[targetSquare]))
    {
        MoveTable::IsChecked = true;

        int thisSquare;
        std::vector<int> thisList;
        for(int i=targetSquare-dir;i!=startSquare;i-=dir)
        {
            thisList.push_back(i);
        }
        for(int i=1;i <= MoveTable::numSquaresToEdge[targetSquare][dirIndex];i++)
        {
            thisSquare = targetSquare + i*dir;
            if(Board::squareState[thisSquare] != 0)
                break;
            std::cout << "Virtual square: " << thisSquare << std::endl;
            MoveTable::VirtualAttackList.push_back(thisSquare);
        }
        thisList.push_back(startSquare);
        MoveTable::CheckSquares.push_back(thisList);
    }

}
void MoveTable::CheckForEnPassantPins(int startSquare, int targetSquare)
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

    int enPassantTargetSquare = (Board::activePlayer == 1) ? MoveTable::enPassantSquare+8 : MoveTable::enPassantSquare-8;

    std::cout << "En Passant target square: " << enPassantTargetSquare << std::endl;

    //THE FUNCTION IS CALLED JUST BEFORE SWITCHING PLAYERS
    for(int i=1;i<=MoveTable::numSquaresToEdge[targetSquare][dirIndex];i++)
    {
        pinTargetSquare = targetSquare + dir*i;
        int targetType, targetColor;
        
        if(Board::squareState[pinTargetSquare] != 0)
        {
            Piece::ReadPiece(Board::squareState[pinTargetSquare],targetType, targetColor);
            if(Piece::IsEnemyKing(Board::squareState[pinTargetSquare]))
            {
                MoveTable::IsEnPassantPinned = true;
            }
            else if((dir == 1 || dir == -1) && i == 1 && targetType == Piece::pawn && targetColor/8 != Board::activePlayer)
            {
                continue;
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
    std::vector<int>::iterator it;

    for(it=MoveTable::PinList.begin();it!=MoveTable::PinList.end();it++)
        if(*it == square)
            return true;
    
    return false;
}

bool MoveTable::IsPinned(int square, int &pinDir)
{
    std::vector<int>::iterator it;
    std::vector<int>::iterator pinDirIt = MoveTable::PinDirectionList.begin();

    for(it=MoveTable::PinList.begin();it!=MoveTable::PinList.end();it++)
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

bool MoveTable::IsAttacked(int square)
{
    std::vector<int>::iterator it;

    for(it=MoveTable::AttackList.begin();it!=MoveTable::AttackList.end();it++)
        if(*it == square)
            return true;
    
    return false;
}

bool MoveTable::IsVirtuallyAttacked(int square)
{
    std::vector<int>::iterator it;

    for(it=MoveTable::VirtualAttackList.begin();it!=MoveTable::VirtualAttackList.end();it++)
        if(*it == square)
            return true;
    
    return false;
}

bool MoveTable::IsCoveringCheck(int square)
{
    if(!MoveTable::IsChecked)
        return true; //whatever
    if(MoveTable::KnightCheckNum == 1)
    {
        if(square != MoveTable::CheckingKnightSquare)
            return false;
    }
    else if(MoveTable::KnightCheckNum > 1)
        return false;

    std::vector<int>::iterator it;
    bool IsThisPathCovered = false;

    for(int i=0;i<MoveTable::CheckSquares.size();i++)
    {
        for(it = MoveTable::CheckSquares[i].begin();it!=MoveTable::CheckSquares[i].end();it++)
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

void MoveTable::CheckForKnightChecks(int targetSquare, int knightSquare)
{
    if(Piece::IsEnemyKing(Board::squareState[targetSquare]))
    {
        //std::cout << "We have verified this potential knight adversary to be the king" << std::endl;
        MoveTable::IsChecked = true;
        MoveTable::KnightCheckNum++;
        MoveTable::CheckingKnightSquare = knightSquare;
    }
}
void MoveTable::CheckForPawnChecks(int targetSquare, int pawnSquare)
{
    std::cout << "I'm checkin' for this pesky pawns" << std::endl;
    if(Piece::IsEnemyKing(Board::squareState[targetSquare]))
    {
        std::cout << "Yup, a pawn check goddamnit" << std::endl;
        MoveTable::IsChecked = true;
        std::vector<int> thisList;
        thisList.push_back(pawnSquare);
        MoveTable::CheckSquares.push_back(thisList);
    }
}
bool MoveTable::IsDefended(int targetSquare)
{
    std::vector<int>::iterator it;

    for(it=MoveTable::DefenseList.begin();it!=MoveTable::DefenseList.end();it++)
        if(*it == targetSquare)
            return true;
    
    return false;
}
bool MoveTable::IsCastling(int startSquare, int targetSquare, bool &IsKingside)
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

void MoveTable::CheckState()
{
    if(MoveTable::CurrentMoveList.size() == 0)
    {
        if(MoveTable::IsChecked)
        {
            Board::DeclareWin((Board::activePlayer % 2) + 1);
        }
        else
            Board::DeclareDraw();
    }
}

void MoveTable::AddCurrentPosition()
{
    FEN currentFEN;
    currentFEN.GetPosition();

    //std::cout << "currentFEN: " << currentFEN.FENtext << std::endl;

    std::list<Position>::iterator it;
    for(it=MoveTable::occurredPositions.begin();it!=MoveTable::occurredPositions.end();it++)
    {
        std::cout << "here" << std::endl;
        if(it->fen == currentFEN)
        {
            it->occurrenceNum++;
            //std::cout << "num: " << it->occurrenceNum << std::endl;
            if(it->occurrenceNum==5)
                Board::DeclareDraw();
            else if(it->occurrenceNum>=3)
                MoveTable::IsThreefoldRepetition = true;
            return;
        }
    }

    MoveTable::occurredPositions.push_back(Position(currentFEN,1));
}
bool MoveTable::IsSufficientMaterial()
{
    int whiteVal, blackVal;
    bool IsTherePawn;

    Evaluation::EvaluateSides(whiteVal, blackVal, IsTherePawn);

    std::cout << "IsTherePawn? " << IsTherePawn << std::endl;

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
    
    //return false;

    return true;
}

void Move::LogMove()
{
    std::cout << "MOVE: " << this->startSquare << "->" << this->targetSquare << "(" << this->promotionPiece << ")" << std::endl;
}

bool MoveTable::CanDoEnPassant(int square)
{
    if(MoveTable::IsEnPassantPinned)
        return false;

    for(int i=0;i<MoveTable::forbiddenEnPassantStartSquares.size();i++)
        if(forbiddenEnPassantStartSquares[i]==square)
            return false;

    return true;
}

int MoveTable::ReadPromotionPieceFromIndex(int index, int color)
{
    switch(index)
    {
        case 1:
            return Piece::ComputePiece(Piece::queen, color);
        case 2:
            return Piece::ComputePiece(Piece::knight, color);
        case 3:
            return Piece::ComputePiece(Piece::rook, color);
        case 4:
            return Piece::ComputePiece(Piece::bishop, color);
        default:
            return Piece::ComputePiece(Piece::queen, color);
    }
}