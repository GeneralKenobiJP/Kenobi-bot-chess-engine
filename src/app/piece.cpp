#include "piece.h"
#include <iostream>

void Piece::ReadPiece(int thisNum)
{
    //thisNum = thisNum & Piece::pieceMask;
    int thisType = (thisNum << 2);
    thisType = thisType & Piece::pieceMask;
    thisType /= 4;
    int thisColor = thisNum - thisType;
    std::cout << thisType << std::endl;
    std::cout << thisColor << std::endl;
}

//int main(){return 0;}