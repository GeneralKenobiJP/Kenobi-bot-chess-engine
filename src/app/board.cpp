#include "board.h"
#include "piece.h"
#include <cmath>

int Board::squareState[64];
coordinates Board::squarePos[64];
int Board::boardWidth;
int Board::boardHeight;
short int Board::selectedSquare=-1;
bool Board::isMove=0;

void Board::InitializeBoard(int x, int y)
{
    int curX=0;
    int curY=y;
    int deltaX=x/8;
    int deltaY=y/8;
    int curIt;

    curY-=deltaY;

    Board::boardWidth=x;
    Board::boardHeight=y;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            curIt = 8 * i + j;
            Board::squareState[curIt] = 0;
            Board::squarePos[curIt].x = curX;
            Board::squarePos[curIt].y = curY;
            Board::squarePos[curIt].width = deltaX;
            Board::squarePos[curIt].height = deltaY;
            curX += deltaX;
        }
        curX = 0;
        curY -= deltaY;
    }
}

void Board::PutOnSquare(int num, int piece, int color)
{
    Board::squareState[num] = (piece | color);
}

void Board::RemoveFromSquare(int num)
{
    Board::squareState[num] = Piece::none;
}

void Board::HandleMouseInput(sf::Vector2i position)
{
    int i=0;
    int iHolder;
    while(position.x>Board::squarePos[i].x && i<8)
    {
        i++;
    }
    i-=1;
    iHolder = i;
    while(position.y<Board::squarePos[i].y && i<=64-(8-iHolder))
    {
        i+=8;
    }
    i-=8;
    Board::selectedSquare = i;
    //add highlighting
}

void Board::HandleMouseReleased(sf::Vector2i position)
{
    for(int i=0;i<64;i++)
    {
        if(position.x>=Board::squarePos[i].x && position.x<Board::squarePos[i].x+Board::squarePos[i].width)
            if(position.y>=Board::squarePos[i].y && position.y < Board::squarePos[i].y+Board::squarePos[i].height)
            {
                Piece::PutPiece(Board::squarePos[i].x,Board::squarePos[i].y);
                break;
            }
        if(i==63)
            Piece::PutPiece(Board::squarePos[63].x,Board::squarePos[63].y); //foolproof code ensuring piece ejection into a sub-orbital flight is impossible
    }
    Board::RemoveFromSquare(Board::selectedSquare);
}

void Board::DisableSelection()
{
    //Board::selectedSquare = -1;
    Piece::spritePtr = nullptr;
}

void Board::ReadSquare(int squareIndex, int &file, int &rank)
{
    file = squareIndex%8;
    rank = squareIndex/8;
}

int Board::CalculateDistance(int squareA, int squareB)
{
    int fileA;
    int fileB;
    int rankA;
    int rankB;

    Board::ReadSquare(squareA,fileA,rankA);
    Board::ReadSquare(squareB,fileB,rankB);

    int dist = std::abs(fileA-fileB) + std::abs(rankA-rankB);

    return dist;
}