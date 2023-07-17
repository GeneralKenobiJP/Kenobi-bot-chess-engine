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

/*int Board::squareState[64];
coordinates Board::squarePos[64];
int Board::boardWidth;
int Board::boardHeight;*/

const int NUM_PIECES = 6;

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

    SpriteHandler::LoadPieceSprites(SpriteHandler::pieceSprite,pieceTexture);

    //SpriteHandler::SetupBoard(SpriteHandler::pieceSprite,windowX,windowY);

    /*
        PUT HERE DEBUGGING CODE
    */

    Board::InitializeBoard(windowX,windowY);
    //Board::PutOnSquare(7,Piece::rook,Piece::black);
    //Piece::ReadPiece(Board::squareState[7]);

    //Piece::SetPiece(Piece::queen, Piece::white, 0);
    //Piece::SetPiece(Piece::knight, Piece::black, 1);
    //Piece::SetPiece(Piece::pawn, Piece::black, 2);
    //Piece::ReadPiece(Board::squareState[0]);
    //Piece::ReadPiece(Board::squareState[1]);
    //Piece::ReadPiece(Board::squareState[2]);
    //Piece::ReadPiece(Board::squareState[57]);
    //Piece::ReadPiece(Board::squareState[58]);
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
                    Board::HandleMouseInput(mousePosition); //useless as for now
                    //std::cout << Board::selectedSquare << std::endl;
                    for(int i=0;i<NUM_PIECES;i++)
                    {
                        if(SpriteHandler::pieceSprite[i].getGlobalBounds().contains(mousePosition.x,mousePosition.y))
                        {
                            //std::cout << "true" << std::endl;
                            Piece::spritePtr = &SpriteHandler::pieceSprite[i];
                            break; //
                        }
                        Board::isMove=false;
                        //std::cout << "false" << std::endl;
                        if(i==NUM_PIECES-1)
                            isEmpty = true;
                    }
                    if(!isEmpty)
                        Board::isMove = true;
                }
            }

            if(event.type == Event::MouseButtonReleased)
            {
                //std::cout << "released" << std::endl;

                Board::isMove = false;

                Board::HandleMouseReleased(mousePosition);
            }
        }

        if(Board::isMove)
        {
            //std::cout << Piece::spritePtr << std::endl;
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
        SpriteHandler::DrawPieces(SpriteHandler::pieceSprite,window);
        //std::cout << "before display" << std::endl;
        window.display();
        //std::cout << "'ere we got" << std::endl;
    }
    return 0;
}