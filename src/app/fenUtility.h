#pragma once
#include <string>

class FEN
{
    std::string FENtext;

    public:
    static const std::string startFEN;
    
    void ReadPosition();
    static void ReadPosition(std::string fenTxt);
    //void GetPosition();
};