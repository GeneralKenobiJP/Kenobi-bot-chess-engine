#include "SpriteHandler.h"
#include "piece.h"
#include <iostream>

sf::IntRect SpriteHandler::pieceTextureRect[12];
sf::Sprite SpriteHandler::pieceSprite[32];
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
            //std::cout << "Here:" << SpriteHandler::pieceTextureRect[6*i+j].left << std::endl;
            //std::cout << SpriteHandler::pieceTextureRect[6*i+j].top << std::endl;
            //std::cout << SpriteHandler::pieceTextureRect[6*i+j].width << std::endl;
            //std::cout << SpriteHandler::pieceTextureRect[6*i+j].height << "stop" << std::endl;
            curX+=deltaX;
        }
        curY+=deltaY;
    }
    for(int i=12;i<32;i++)
    {
        Sprite[i].setTexture(texture);
    }
}

void SpriteHandler::SelectPieceTexture(int spriteIndex, int targetPieceType, int targetPieceColor)
{
    int rectIndex=0;
    int targetPieceIndex = targetPieceType | targetPieceColor;
    //std::cout << targetPieceIndex << std::endl;

    ///KING - QUEEN - BISHOP - KNIGHT - ROOK - PAWN (white - black)
    ///0    - 1     - 2     - 3     - 4     - 5     (+0 - +6)

    if(targetPieceIndex>15) //checks if black and simplifies code
    {
        rectIndex+=6;
        targetPieceIndex-=8;
    }
    targetPieceIndex -= 8;

    //std::cout << targetPieceIndex << std::endl; //same piece type as above, but now colorless

    switch(targetPieceIndex)
    {
        case 1: //king
            rectIndex += 0;
            break;
        case 2: //pawn
            rectIndex += 5;
            break;
        case 3: //knight
            rectIndex += 3;
            break;
        case 4: //bishop
            rectIndex += 2;
            break;
        case 5: //rook
            rectIndex += 4;
            break;
        case 6: //queen
            rectIndex += 1;
            break;
    }
    //std::cout << "rectIndex: " << rectIndex << std::endl;

    SpriteHandler::pieceSprite[spriteIndex].setTextureRect(SpriteHandler::pieceTextureRect[rectIndex]);
    sf::IntRect checkRect = SpriteHandler::pieceSprite[spriteIndex].getTextureRect();
    //std::cout << "Here:" << checkRect.left << std::endl;
    //std::cout << checkRect.top << std::endl;
    //std::cout << checkRect.width << std::endl;
    //std::cout << checkRect.height << "stop" << std::endl;
    SpriteHandler::pieceSprite[spriteIndex].setScale(0.23,0.23);
}

void SpriteHandler::SetupBoard(sf::Sprite pieceSprite[],int width, int height) /////////OBSOLETE FUNCTION (i think)/////////
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
    //std::cout << "pieceNum: " << SpriteHandler::pieceNum << std::endl;
    for(int i=0;i<SpriteHandler::pieceNum;i++)
    {
        window.draw(SpriteHandler::pieceSprite[i]);
        //std::cout << "drew: " << i << std::endl;
    }
    //std::cout << "loop looped itself" << std::endl;
}