#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

int main(){
    RenderWindow window(VideoMode(453,453),"the chess");

    Texture boardTexture;
    boardTexture.loadFromFile("empty.png");

    Sprite boardSprite(boardTexture);

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
        window.display();
    }
    return 0;
}