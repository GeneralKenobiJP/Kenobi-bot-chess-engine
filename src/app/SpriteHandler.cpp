#include "SpriteHandler.h"

sf::Sprite SpriteHandler::pieceSprite[12];

void SpriteHandler::LoadPieceSprites(sf::Sprite Sprite[], sf::Texture &texture)
{
    int startX = 30;
    int startY = 0;
    int endXSub = 30;
    int endYSub = 0;
    int width = 2000;
    int height = 667;
    int curX;
    int curY = startY;
    int deltaX = (width-endXSub-startX)/6;
    int deltaY = (height-endYSub-startY)/2;

    ///KING - QUEEN - BISHOP - KNIGHT - ROOK - PAWN (white - black)
    ///0    - 1     - 2     - 3     - 4     - 5     (+0 - +6)

    for(int i=0;i<1;i++) ///////////CHANGE BACK TO i<2 /////////////////
    {
        curX = startX;
        for(int j=0;j<6;j++)
        {
            Sprite[6*i+j].setTexture(texture);
            Sprite[6*i+j].setTextureRect(sf::IntRect(curX,curY,deltaX,deltaY));
            curX+=deltaX;
        }
        curY-=deltaY;
    }
}

void SpriteHandler::SetupBoard(sf::Sprite pieceSprite[],int width, int height)
{
    int curX=0;
    int curY=0;
    int deltaX=width/8;
    int deltaY=height/8;

    for(int i=0;i<6;i++)
    {
        pieceSprite[i].setPosition(curX,curY);
        pieceSprite[i].setScale(0.23,0.23);
        curX+=deltaX;
    }
}

void SpriteHandler::DrawPieces(sf::Sprite pieceSprite[], sf::RenderWindow &window)
{
    for(int i=0;i<6;i++)
    {
        window.draw(pieceSprite[i]);
    }
}