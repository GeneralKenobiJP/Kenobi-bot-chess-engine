#pragma once
#include <SFML/Graphics.hpp>

class SpriteHandler //this class does not handle boardSprite, which is handled by main.cpp instead
{
    private:

    public:
    static sf::IntRect pieceTextureRect[12];
    static sf::Sprite pieceSprite[32];
    static int pieceNum;

    static void LoadPieceSprites(sf::Sprite Sprite[], sf::Texture &texture); //here we set pieceTextureRect and texture file for pieces
    static void SelectPieceTexture(int spriteIndex, int targetPieceType, int targetPieceColor); //selects texture rect according to the piece
    static void SetupBoard(sf::Sprite pieceSprite[],int width, int height); //obsolete
    static void DrawPieces(sf::Sprite pieceSprite[], sf::RenderWindow &window); //draws each piece
};