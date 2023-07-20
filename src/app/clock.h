#pragma once
#include <../../include/SFML/Graphics/Font.hpp>
#include <../../include/SFML/Graphics/Text.hpp>
#include <time.h>
#include <../../include/SFML/System/Clock.hpp>

class ChessClock
{ //private as for now
    static const int FONT_SIZE=34;
    //DEFINED IN clock.cpp:
    //const std::string FONT_DIR = "img/fonts/Roboto-Light.ttf";
    int timeStart; //1 = 0.1s
    int timeLeft;
    static unsigned short activePlayer; //0 = none, 1 = white, 2 = black
    sf::Font clockFont;
    int seconds;
    int minutes;

    void SetTimeDisplayFormat();

    public:
    sf::Text clockText;
    static ChessClock* chessClockPtr;

    void InitializeClock(int posX, int posY, std::string dir); //initializes clock and sets positions to the input, it takes root directory for font search
    void SetTime(int startingTime);
    static void CountDown(ChessClock &whiteClock, ChessClock &blackClock); //decreases the timer
    static void SetActivePlayer(unsigned short playerID, ChessClock &thisClock);
    //static void ThreadClock(ChessClock &whiteClock, ChessClock &blackClock); //function used for multi-threading the clock
    //maybe void Flag
};