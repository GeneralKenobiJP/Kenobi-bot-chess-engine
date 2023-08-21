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
    void GetPosition(int squareState[64], short activePlayer, bool W_K, bool W_Q, bool B_K, bool B_Q, int enPassantSquare, int consecutiveMoves);

    const std::string CutHalfmoves() const;

    bool operator == (const FEN &f);
};