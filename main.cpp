#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;

int main(){
    std::string thisDir = "desktop/github-projects/Kenobi-bot-chess-engine/";
    const int windowX=600,windowY=600;
    RenderWindow window(VideoMode(windowX,windowY),"the chess");

    Texture boardTexture;
    Texture pieceTexture;
    boardTexture.loadFromFile(thisDir+"img/chessBoard.png",IntRect(0,0,windowX,windowY));
    pieceTexture.loadFromFile(thisDir+"img/pieces.png");

    Sprite boardSprite(boardTexture);
    Sprite pieceSprite(pieceTexture);

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
        window.draw(pieceSprite);
        window.display();
    }
    return 0;
}