#include "button.h"

Button::Button()
{

}

Button::Button(sf::Text thisText, sf::Font thisFont, sf::Color thisColor, sf::Vector2f thisSize)
{
    text = thisText;
    font = thisFont;
    color = thisColor;
    size = thisSize;
}


Button::Button(sf::Text thisText, sf::Font thisFont, sf::Color thisColor, sf::Vector2f thisSize, sf::Vector2f thisPosition)
{
    text = thisText;
    font = thisFont;
    color = thisColor;
    size = thisSize;
    position = thisPosition;
}

void Button::SetPosition(sf::Vector2f newPosition)
{
    position = newPosition;
}

template <typename F>
void Button::SetFunction(F Function)
{
    functionPtr = Function;
}

template <typename F, typename A>
void Button::SetFunction(F Function, A &argument)
{
    functionPtr = Function;
    argumentPtr = &argument;
}

template <typename A>
void Button::SetFunctionArgument(A &argument)
{
    argumentPtr = &argument;
}

void Button::PressButton()
{
    //functionPtr(argumentPtr);
    functionPtr();
}

void Button::Draw(sf::RenderWindow &window)
{
    window.draw(shape);
    window.draw(text);
}