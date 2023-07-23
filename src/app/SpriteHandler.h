#pragma once
#include <SFML/Graphics.hpp>
#include "MoveTable.h"
#include <vector>

class SpriteHandler //this class does not handle boardSprite, which is handled by main.cpp instead
{
    private:
    static const int moveDotRadius = 15;
    static const sf::Color moveDotColor;

    public:
    static sf::IntRect pieceTextureRect[12];
    static std::vector<sf::Sprite> pieceSprite;
    static std::vector<sf::CircleShape> dots;
    static int pieceNum;

    static void LoadPieceSprites(std::vector<sf::Sprite> &Sprite, sf::Texture &texture); //here we set pieceTextureRect and texture file for pieces
    static void SelectPieceTexture(int spriteIndex, int targetPieceType, int targetPieceColor); //selects texture rect according to the piece
    static void SetupBoard(sf::Sprite pieceSprite[],int width, int height); //obsolete
    static void DrawPieces(sf::RenderWindow &window); //draws each piece
    static void DrawDot(int square);
    static void DrawMoveDots(int square, std::list<Move> moveList); //draws dots at the center of each square, where a move can be made (after piece selection)
    static void RemoveMoveDots();
    static void DrawDots(sf::RenderWindow &window);
};