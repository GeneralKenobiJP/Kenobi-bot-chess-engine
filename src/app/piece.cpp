#include "piece.h"
#include "board.h"
#include <iostream>

sf::Sprite* Piece::spritePtr;

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
void Piece::SetPiece(int pieceType, int pieceColor, int squareIndex)
{
    
    Board::PutOnSquare(squareIndex, pieceType, pieceColor);
}