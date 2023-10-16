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
#include "src/engine/Search.h"
//#include <thread>

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
    SpriteHandler::InitializePromotionMenu();

    /*
        PUT HERE DEBUGGING CODE
    */

    //Search::DebugSearch();

    ChessClock whiteClock;
    ChessClock blackClock;

    Board::InitializeBoard(windowX,windowY);
    MoveTable::CalculateStartMoveData();

    SpriteHandler::LoadFonts(650,300,thisDir);
    SpriteHandler::DisplayCurrentEvaluation();

    whiteClock.InitializeClock(650,400,thisDir);
    blackClock.InitializeClock(650,200,thisDir,whiteClock,blackClock);
    whiteClock.SetTime(6000);
    blackClock.SetTime(6000);

    FEN::ReadPosition(FEN::startFEN);
    //MoveTable::AddCurrentPosition();

    ChessClock::SetActivePlayer(1,whiteClock);

    Board::PrepareBoard();

    Thread clockThread(std::bind(&ChessClock::CountDown,whiteClock,blackClock));
    clockThread.launch();

    MoveTable::GenerateMoves(MoveTable::CurrentMoveList);
    //SpriteHandler::DrawMoveDots(1,MoveTable::GenerateMoves());

    //Board::Promote(3,Piece::white);

    Search thisSearch;
    //Thread searchThread(std::bind(&Search::LogDebugSearch,&thisSearch,1));
    int negMax = Search::NegMax;
    int posMax = Search::PosMax;
    Thread searchThread(std::bind(&Search::SearchMoves,&thisSearch,6,negMax,posMax));
    searchThread.launch();

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
                std::cout << "Pressed" << std::endl;
                if(event.mouseButton.button == Mouse::Left)
                {
                    if(!SpriteHandler::IsPromotion)
                    {
                        bool isEmpty = false;
                        Board::HandleMouseInput(mousePosition);
                        //std::cout << Board::selectedSquare << std::endl;
                        for(int i=0;i<SpriteHandler::pieceNum;i++)
                        {
                            if(SpriteHandler::pieceSprite[i].getGlobalBounds().contains(Board::squarePos[Board::selectedSquare].centerX,Board::squarePos[Board::selectedSquare].centerY))
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
                        {
                            Board::isMove = true;
                            SpriteHandler::DrawMoveDots(Board::selectedSquare,MoveTable::CurrentMoveList);
                        }
                        //else
                            //Piece::spritePtr = nullptr;
                    }
                    else
                    {
                        for(int i=0;i<8;i++)
                        {
                            if(SpriteHandler::promotionPieceSprite[i].getGlobalBounds().contains(mousePosition.x,mousePosition.y))
                            {
                                Board::HandlePromotion(i);
                            }
                        }
                    }
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
                if(event.key.code == Keyboard::T)
                {
                    //Piece::LogPiece(Board::squareState[16]);
                    //MoveTable::GenerateMoves();
                    //SpriteHandler::DrawMoveDots(1,MoveTable::GenerateMoves());
                    std::cout << "... ... ..." << std::endl;
                    std::cout << MoveTable::IsAttacked(43) << std::endl;
                    std::cout << MoveTable::IsAttacked(52) << std::endl;
                    std::cout << MoveTable::IsAttacked(61) << std::endl;
                }
                if(event.key.code == Keyboard::L)
                {
                    FEN testFen;
                    testFen.GetPosition();
                    std::cout << testFen.FENtext << std::endl;
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
        window.draw(whiteClock.clockText);
        window.draw(blackClock.clockText);
        SpriteHandler::DrawPieces(window);
        SpriteHandler::DrawDots(window);
        SpriteHandler::DrawPromotionMenu(window);
        SpriteHandler::DrawDebugShapes(window);
        SpriteHandler::DrawTexts(window);
        window.display();
    }
    return 0;
}