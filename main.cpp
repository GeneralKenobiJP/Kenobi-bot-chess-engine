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

const int NUM_PIECES = 6;

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

    for(int i=0;i<1;i++) ///////////CHANGE BACK TO i<2 /////////////////
    {
        curX = startX;
        for(int j=0;j<6;j++)
        {
            Sprite[6*i+j].setTexture(texture);
            Sprite[6*i+j].setTextureRect(IntRect(curX,curY,deltaX,deltaY));
            curX+=deltaX;
        }
        curY-=deltaY;
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
    for(int i=0;i<NUM_PIECES;i++)
    {
        window.draw(pieceSprite[i]);
    }
}

int main(){
    const int windowX=600,windowY=600;
    RenderWindow window(VideoMode(windowX,windowY),"the chess");
    window.setFramerateLimit(30);

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

    
    //Test test;
    //test.x=2;
    //test.SetX(3);
    
    //Board::squareState[9] = 2;

    //std::cout << Board::squareState[9] << std::endl;

    //board.squareState[9] = 2;
    
    //std::cout << board.squareState[9] << std::endl;

    //Board::squareState = new int[64];

    Board::InitializeBoard(windowX,windowY);
    Board::PutOnSquare(7,Piece::rook,Piece::black);
    Piece::ReadPiece(Board::squareState[7]);

    ///

    while (window.isOpen())
    {
        Event event;

        Vector2i mousePosition = Mouse::getPosition(window);
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == Event::Closed)
                window.close();

            if(event.type == Event::MouseButtonPressed)
            {
                std::cout << "down" << std::endl;

                if(event.mouseButton.button == Mouse::Left)
                {
                    bool isEmpty = false;
                    Board::HandleMouseInput(mousePosition); //useless as for now
                    std::cout << Board::selectedSquare << std::endl;
                    for(int i=0;i<NUM_PIECES;i++)
                    {
                        if(pieceSprite[i].getGlobalBounds().contains(mousePosition.x,mousePosition.y))
                        {
                            std::cout << "true" << std::endl;
                            Piece::spritePtr = &pieceSprite[i];
                            break; //
                        }
                        Board::isMove=false;
                        std::cout << "false" << std::endl;
                        if(i==NUM_PIECES-1)
                            isEmpty = true;
                    }
                    if(!isEmpty)
                        Board::isMove = true;
                }
            }

            if(event.type == Event::MouseButtonReleased)
            {
                std::cout << "released" << std::endl;

                Board::isMove = false;

                Board::HandleMouseReleased(mousePosition);
            }
        }

        if(Board::isMove)
        {
            std::cout << Piece::spritePtr << std::endl;
            //std::cout << Piece::spritePtr->getGlobalBounds().left << std::endl;
            Piece::MovePiece(mousePosition);
        }

        /*if(Mouse::isButtonPressed(Mouse::Left))
        {
            Board::CheckMousePositionOnPressed(Mouse::getPosition(window));
        }*/

        window.clear();
        window.draw(boardSprite);
        //window.draw(pieceSprite[1]);
        DrawPieces(pieceSprite,window);
        window.display();
    }
    return 0;
}