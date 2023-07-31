#include "board.h"
#include "piece.h"
#include <cmath>
#include "MoveTable.h"
#include "SpriteHandler.h"
#include <iostream>
#include "Evaluation.h"
#include "clock.h"

int Board::squareState[64];
coordinates Board::squarePos[64];
int Board::boardWidth;
int Board::boardHeight;
short int Board::selectedSquare=-1;
bool Board::isMove=0;
unsigned short Board::activePlayer;
short Board::promotionSquare=-1;
int Board::CurrentEvalution=0;
bool Board::CanDeclareDraw=0;

void Board::InitializeBoard(int x, int y)
{
    int curX=0;
    int curY=y;
    int deltaX=x/8;
    int deltaY=y/8;
    int curIt;

    curY-=deltaY;

    Board::boardWidth=x;
    Board::boardHeight=y;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            curIt = 8 * i + j;
            Board::squareState[curIt] = 0;
            Board::squarePos[curIt].x = curX;
            Board::squarePos[curIt].y = curY;
            Board::squarePos[curIt].width = deltaX;
            Board::squarePos[curIt].height = deltaY;
            Board::squarePos[curIt].centerX = curX + deltaX/2;
            Board::squarePos[curIt].centerY = curY + deltaX/2;
            curX += deltaX;
        }
        curX = 0;
        curY -= deltaY;
    }
}

void Board::PutOnSquare(int num, int piece, int color)
{
    Board::squareState[num] = (piece | color);
    //Piece::pieceList.
}
void Board::PutOnSquare(int num, int pieceID)
{
    Board::squareState[num] = pieceID;
    //Piece::pieceList.
}

void Board::RemoveFromSquare(int num)
{
    Board::squareState[num] = Piece::none;
}

void Board::HandleMouseInput(sf::Vector2i position)
{
    int i=0;
    int iHolder;
    while(position.x>Board::squarePos[i].x && i<8)
    {
        i++;
    }
    i-=1;
    iHolder = i;
    while(position.y<Board::squarePos[i].y && i<=64-(8-iHolder))
    {
        i+=8;
    }
    //i-=8;
    Board::selectedSquare = i;
    //add highlighting
}

void Board::HandleMouseReleased(sf::Vector2i position)
{
    bool hasChanged = 1;
    bool hasCaptured = 0;
    int pieceType;
    int pieceColor;
    Piece::ReadPiece(Board::squareState[Board::selectedSquare],pieceType,pieceColor);

    SpriteHandler::RemoveMoveDots();

    if(pieceColor/8 != Board::activePlayer) //don't move that which is not thine, fool
    {
        //hasChanged = 0;
        std::cout << "don't move that which is not thine, fool" << std::endl;
        std::cout << Board::activePlayer << std::endl;
        std::cout << pieceColor << std::endl;
        Piece::PutPiece(Board::squarePos[Board::selectedSquare].x,Board::squarePos[Board::selectedSquare].y);
        return;
    }

    for(int i=0;i<64;i++)
    {
        if(position.x>=Board::squarePos[i].x && position.x<Board::squarePos[i].x+Board::squarePos[i].width)
            if(position.y>=Board::squarePos[i].y && position.y < Board::squarePos[i].y+Board::squarePos[i].height)
            {
                if(!MoveTable::IsLegal(Board::selectedSquare,i))
                {
                    i=Board::selectedSquare;
                }
                else
                {
                    if(pieceType == Piece::pawn)
                    {
                        if(MoveTable::IsEnPassant(i))
                        {
                            hasCaptured = true;
                            if(Board::activePlayer==1)
                            {
                                Piece::RemovePieceSprite(i-8);
                                Board::RemoveFromSquare(i-8);
                            }
                            else //black
                            {
                                Piece::RemovePieceSprite(i+8);
                                Board::RemoveFromSquare(i+8);
                            }
                            //break;
                        }
                        MoveTable::enPassantSquare = -1;
                        if(MoveTable::IsTwoSquareAdvance(Board::selectedSquare,i))
                        {
                            MoveTable::enPassantSquare = (i+Board::selectedSquare)/2;
                        }
                        if(i/8==7 || i/8==0) //PROMOTION
                        {
                            if(Board::squareState[i] !=0 )
                            {
                                if(Piece::ToColor(Board::squareState[i]) == pieceColor) //you can't take your own pieces, dummy
                                {
                                    i=64;
                                    hasChanged = 0;
                                    //std::cout << "Friendly fire" << std::endl;
                                }
                                else //enemy piece
                                {
                                    hasCaptured = true;
                                    Piece::RemovePieceSprite(i);
                                    Board::RemoveFromSquare(i);
                                    Board::Promote(i, pieceColor);
                                    Piece::PutPiece(Board::squarePos[i].x,Board::squarePos[i].y);
                                    //Board::SwitchPlayer();
                                    break;
                                }
                                
                            }
                            else
                            {
                                Board::Promote(i, pieceColor);
                                Piece::PutPiece(Board::squarePos[i].x,Board::squarePos[i].y);
                                //Board::SwitchPlayer();
                                break;
                            }
                        }
                    }
                    else
                        MoveTable::enPassantSquare = -1;

                    if(pieceType == Piece::king)
                    {
                        bool IsKingside;

                        if(pieceColor == Piece::white)
                        {
                            MoveTable::W_CanCastleKingside = 0;
                            MoveTable::W_CanCastleQueenside = 0;
                        }
                        else
                        {
                            MoveTable::B_CanCastleKingside = 0;
                            MoveTable::B_CanCastleQueenside = 0;
                        }
                    
                        if(MoveTable::IsCastling(Board::selectedSquare,i,IsKingside))
                        {
                            if(IsKingside)
                            {
                                Piece::RemovePieceSprite(i+1);
                                Board::RemoveFromSquare(i+1);
                                Piece::SetPiece(Piece::rook,Piece::ToColor(Board::squareState[Board::selectedSquare]),i-1);
                            }
                            else
                            {
                                Piece::RemovePieceSprite(i-2);
                                Board::RemoveFromSquare(i-2);
                                Piece::SetPiece(Piece::rook,Piece::ToColor(Board::squareState[Board::selectedSquare]),i+1);
                            }
                        }
                    }
                    
                    if(pieceType == Piece::rook)
                    {
                        if(Board::selectedSquare == 0) //a1
                            MoveTable::W_CanCastleKingside = 0;
                        else if(Board::selectedSquare == 7) //h1
                            MoveTable::W_CanCastleQueenside = 0;
                        else if(Board::selectedSquare == 56) //a8
                            MoveTable::B_CanCastleKingside = 0;
                        else if(Board::selectedSquare == 63) //h8
                            MoveTable::B_CanCastleQueenside = 0;
                    }
                }
                if(Board::squareState[i] !=0 )
                {
                    if(Piece::ToColor(Board::squareState[i]) == pieceColor) //you can't take your own pieces, dummy
                    {
                        i=64;
                        hasChanged = 0;
                        //std::cout << "Friendly fire" << std::endl;
                    }
                    else //enemy piece
                    {
                        Piece::RemovePieceSprite(i);
                        Board::RemoveFromSquare(i);
                        hasCaptured = true;
                    }
                    
                }

                if(i<=63)
                {
                    Piece::PutPiece(Board::squarePos[i].x,Board::squarePos[i].y);
                    if(i == Board::selectedSquare) //stop proceeding if a piece was put back on the same square
                        return;
                    Board::PutOnSquare(i,Board::squareState[Board::selectedSquare]);
                    if(hasChanged)
                        Board::RemoveFromSquare(Board::selectedSquare);
                    Board::SwitchPlayer();
                    break;
                }
            }
        if(i>=63)
        {
            Piece::PutPiece(Board::squarePos[Board::selectedSquare].x,Board::squarePos[Board::selectedSquare].y); //foolproof code ensuring piece ejection into a sub-orbital flight is impossible
            hasChanged = 0;
        }
    }
    if(hasChanged)
    {
        if(pieceType != Piece::pawn && !hasCaptured)
            MoveTable::consecutiveMoves++;
        else
            MoveTable::consecutiveMoves=0;

        Board::RemoveFromSquare(Board::selectedSquare);
        MoveTable::GenerateMoves(MoveTable::CurrentMoveList);
        MoveTable::CheckState();
    }
}

