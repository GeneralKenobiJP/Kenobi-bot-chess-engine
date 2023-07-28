#include "SpriteHandler.h"
#include "piece.h"
#include "board.h"
#include <iostream>

sf::IntRect SpriteHandler::pieceTextureRect[12];
std::vector<sf::Sprite> SpriteHandler::pieceSprite;
int SpriteHandler::pieceNum=0;
const sf::Color SpriteHandler::moveDotColor = sf::Color(40,40,70,100);
const sf::Color SpriteHandler::promotionMenuColor = sf::Color(240,240,240,200);
std::vector<sf::CircleShape> SpriteHandler::dots;
sf::RectangleShape SpriteHandler::promotionMenu;
sf::Sprite SpriteHandler::promotionPieceSprite[8];
bool SpriteHandler::IsPromotion = false;
std::vector<sf::CircleShape> SpriteHandler::debugShapes;

void SpriteHandler::LoadPieceSprites(std::vector<sf::Sprite> &Sprite, sf::Texture &texture)
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

    for(int i=0;i<32;i++)
    {
        Sprite.push_back(sf::Sprite());
    }

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

    ///KING - QUEEN - BISHOP - KNIGHT - ROOK - PAWN (white - black)
    ///0    - 1     - 2     - 3     - 4     - 5     (+0 - +6)

    if(targetPieceIndex>15) //checks if black and simplifies code
    {
        rectIndex+=6;
        targetPieceIndex-=8;
    }
    targetPieceIndex -= 8;

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

    SpriteHandler::pieceSprite[spriteIndex].setTextureRect(SpriteHandler::pieceTextureRect[rectIndex]);
    //sf::IntRect checkRect = SpriteHandler::pieceSprite[spriteIndex].getTextureRect();
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

void SpriteHandler::DrawPieces(sf::RenderWindow &window)
{
    for(int i=0;i<SpriteHandler::pieceNum;i++)
    {
        window.draw(SpriteHandler::pieceSprite[i]);
    }
}
void SpriteHandler::DrawPromotionMenu(sf::RenderWindow &window)
{
    if(SpriteHandler::IsPromotion)
    {
        window.draw(SpriteHandler::promotionMenu);
        for(int i=0;i<4;i++)
        {
            window.draw(SpriteHandler::promotionPieceSprite[i+4*(Board::activePlayer-1)]);
        }
    }
}

void SpriteHandler::DrawDot(int square)
{
    sf::CircleShape thisDot;
    thisDot.setFillColor(SpriteHandler::moveDotColor);
    thisDot.setRadius(SpriteHandler::moveDotRadius);
    thisDot.setPosition(Board::squarePos[square].centerX-SpriteHandler::moveDotRadius,Board::squarePos[square].centerY-SpriteHandler::moveDotRadius);
    SpriteHandler::dots.push_back(thisDot);
}

void SpriteHandler::DrawMoveDots(int square, std::list<Move> moveList)
{
    //std::cout << "active square: " << square << std::endl;
    //std::list<Move> targetMoves;
    std::list<Move>::iterator it = moveList.begin();
    //std::cout << "cello there" << std::endl;
    //std::cout << moveList.size();
    for(it=moveList.begin();it!=moveList.end();it++)
    {
        //std::cout << "we're workin' on it, lads" << std::endl;
        //std::cout << it->startSquare << std::endl;
        if(it->startSquare == square)
        {
            //std::cout << it->startSquare << std::endl;
            SpriteHandler::DrawDot(it->targetSquare);
        }
    }
    //std::cout << "We're finished, mate" << std::endl;
}

void SpriteHandler::RemoveMoveDots()
{
    SpriteHandler::dots.clear();
    std::cout << "Removal team outta rescue" << std::endl;
}

void SpriteHandler::DrawDots(sf::RenderWindow &window)
{
    for(int i=0;i<SpriteHandler::dots.size();i++)
    {
        window.draw(dots[i]);
    }
}

