#include "board.h"

void Board::InitializeBoard(int x, int y)
{
    int curX=0;
    int curY=y;
    int deltaX=x/8;
    int deltaY=y/8;
    int curIt;

    boardWidth=x;
    boardHeight=y;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            curIt = 8 * i + j;
            squareState[curIt] = 0;
            squarePos[curIt].x = curX;
            squarePos[curIt].y = curY;
            squarePos[curIt].width = deltaX;
            squarePos[curIt].height = deltaY;
            curX += deltaX;
        }
        curX = 0;
        curY -= deltaY;
    }
}

void Board::PutOnSquare(int num, int piece, int color)
{
    squareState[num] = piece | color;
}

//int main(){return 0;}