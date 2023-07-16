#include "SpriteHandler.h"
#include "piece.h"

sf::IntRect SpriteHandler::pieceTextureRect[12];
sf::Sprite SpriteHandler::pieceSprite[16];
int SpriteHandler::pieceNum=0;

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

    for(int i=0;i<2;i++) ///////////Set to i<1 if sth goes wrong, but 2 is correct /////////////////
    {
        curX = startX;
        for(int j=0;j<6;j++)
        {
            Sprite[6*i+j].setTexture(texture);
            SpriteHandler::pieceTextureRect[6*i+j] = sf::IntRect(curX,curY,deltaX,deltaY);
            curX+=deltaX;
        }
        curY-=deltaY;
    }
}

void SpriteHandler::SelectPieceTexture(int spriteIndex, int targetPieceType, int targetPieceColor)
{
    int rectIndex=0;
    int targetPieceIndex = (targetPieceType | targetPieceColor) & Piece::pieceMask;
    targetPieceIndex /= 4;

    ///KING - QUEEN - BISHOP - KNIGHT - ROOK - PAWN (white - black)
    ///0    - 1     - 2     - 3     - 4     - 5     (+0 - +6)

    if(targetPieceIndex>5) //checks if black and simplifies code
    {
        rectIndex+=6;
        targetPieceIndex-=6;
    }

    switch(targetPieceIndex)
    {
        case 0: //king
            rectIndex = 0;
            break;
        case 1: //pawn
            rectIndex = 5;
            break;
        case 2: //knight
            rectIndex = 3;
            break;
        case 3: //bishop
            rectIndex = 2;
            break;
        case 4: //rook
            rectIndex = 4;
            break;
        case 5: //queen
            rectIndex = 1;
            break;
    }

    SpriteHandler::pieceSprite[spriteIndex].setTextureRect(SpriteHandler::pieceTextureRect[rectIndex]); 
}

void SpriteHandler::SetupBoard(sf::Sprite pieceSprite[],int width, int height)
{
    int curX=0;
    int curY=0;
    int deltaX=width/8;
    int deltaY=height/8;

    for(int i=0;i<16;i++)
    {
        pieceSprite[i].setPosition(curX,curY); //obsolete, but I'm gonna leave it for now
        pieceSprite[i].setScale(0.23,0.23);
        curX+=deltaX;
    }
}

void SpriteHandler::DrawPieces(sf::Sprite pieceSprite[], sf::RenderWindow &window)
{
    for(int i=0;i<SpriteHandler::pieceNum;i++)
    {
        window.draw(pieceSprite[i]);
    }
}