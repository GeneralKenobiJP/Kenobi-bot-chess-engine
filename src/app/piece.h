#pragma once
#include <../../include/SFML/Graphics/Sprite.hpp>

class Piece
{
    public:
    static const int none = 0;
    static const int king = 1;
    static const int pawn = 2;
    static const int knight = 3;
    static const int bishop = 4;
    static const int rook = 5;
    static const int queen = 6;

    static const int white = 8;
    static const int black = 16;

    static const int pieceMask = 0b11100;

    static sf::Sprite* spritePtr;

    static void ReadPiece(int thisNum);
    static void MovePiece(sf::Vector2i pos);
    static void PutPiece(int posX, int posY);
    static void PutPieceSprite(int posX, int posY, int spriteIndex);
    static void SetPiece(int pieceType, int pieceColor, int squareIndex);
};