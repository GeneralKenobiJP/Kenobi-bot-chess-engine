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
    //std::cout << "move" << std::endl;
    Piece::spritePtr->setPosition(pos.x-x,pos.y-y);
}
void Piece::PutPiece(int posX, int posY)
{
    //std::cout << "put" << std::endl;
    Piece::spritePtr->setPosition(posX,posY);
}
void Piece::PutPieceSprite(int posX, int posY, int spriteIndex)
{
    SpriteHandler::pieceSprite[spriteIndex].setPosition(posX,posY);
}
void Piece::SetPiece(int pieceType, int pieceColor, int squareIndex)
{
    //std::cout << "Square: " << Board::squarePos[squareIndex].x << std::endl;
    //std::cout << "Square: " << Board::squarePos[squareIndex].y << std::endl;
    SpriteHandler::SelectPieceTexture(SpriteHandler::pieceNum,pieceType,pieceColor);
    Board::PutOnSquare(squareIndex, pieceType, pieceColor);
    Piece::PutPieceSprite(Board::squarePos[squareIndex].x,Board::squarePos[squareIndex].y,SpriteHandler::pieceNum);
    //std::cout << "From " << SpriteHandler::pieceNum << std::flush;;
    SpriteHandler::pieceNum++;
    //std::cout << " to " << SpriteHandler::pieceNum << std::endl;
}