#pragma once
#include <SFML/Graphics.hpp>

class SpriteHandler //this class does not handle boardSprite, which is handled by main.cpp instead
{
    private:
    static sf::IntRect pieceTextureRect[12];

    public:
    static sf::Sprite pieceSprite[16];
    static int pieceNum;

    static void LoadPieceSprites(sf::Sprite Sprite[], sf::Texture &texture);
    static void SelectPieceTexture(int spriteIndex, int targetPieceType, int targetPieceColor);
    static void SetupBoard(sf::Sprite pieceSprite[],int width, int height);
    static void DrawPieces(sf::Sprite pieceSprite[], sf::RenderWindow &window);
};