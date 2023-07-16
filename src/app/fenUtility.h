#pragma once
#include <string>
#include "piece.h"

class FEN
{
    std::string FENtext;

    static int ReadLetter(char letter);

    public:
    static const std::string startFEN;
    
    void ReadPosition();
    static void ReadPosition(std::string fenTxt);
    //void GetPosition();
};