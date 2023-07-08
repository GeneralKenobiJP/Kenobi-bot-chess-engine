#pragma once

class Board //static
{
    struct coordinates //x and y position of top-left corner
    {
        int x;
        int y;
        int width;
        int height;
    };

    public:
    static int squareState[64];
    static coordinates squarePos[64];
    static int boardWidth;
    static int boardHeight;

    static void InitializeBoard(int x, int y);
    static void PutOnSquare(int num,int piece, int color);
};