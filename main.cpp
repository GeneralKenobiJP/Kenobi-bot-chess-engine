#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "src/app/board.h"
#include "src/app/piece.h"
#include "src/app/test.h"
#include "src/app/SpriteHandler.h"
#include "src/app/fenUtility.h"

using namespace sf;

std::string thisDir = "desktop/github-projects/Kenobi-bot-chess-engine/";
const int FPS_RATE = 20; //set to 30 for release

const int NUM_PIECES = 6;

int main(){
    const int windowX=600,windowY=600;
    RenderWindow window(VideoMode(windowX,windowY),"the chess");
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

    Board::InitializeBoard(windowX,windowY);

    FEN::ReadPosition(FEN::startFEN);

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
                    else
                        Piece::spritePtr = nullptr;
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

                Board::selectedSquare = -1;
            }
        }

        if(Board::isMove)
        {
            Piece::MovePiece(mousePosition);
        }

        window.clear();
        window.draw(boardSprite);
        SpriteHandler::DrawPieces(SpriteHandler::pieceSprite,window);
        window.display();
    }
    return 0;
}