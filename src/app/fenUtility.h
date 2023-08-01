#pragma once
#include <string>
#include "piece.h"

class FEN
{
    static int ReadLetter(char letter);

    public:
    static const std::string startFEN;
    std::string FENtext;
    
    void ReadPosition(); //Read FENtext of an object of the FEN class
    static void ReadPosition(std::string fenTxt); //set the board according to the fenTxt
    void GetPosition();
};