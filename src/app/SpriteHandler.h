#pragma once
#include <SFML/Graphics.hpp>
#include "MoveTable.h"

class SpriteHandler //this class does not handle boardSprite, which is handled by main.cpp instead
{
    private:
    static const int moveDotRadius = 15;
    static const sf::Color moveDotColor;

    public:
    static sf::IntRect pieceTextureRect[12];
    static sf::Sprite pieceSprite[32];
    static int pieceNum;

    static void LoadPieceSprites(sf::Sprite Sprite[], sf::Texture &texture); //here we set pieceTextureRect and texture file for pieces
    static void SelectPieceTexture(int spriteIndex, int targetPieceType, int targetPieceColor); //selects texture rect according to the piece
    static void SetupBoard(sf::Sprite pieceSprite[],int width, int height); //obsolete
    static void DrawPieces(sf::Sprite pieceSprite[], sf::RenderWindow &window); //draws each piece
    static void DrawMoveDots(int square, std::list<Move> moveList); //draws dots at the center of each square, where a move can be made (after piece selection)
};