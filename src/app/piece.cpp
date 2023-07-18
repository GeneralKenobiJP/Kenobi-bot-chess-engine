#include "piece.h"
#include "board.h"
#include "SpriteHandler.h"
#include <iostream>

sf::Sprite* Piece::spritePtr = nullptr;

void Piece::ReadPiece(int thisNum)
{
    int thisType = (thisNum << 2);
    thisType = thisType & Piece::pieceMask;
    thisType /= 4;
    int thisColor = thisNum - thisType;
    std::cout << thisType << std::endl;
    std::cout << thisColor << std::endl;
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