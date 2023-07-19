#include "clock.h"
#include <iostream>

const std::string FONT_DIR = "img/fonts/Roboto-Light.ttf";
unsigned short ChessClock::activePlayer;

void ChessClock::InitializeClock(int posX, int posY, std::string dir)
{
    //clockFont.loadFromFile("../../img/fonts/Roboto-Light.ttf");
    if (!clockFont.loadFromFile(dir+FONT_DIR))
    {
        std::cout << "clockFont not loaded properly" << std::endl;
    }
    clockText.setFont(clockFont);
    clockText.setPosition(posX, posY);
    clockText.setCharacterSize(ChessClock::FONT_SIZE);
    clockText.setString("Test");
}
void ChessClock::SetTime(int startingTime)
{
    timeStart = startingTime;
    timeLeft = startingTime;
    ChessClock::activePlayer = 0;

    seconds = timeLeft/10;
    minutes = seconds/60;
    seconds = seconds%60;
    std::string thisString;
    if(seconds!=0)
        thisString = std::to_string(minutes)+":"+std::to_string(seconds);
    else
        thisString = std::to_string(minutes)+':'+std::to_string(seconds)+'0';
    clockText.setString(thisString);
}