void Board::DisableSelection()
{
    //Board::selectedSquare = -1;
    Piece::spritePtr = nullptr;
    SpriteHandler::RemoveMoveDots();
}

void Board::ReadSquare(int squareIndex, int &file, int &rank)
{
    file = squareIndex%8;
    rank = squareIndex/8;
}

int Board::CalculateDistance(int squareA, int squareB)
{
    int fileA;
    int fileB;
    int rankA;
    int rankB;

    Board::ReadSquare(squareA,fileA,rankA);
    Board::ReadSquare(squareB,fileB,rankB);

    int dist = std::abs(fileA-fileB) + std::abs(rankA-rankB);

    return dist;
}

void Board::SwitchPlayer()
{
    MoveTable::GenerateAttacks();
    Board::activePlayer = (Board::activePlayer % 2) + 1;
    Board::CurrentEvalution = Evaluation::Evaluate();
    SpriteHandler::DisplayCurrentEvaluation();
    ChessClock::SetActivePlayer(Board::activePlayer);

    //SpriteHandler::ClearDebug();
    //SpriteHandler::DrawDebug(MoveTable::AttackList, sf::Color::Magenta,0);
    //SpriteHandler::DrawDebug(MoveTable::PinList, sf::Color::Cyan,31);
    //std::cout << MoveTable::VirtualAttackList.size() << std::endl;
    //SpriteHandler::DrawDebug(MoveTable::VirtualAttackList, sf::Color::Green,0);
    //SpriteHandler::DrawDebug(MoveTable::DefenseList, sf::Color::Yellow,31);
    //for(int i=0;i<MoveTable::CheckSquares.size();i++)
        //SpriteHandler::DrawDebug(MoveTable::CheckSquares[i], sf::Color::Black, 0);

    std::cout << "switcheroo, now: " << Board::activePlayer << std::endl;
}

void Board::Promote(int square, int color)
{
    SpriteHandler::IsPromotion = true;
    Board::promotionSquare = square;
    SpriteHandler::ShowPromotionMenu(square);
}

void Board::HandlePromotion(int promotionSpriteIndex)
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

    Board::PutOnSquare(Board::promotionSquare, type, color);
    SpriteHandler::HidePromotionMenu(type, color);
    Board::promotionSquare = -1;
    Board::selectedSquare = -1;
    Board::SwitchPlayer();
    MoveTable::GenerateMoves(MoveTable::CurrentMoveList);
    MoveTable::CheckState();
}

void Board::DeclareWin(int victoriousPlayer)
{
    std::cout << "Win" << std::endl;
    std::cout << "Won player " << victoriousPlayer << std::endl;
    Board::activePlayer = 0;
}
void Board::DeclareDraw()
{
    std::cout << "Draw" << std::endl;
    Board::activePlayer = 0;
}