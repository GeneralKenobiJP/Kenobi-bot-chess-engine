#include "clock.h"
#include <iostream>

const std::string FONT_DIR = "img/fonts/Roboto-Light.ttf";
unsigned short ChessClock::activePlayer;
ChessClock* ChessClock::chessClockPtr = nullptr;

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

    this->SetTimeDisplayFormat();
}
void ChessClock::SetTimeDisplayFormat()
{
    seconds = timeLeft/10;
    minutes = seconds/60;
    seconds = seconds%60;
    int decimal;
    std::string thisString;
    if(seconds!=0)
        thisString = std::to_string(minutes)+":"+std::to_string(seconds);
    else
        thisString = std::to_string(minutes)+':'+std::to_string(seconds)+'0';

    if(timeLeft<300)
    {
        decimal = timeLeft%10;
        thisString += '.'+std::to_string(decimal);
    }

    clockText.setString(thisString);
}
void ChessClock::CountDown(ChessClock &whiteClock, ChessClock &blackClock)
{/*
    clock_t startTime;
    clock_t endTime;
    double timeDifference;
    if(ChessClock::activePlayer!=0)
    {
        startTime = clock();
        endTime = startTime+0.1*CLOCKS_PER_SEC; //0.1 = 0.1s
        while(clock() < endTime);
        if(ChessClock::activePlayer==1)
        {
            whiteClock.timeLeft-=1;
            whiteClock.SetTimeDisplayFormat();
        }
        else
        {
            blackClock.timeLeft-=1;
            blackClock.SetTimeDisplayFormat();
        }
    }*/

    sf::Clock timer;
    sf::Time startTime;
    sf::Time endTime;
    

    if(ChessClock::activePlayer!=0)
    {
        
        startTime = timer.getElapsedTime();
        endTime = startTime + sf::seconds(0.1f);
        while(timer.getElapsedTime()<endTime);
        chessClockPtr->timeLeft-=1;
        chessClockPtr->SetTimeDisplayFormat();
    }
}
void ChessClock::SetActivePlayer(unsigned short playerID, ChessClock &thisClock)
{
    ChessClock::activePlayer = playerID;
    ChessClock::chessClockPtr = &thisClock;
}
/*void ChessClock::ThreadClock(ChessClock &whiteClock, ChessClock &blackClock)
{
    ChessClock::CountDown(whiteClock,blackClock);
}*/