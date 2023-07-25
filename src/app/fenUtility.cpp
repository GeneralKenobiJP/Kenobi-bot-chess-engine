#include "fenUtility.h"
#include <ctype.h>
#include <iostream>
#include "board.h"
#include <MoveTable.h>

const std::string FEN::startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int FEN::ReadLetter(char letter) //lowercase
{
    switch(letter)
    {
        case 'r':
            return Piece::rook;
        case 'n':
            return Piece::knight;
        case 'b':
            return Piece::bishop;
        case 'q':
            return Piece::queen;
        case 'k':
            return Piece::king;
        case 'p':
            return Piece::pawn;
    }
    return Piece::none;
}

void FEN::ReadPosition() //non-static
{
    FEN::ReadPosition(FENtext);
}

void FEN::ReadPosition(std::string fenTxt) //static
{
    int currentFile = 0;
    int currentRank = 7; //8 actually
    int currentSquare;

    //int debugNum = 0;

    while(fenTxt[0]!=' ')
    {
        //std::cout << fenTxt << std::endl;
        //debugNum++;

        //std::cout << "Debug: " << debugNum << std::endl;

        if(fenTxt[0]=='/')
        {
            currentFile = 0;
            currentRank -= 1;
            fenTxt = fenTxt.substr(1,fenTxt.length()-1);
            continue;
        }
        if(isdigit(fenTxt[0]))
        {
            currentFile+=(int)fenTxt[0];
            fenTxt = fenTxt.substr(1,fenTxt.length()-1);
            continue;
        }
        currentSquare = currentRank*8 + currentFile;
        if(fenTxt[0]>90) //checks if lowercase
        {
            Piece::SetPiece(ReadLetter(fenTxt[0]),Piece::black,currentSquare);
            fenTxt = fenTxt.substr(1,fenTxt.length()-1);
            currentFile++;
            continue;
        }
        //uppercase here
        Piece::SetPiece(ReadLetter(fenTxt[0]+32),Piece::white,currentSquare);
        fenTxt = fenTxt.substr(1,fenTxt.length()-1);
        currentFile++;
    }

    if(fenTxt[1] == 'w')
    {
        Board::activePlayer = 1;
    }
    else //'b'
    {
        Board::activePlayer = 2;
    }

    fenTxt = fenTxt.substr(3);

    while(fenTxt[0]!=' ')
    {
        switch(fenTxt[0])
        {
            case 'K':
                MoveTable::W_CanCastleKingside = 1;
                break;
            case 'Q':
                MoveTable::W_CanCastleQueenside = 1;
                break;
            case 'k':
                MoveTable::B_CanCastleKingside = 1;
                break;
            case 'q':
                MoveTable::B_CanCastleQueenside = 1;
                break;
        }
        fenTxt = fenTxt.substr(1);
    }
    //

    //std::cout << "done" << std::endl;
}

//GetPosition