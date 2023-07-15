#pragma once
#include <SFML/Graphics.hpp>

class SpriteHandler //this class does not handle boardSprite, which is handled by main.cpp instead
{

    public:
    static sf::Sprite pieceSprite[12];

    static void LoadPieceSprites(sf::Sprite Sprite[], sf::Texture &texture);
    static void SetupBoard(sf::Sprite pieceSprite[],int width, int height);
    static void DrawPieces(sf::Sprite pieceSprite[], sf::RenderWindow &window);
};