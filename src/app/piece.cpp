#include "piece.h"
#include <iostream>

void Piece::ReadPiece(int num)
{
    int thisType = num << 2;
    int thisColor = num - thisType;
    std::cout << thisType << std::endl;
    std::cout << thisColor << std::endl;
}

//int main(){return 0;}