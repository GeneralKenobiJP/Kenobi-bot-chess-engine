#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "src/app/board.h"
#include "src/app/piece.h"
#include "src/app/test.h"
#include "src/app/SpriteHandler.h"
#include "src/app/fenUtility.h"
#include "src/app/clock.h"
#include "src/app/MoveTable.h"
#include <thread>

using namespace sf;

std::string thisDir = "desktop/github-projects/Kenobi-bot-chess-engine/";
const int FPS_RATE = 30; //set to 30 or 60 for release

//const int NUM_PIECES = 6;

int main(){
    const int windowX=600,windowY=600;
    const int sideBarX=160;
    RenderWindow window(VideoMode(windowX+sideBarX,windowY),"the chess");
    window.setFramerateLimit(FPS_RATE);

    Texture boardTexture;
    Texture pieceTexture;
    boardTexture.loadFromFile(thisDir+"img/chessBoard.png",IntRect(0,0,windowX,windowY));
    pieceTexture.loadFromFile(thisDir+"img/pieces.png");
    boardTexture.setSmooth(true);
    pieceTexture.setSmooth(true);

    Sprite boardSprite(boardTexture);

    SpriteHandler::LoadPieceSprites(SpriteHandler::pieceSprite,pieceTexture); //we are loading textures

    /*
        PUT HERE DEBUGGING CODE
    */

    ChessClock whiteClock;
    ChessClock blackClock;

    Board::InitializeBoard(windowX,windowY);
    MoveTable::CalculateStartMoveData();

    whiteClock.InitializeClock(650,400,thisDir);
    blackClock.InitializeClock(650,200,thisDir);
    whiteClock.SetTime(600);
    blackClock.SetTime(600);

    FEN::ReadPosition(FEN::startFEN);
    ChessClock::SetActivePlayer(1,whiteClock);

    /// DEBUGGING CODE ENDS HERE

    while (window.isOpen())
    {
        //std::cout << "'ere we're" << std::endl;
        Event event;

        Vector2i mousePosition = Mouse::getPosition(window);
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == Event::Closed)
                window.close();

            if(event.type == Event::MouseButtonPressed)
            {
                //std::cout << "down" << std::endl;

                if(event.mouseButton.button == Mouse::Left)
                {
                    bool isEmpty = false;
                    Board::HandleMouseInput(mousePosition);
                    //std::cout << Board::selectedSquare << std::endl;
                    for(int i=0;i<SpriteHandler::pieceNum;i++)
                    {
                        if(SpriteHandler::pieceSprite[i].getGlobalBounds().contains(mousePosition.x,mousePosition.y))
                        {
                            //std::cout << "true" << std::endl;
                            Piece::spritePtr = &SpriteHandler::pieceSprite[i];
                            break; //
                        }
                        Board::isMove=false;
                        //std::cout << "false" << std::endl;
                        if(i==SpriteHandler::pieceNum-1)
                            isEmpty = true;
                    }
                    if(!isEmpty)
                        Board::isMove = true;
                    //else
                        //Piece::spritePtr = nullptr;
                }
                if(event.mouseButton.button == Mouse::Right)
                {
                    Board::DisableSelection();
                }
            }
            if(event.type == Event::KeyPressed)
            {
                if(event.key.code == Keyboard::Escape)
                {
                    std::cout << "disabled" << std::endl;
                    Board::DisableSelection();
                }
            }

            if(event.type == Event::MouseButtonReleased && Board::isMove)
            {
                //std::cout << "released" << std::endl;

                Board::isMove = false;

                Board::HandleMouseReleased(mousePosition);

                //Board::selectedSquare = -1;
            }
        }

        if(Board::isMove)
        {
            Piece::MovePiece(mousePosition);
        }

        //std::thread clockThread(ChessClock::CountDown,whiteClock,blackClock);
        ChessClock::CountDown(whiteClock,blackClock);

        window.clear();
        window.draw(boardSprite);
        window.draw(whiteClock.clockText);
        window.draw(blackClock.clockText);
        SpriteHandler::DrawPieces(SpriteHandler::pieceSprite,window);
        window.display();
    }
    return 0;
}