void SpriteHandler::ShowPromotionMenu(int square)
{
    int thisSquare = (square/8 == 0) ? (square+32) : (square-8);
    std::cout << "this square: " << thisSquare << std::endl;
    int posX = Board::squarePos[thisSquare].x;
    int posY = Board::squarePos[thisSquare].y;
    int thisWidth = Board::squarePos[thisSquare].width;
    int thisHeight = Board::squarePos[thisSquare].height*4;

    promotionMenu.setPosition(posX,posY);
    promotionMenu.setSize(sf::Vector2f(thisWidth,thisHeight));
    promotionMenu.setFillColor(promotionMenuColor);

    int thisY = (square/8 == 0) ? (Board::squarePos[square+8].y) : posY;
    int thisX = posX;
    int deltaY = (square/8 == 0) ? (-1*Board::squarePos[thisSquare].height) : (Board::squarePos[thisSquare].height);

    int i = (square/8==0) ? 4 : 0;
    int n = i+4;
    for(i;i<n;i++)
    {
        SpriteHandler::promotionPieceSprite[i].setPosition(thisX, thisY);
        //SpriteHandler::promotionPieceSprite[i].

        thisY += deltaY;
    }
}

void SpriteHandler::InitializePromotionMenu()
{
    for(int i=0;i<8;i++)
    {
        SpriteHandler::promotionPieceSprite[i].setTexture(*SpriteHandler::pieceSprite[0].getTexture());
    }
    for(int i=0;i<2;i++)
    {
        int curIndex = 4*i;
        SpriteHandler::promotionPieceSprite[curIndex].setTextureRect(SpriteHandler::pieceTextureRect[1+6*i]);
        SpriteHandler::promotionPieceSprite[curIndex].setScale(0.23,0.23);
        curIndex++;
        SpriteHandler::promotionPieceSprite[curIndex].setTextureRect(SpriteHandler::pieceTextureRect[3+6*i]);
        SpriteHandler::promotionPieceSprite[curIndex].setScale(0.23,0.23);
        curIndex++;
        SpriteHandler::promotionPieceSprite[curIndex].setTextureRect(SpriteHandler::pieceTextureRect[4+6*i]);
        SpriteHandler::promotionPieceSprite[curIndex].setScale(0.23,0.23);
        curIndex++;
        SpriteHandler::promotionPieceSprite[curIndex].setTextureRect(SpriteHandler::pieceTextureRect[2+6*i]);
        SpriteHandler::promotionPieceSprite[curIndex].setScale(0.23,0.23);
    }
}

void SpriteHandler::HidePromotionMenu(int type, int color)
{
    std::cout << "So here we are, lads, we got the function that bears the name of HidePromotionMenu()" << std::endl;
    for(int i = 0; i<SpriteHandler::pieceNum;i++)
    {
        if(SpriteHandler::pieceSprite[i].getGlobalBounds().contains(Board::squarePos[Board::promotionSquare].centerX,Board::squarePos[Board::promotionSquare].centerY))
        {
            std::cout << "Hide 'em all" << std::endl;
            SpriteHandler::SelectPieceTexture(i,type,color);
            break;
        }
    }
    SpriteHandler::IsPromotion = false;
}

void SpriteHandler::DrawDebug(std::list<int> feature, sf::Color color, int offset)
{
    std::list<int>::iterator it;

    std::cout << "Le feature size: " << feature.size() << std::endl;

    for(it = feature.begin(); it != feature.end();it++)
    {
        std::cout << *it << std::endl;
        SpriteHandler::debugShapes.push_back(sf::CircleShape(10));
        SpriteHandler::debugShapes.back().setFillColor(color);
        SpriteHandler::debugShapes.back().setPosition(Board::squarePos[*it].x+offset,Board::squarePos[*it].y);
    }
}

void SpriteHandler::DrawDebugShapes(sf::RenderWindow &window)
{
    //std::cout << SpriteHandler::debugShapes.size() << std::endl;
    for(int i=0;i<SpriteHandler::debugShapes.size();i++)
    {
        window.draw(SpriteHandler::debugShapes[i]);
    }
}

void SpriteHandler::ClearDebug()
{
    SpriteHandler::debugShapes.clear();
}