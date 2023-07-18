#pragma once
#include <../../include/SFML/System/Vector2.hpp>

struct coordinates // x and y position of top-left corner
{
    int x;
    int y;
    int width;
    int height;
};

class Board // static
{
public:
    static int squareState[64];
    static coordinates squarePos[64];
    static int boardWidth;
    static int boardHeight;

    static short int selectedSquare;

    static bool isMove;

    static void InitializeBoard(int x, int y);
    static void PutOnSquare(int num, int piece, int color);
    static void RemoveFromSquare(int num);
    static void HandleMouseInput(sf::Vector2i position);
    static void HandleMouseReleased(sf::Vector2i position);
    static void DisableSelection();
};