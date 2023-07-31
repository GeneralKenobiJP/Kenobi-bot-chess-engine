#pragma once
#include <SFML/Graphics.hpp>

class Button
{
    sf::Rect<float> rect;
    sf::RectangleShape shape;
    public:
    sf::Text text;
    sf::Font font;
    sf::Color color;
    sf::Vector2f position;
    sf::Vector2f size;

    void (*functionPtr)();
    void* argumentPtr;

    Button();
    Button(sf::Text thisText, sf::Font thisFont, sf::Color thisColor, sf::Vector2f thisSize);
    Button(sf::Text thisText, sf::Font thisFont, sf::Color thisColor, sf::Vector2f thisSize, sf::Vector2f thisPosition);

    void SetPosition(sf::Vector2f newPosition);

    template <typename F>
    void SetFunction(F Function);

    template <typename F, typename A>
    void SetFunction(F Function, A &argument);

    template <typename A>
    void SetFunctionArgument(A &argument);

    void PressButton();
};