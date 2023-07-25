#pragma once
#include <../../include/SFML/System/Vector2.hpp>

struct coordinates // x and y position of top-left corner
{
    int x;
    int y;
    int width;
    int height;
    int centerX;
    int centerY;
};

class Board // static
{
public:
    static int squareState[64];
    static coordinates squarePos[64];
    static int boardWidth;
    static int boardHeight;

    static short int selectedSquare;
    static unsigned short activePlayer; //0 = none, 1 = white, 2 = black

    static short promotionSquare; //-1 = none

    static bool isMove;

    static void InitializeBoard(int x, int y); //load squarePos and set squareState to 0
    static void PutOnSquare(int num, int piece, int color); //sets squareState to the input piece
    static void PutOnSquare(int num, int pieceID); //sets squareState to the input piece
    static void RemoveFromSquare(int num); //sets squareState to 0
    static void HandleMouseInput(sf::Vector2i position); //sets selectedSquare
    static void HandleMouseReleased(sf::Vector2i position); //triggers PutPiece() and RemoveFromSquare() on mouse release
    static void DisableSelection(); //sets selectedSquare=-1
    static void ReadSquare(int squareIndex, int &file, int &rank);
    static int CalculateDistance(int squareA, int squareB);
    static void SwitchPlayer();
    static void Promote(int square, int color);
    static void HandlePromotion(int promotionSpriteIndex);
};