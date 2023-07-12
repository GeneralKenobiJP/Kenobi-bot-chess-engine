#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "src/app/board.h"
#include "src/app/piece.h"
#include "src/app/test.h"
//#include "src/app/singlefiletest.cpp"

using namespace sf;

std::string thisDir = "desktop/github-projects/Kenobi-bot-chess-engine/";

/*int Board::squareState[64];
coordinates Board::squarePos[64];
int Board::boardWidth;
int Board::boardHeight;*/

void LoadPieceSprites(Sprite Sprite[], Texture &texture)
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

    for(int i=0;i<2;i++)
    {
        curX = startX;
        for(int j=0;j<6;j++)
        {
            Sprite[6*i+j].setTexture(texture);
            Sprite[6*i+j].setTextureRect(IntRect(curX,curY,deltaX,deltaY));
            curX+=deltaX;
        }
        curY+=deltaY;
    }
}

void SetupBoard(Sprite pieceSprite[],int width, int height)
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

void DrawPieces(Sprite pieceSprite[], RenderWindow &window)
{
    for(int i=0;i<6;i++)
    {
        window.draw(pieceSprite[i]);
    }
}

int main(){
    const int windowX=600,windowY=600;
    RenderWindow window(VideoMode(windowX,windowY),"the chess");

    Texture boardTexture;
    Texture pieceTexture;
    boardTexture.loadFromFile(thisDir+"img/chessBoard.png",IntRect(0,0,windowX,windowY));
    pieceTexture.loadFromFile(thisDir+"img/pieces.png");
    boardTexture.setSmooth(true);
    pieceTexture.setSmooth(true);

    Sprite boardSprite(boardTexture);
    Sprite pieceSprite[12];

    LoadPieceSprites(pieceSprite,pieceTexture);

    SetupBoard(pieceSprite,windowX,windowY);

    /*
        PUT HERE DEBUGGING CODE
    */

    

    //Board board;
    //Piece piece;

    //SingleFileTest sft;
    //sft.SetX(3);

    
    Test test;
    test.x=2;
    test.SetX(3);
    
    //Board::squareState[9] = 2;

    //std::cout << Board::squareState[9] << std::endl;

    //board.squareState[9] = 2;
    
    //std::cout << board.squareState[9] << std::endl;

    //Board::squareState = new int[64];

    Board::InitializeBoard(windowX,windowY);
    Board::PutOnSquare(7,Piece::rook,Piece::black);
    Piece::ReadPiece(9);

    ///

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(boardSprite);
        //window.draw(pieceSprite[1]);
        DrawPieces(pieceSprite,window);
        window.display();
    }
    return 0;
}