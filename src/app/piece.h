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

    static void ReadPiece(int thisNum); //log piece type and color from thisNum int
    static void MovePiece(sf::Vector2i pos); //moves piece sprite, with such position adjustments that looks correctly when following the mouse
    static void PutPiece(int posX, int posY); //moves piece sprite to the input position (piecePtr selects the sprite)
    static void PutPieceSprite(int posX, int posY, int spriteIndex); //moves target piece sprite to the input position
    static void SetPiece(int pieceType, int pieceColor, int squareIndex); //initializes a piece, with its texture rect
};