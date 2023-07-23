#include "piece.h"
#include "board.h"
#include "SpriteHandler.h"
#include <iostream>

sf::Sprite* Piece::spritePtr = nullptr;

void Piece::LogPiece(int thisNum)
{
    int thisType = (thisNum << 2);
    thisType = thisType & Piece::pieceMask;
    thisType /= 4;
    int thisColor = thisNum - thisType;
    std::cout << thisType << std::endl;
    std::cout << thisColor << std::endl;
}
void Piece::ReadPiece(int thisNum, int &type, int &color)
{
    type = (thisNum << 2);
    type = type & Piece::pieceMask;
    type /= 4;
    color = thisNum - type;
}
void Piece::ReadPieceType(int thisNum, int &type)
{
    type = (thisNum << 2);
    type = type & Piece::pieceMask;
    type /= 4;
}
void Piece::ReadPieceColor(int thisNum, int &color)
{
    color = (thisNum >> 3);
    color *= 8;
}
int Piece::ToColor(int thisNum)
{
    int color = (thisNum >> 3);
    color *= 8;
    return color;
}
void Piece::MovePiece(sf::Vector2i pos)
{
    if(Board::selectedSquare==-1 || Piece::spritePtr == nullptr)
        return;
    int x = Board::squarePos[0].width / 2;
    int y = Board::squarePos[0].height / 2;
    Piece::spritePtr->setPosition(pos.x-x,pos.y-y);
}
void Piece::PutPiece(int posX, int posY)
{
    Piece::spritePtr->setPosition(posX,posY);
}
void Piece::PutPieceSprite(int posX, int posY, int spriteIndex)
{
    SpriteHandler::pieceSprite[spriteIndex].setPosition(posX,posY);
}
void Piece::SetPiece(int pieceType, int pieceColor, int squareIndex)
{
    SpriteHandler::SelectPieceTexture(SpriteHandler::pieceNum,pieceType,pieceColor);
    Board::PutOnSquare(squareIndex, pieceType, pieceColor);
    Piece::PutPieceSprite(Board::squarePos[squareIndex].x,Board::squarePos[squareIndex].y,SpriteHandler::pieceNum);
    SpriteHandler::pieceNum++;
}
bool Piece::IsLongRange(int pieceType)
{
    if(pieceType == Piece::bishop || pieceType == Piece::rook || pieceType == Piece::queen)
        return 1;
    else
        return 0;
}
void Piece::RemovePieceSprite(int square)
{
    bool IsFound = 0;

    for(int i = 0; i < SpriteHandler::pieceNum; i++)
    {
        std::cout << i << ": " << std::flush;
        if(IsFound)
        {
            std::cout << "Undergoing copying" << std::endl;
            SpriteHandler::pieceSprite[i-1] = SpriteHandler::pieceSprite[i];
            if(Piece::spritePtr == &SpriteHandler::pieceSprite[i])
                spritePtr--;
            //SpriteHandler::pieceSprite[i].setScale(0,0);
        }
        else
            if(SpriteHandler::pieceSprite[i].getPosition().x == Board::squarePos[square].x)
                if(SpriteHandler::pieceSprite[i].getPosition().y == Board::squarePos[square].y)
                {
                    std::cout << "Is Found" << std::endl;
                    IsFound = 1;
                }
    }
    std::cout << "'Ere we are" << std::endl;
    SpriteHandler::pieceNum--;
    SpriteHandler::pieceSprite.pop_back();
    std::cout << "Finito" << std::endl;
}