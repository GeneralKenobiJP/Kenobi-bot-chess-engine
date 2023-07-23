#pragma once
#include <../../include/SFML/Graphics/Sprite.hpp>
//#include <list>

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

    //static std::list<int> pieceList;

    static sf::Sprite* spritePtr;

    static void LogPiece(int thisNum); //log piece type and color from thisNum int
    static void ReadPiece(int thisNum, int &type, int &color); //read piece type and color from thisNum int, save into input variables
    static void ReadPieceType(int thisNum, int &type); //read piece type from thisNum int, save into input variables
    static void ReadPieceColor(int thisNum, int &color); //read piece color from thisNum int, save into input variables
    static int ToColor(int thisNum);
    static void MovePiece(sf::Vector2i pos); //moves piece sprite, with such position adjustments that looks correctly when following the mouse
    static void PutPiece(int posX, int posY); //moves piece sprite to the input position (piecePtr selects the sprite)
    static void PutPieceSprite(int posX, int posY, int spriteIndex); //moves target piece sprite to the input position
    static void SetPiece(int pieceType, int pieceColor, int squareIndex); //initializes a piece, with its texture rect
    static bool IsLongRange(int pieceType); //checks if a bishop or a rook or a queen
    static void RemovePieceSprite(int square); // receives a square from which to remove a piece sprite and deletes it
};