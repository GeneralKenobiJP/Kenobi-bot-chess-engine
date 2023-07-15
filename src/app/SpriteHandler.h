#pragma once
#include <../../include/SFML/Graphics/Sprite.hpp>
#include <../../include/SFML/Graphics/Texture.hpp>

class SpriteHandler //this class does not handle boardSprite, which is handled by main.cpp instead
{

    public:
    //sf::Sprite pieceSprite[12];

    static void LoadPieceSprites(sf::Sprite Sprite[], sf::Texture &texture);
};